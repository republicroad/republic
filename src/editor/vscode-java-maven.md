
# VS Code 中使用公司内网 Maven 仓库（redhat.java / m2e）

> 记录 `ebike-account-next` 多模块 Maven 工程在 VS Code 中 Problems 报 `Non-resolvable parent POM` 的完整排查过程，以及与 Zed 收敛为一致配置后的最佳实践。

## 背景

`ebike-account-next` 是 DDD 多模块 Maven 工程（8 个 module），根 POM 的父 POM 为
`com.xiaoantech:xiaoantech-dependencies:2.0-SNAPSHOT`，只发布在内部 Nexus group：

```
https://mvn.xiaoantech.com/repository/maven-public/
```

## 环境

| 项 | 值 |
|---|---|
| 操作系统 | Windows |
| JDK | 21.0.4（`C:\Program Files\jdk`，JAVA_HOME 已设置） |
| Maven | 3.9.9（`D:\Software\Maven\apache-maven-3.9.9`） |
| VS Code Java 扩展 | 官方 Extension Pack for Java（`redhat.java`，内嵌 m2e 解析 Maven） |
| 本地仓库 | `D:\Software\Maven\repo` |
| 用户级 settings | `~/.m2/settings.xml` |
| 全局 settings | `D:\Software\Maven\apache-maven-3.9.9\conf\settings.xml` |
| JDT 工作区 | `%APPDATA%\jdtls-<hash>`（VS Code / Zed 共用 jdtls 机制） |

## 现象：Problems 面板报 Non-resolvable parent POM

### 报错

```
Project build error: Non-resolvable parent POM for com.xiaoantech.ebike:ebike-account-next:0.0.1-SNAPSHOT:
The following artifacts could not be resolved:
com.xiaoantech:xiaoantech-dependencies:pom:2.0-SNAPSHOT (absent):
Could not find artifact com.xiaoantech:xiaoantech-dependencies:pom:2.0-SNAPSHOT
and 'parent.relativePath' points at no local POM
```

## 排查过程

### 第 0 步：先明确 IDE 读哪些 settings

VS Code 的 Java 语言服务器（redhat.java）通过以下配置项决定 Maven settings 来源：

- `java.configuration.maven.userSettings`：指定用户级 settings
- `java.configuration.maven.globalSettings`：指定全局 settings
- 都为空时，使用 jdtls 自带的全局 settings（与独立 Maven 的 `conf/settings.xml` 无关）

> 排查初期项目 `.vscode/settings.json` 曾把这两项都指向
> `D:\Software\Maven\apache-maven-3.9.9\conf\settings.xml`，因此 IDE 实际读的是这份文件。

### 第 1 步：用 CLI 复现，缩小范围

```bash
mvn validate            # 报与 IDE 完全相同的 (absent) 错误
mvn help:effective-settings   # 观察最终生效的 mirror / profile / localRepository
```

`mvn -X` 能看到实际尝试的仓库：

```
[DEBUG] Resolving artifact com.xiaoantech:xiaoantech-dependencies:pom:2.0-SNAPSHOT from
        [nexus-public (https://mvn.xiaoantech.com/repository/maven-public/, default, releases)]
```

> `default, releases` 表示该仓库**只有 releases 策略，snapshot 关闭** —— 这是后面的关键线索。

### 第 2 步：定位根因 A —— 多个 `mirrorOf=*` 只有一个生效

`conf/settings.xml` 里原来配了 **3 个 `mirrorOf=*` 的 mirror**：

| 顺序  | id             | url                      | 问题                      |
| --- | -------------- | ------------------------ | ----------------------- |
| 1   | `alimaven`     | `.../aliyun-repository/` | **Maven 只认第一个匹配**，始终命中它 |
| 2   | `nexus-public` | `.../maven-releases/`    | 永不生效                    |
| 3   | `nexus-public` | `.../maven-snapshots/`   | 永不生效                    |

而 `aliyun-repository` 只是阿里云公共代理：

- 没有对应 `<server>` 凭据 → 请求 401
- 即使带凭据，私有 `com.xiaoantech` 构件也返回 404

→ 父 POM 无法解析，报 `(absent)`。

### 第 3 步：用 PowerShell + Basic Auth 探测 Nexus 验证假设

```powershell
# 列出 Nexus 仓库（发现 maven-public 是 group 类型）
$u="<user>"; $p="<pass>"
$b=[Convert]::ToBase64String([Text.Encoding]::UTF8.GetBytes("${u}:${p}"))
Invoke-WebRequest -Uri "https://mvn.xiaoantech.com/service/rest/v1/repositories" `
  -Headers @{Authorization="Basic $b"}

# 验证父 POM 是否在某仓库（无凭据 401 = 需认证；带凭据 404 = 不含该构件）
Invoke-WebRequest -Uri "https://mvn.xiaoantech.com/repository/maven-snapshots/com/xiaoantech/xiaoantech-dependencies/maven-metadata.xml" `
  -Headers @{Authorization="Basic $b"}     # 200 → 存在
Invoke-WebRequest -Uri "https://mvn.xiaoantech.com/repository/maven-public/com/xiaoantech/xiaoantech-dependencies/maven-metadata.xml" `
  -Headers @{Authorization="Basic $b"}     # 200 → group 也聚合了它
Invoke-WebRequest -Uri "https://mvn.xiaoantech.com/repository/aliyun-repository/com/xiaoantech/xiaoantech-dependencies/maven-metadata.xml" `
  -Headers @{Authorization="Basic $b"}     # 404 → 纯阿里代理，确认根因 A
```

结论：父 POM 存在于 `maven-snapshots` 与 `maven-public` group；`maven-public` 同时也代理了
中央仓库（lombok / spring-boot 等公共构件都能 200）。

### 第 4 步：修复 mirror —— 单一 Nexus group

把 3 个 `mirrorOf=*` 合并为单个指向 `maven-public` 的 mirror，id 复用 `nexus-public`，
让已有 `<server id="nexus-public">` 凭据自动生效：

```xml
<mirror>
  <id>nexus-public</id>
  <name>nexus maven-public group</name>
  <url>https://mvn.xiaoantech.com/repository/maven-public/</url>
  <mirrorOf>*</mirrorOf>
</mirror>
```

保留 `maven-default-http-blocker`（`external:http:*`，防外部 HTTP 仓库）。

### 第 5 步：定位根因 B —— mirror 不携带 snapshot 策略

只改 mirror 后 `mvn validate` 仍报 `(absent)`。原因：**mirror 不携带 release/snapshot 策略**，
策略继承自被镜像的仓库。父 POM 作为 SNAPSHOT，其所属的 `central` 仓库默认 **releases-only**，
所以 `2.0-SNAPSHOT` 仍拉不到。

解决：在 profile 中声明开启 snapshots 的 repository（同时配 pluginRepository），并激活：

```xml
<profile>
  <id>maven-public</id>
  <repositories>
    <repository>
      <id>maven-public-repo</id>
      <url>https://mvn.xiaoantech.com/repository/maven-public/</url>
      <releases><enabled>true</enabled></releases>
      <snapshots><enabled>true</enabled></snapshots>
    </repository>
  </repositories>
  <pluginRepositories>
    <pluginRepository>
      <id>maven-public-repo</id>
      <url>https://mvn.xiaoantech.com/repository/maven-public/</url>
      <releases><enabled>true</enabled></releases>
      <snapshots><enabled>true</enabled></snapshots>
    </pluginRepository>
  </pluginRepositories>
</profile>

<activeProfiles>
  <activeProfile>maven-public</activeProfile>
</activeProfiles>
```

> 该仓库 URL 会被 `mirrorOf=*` 镜像到 maven-public group，真正起作用的是它声明的 snapshot 策略。

> 编辑坑：`settings.xml` 里已有 `<profiles>` 段，追加 profile 要放进**同一段**内，
> 否则报 `Duplicated tag: 'profiles'`（XML 不允许同名顶层元素重复）。

### 第 6 步：验证

```bash
mvn help:effective-settings   # 确认 mirror / profile / activeProfile 生效
mvn validate                  # 8 模块 SUCCESS
```

本地仓库出现父 POM（时间戳快照）：

```
D:\Software\Maven\repo\com\xiaoantech\xiaoantech-dependencies\2.0-SNAPSHOT\
├── xiaoantech-dependencies-2.0-<yyyyMMdd>.<HHmmss>-<n>.pom
├── xiaoantech-dependencies-2.0-SNAPSHOT.pom
└── maven-metadata-nexus-public.xml
```

### 第 7 步：让 VS Code 生效

1. 命令面板执行 **Java: Clean Java Language Server Workspace**（会清除 `%APPDATA%\jdtls-<hash>` 工作区）
2. 右键 `pom.xml` → **Reload Maven Projects**（或 **Maven: Reload Projects**）
3. 等重新导入完成，Problems 面板错误消失

> 极少数情况下残留旧 classpath，手动删除本项目对应的 `%APPDATA%\jdtls-<hash>` 目录再重导即可。

## 与 Zed 收敛为一致（当前状态）

- **单一事实来源**：镜像 / profile / 凭据 / localRepository 全部收敛在
  `~/.m2/settings.xml`。CLI、Zed、VS Code、IntelliJ 的 Maven 默认都读它。
- **`.vscode/settings.json` 去掉 settings 覆盖**：删除
  `java.configuration.maven.userSettings` / `globalSettings`，只保留：
  ```jsonc
  {
      "maven.view": "flat",
      "java.maven.downloadSources": true
  }
  ```
- **全局 `conf/settings.xml` 与用户级保持一致**：作为 CLI 备用，同步同样的
  mirror / profile / activeProfile；凭据同样用 `${env.NEXUS_USER}` / `${env.NEXUS_PASSWORD}`。
- **凭据不入文件**：`setx NEXUS_USER ...` / `setx NEXUS_PASSWORD ...`，settings 中写
  `${env.NEXUS_USER}` / `${env.NEXUS_PASSWORD}`。
  - `setx` 只写注册表，**已运行的进程不会自动刷新环境变量**；改完后注销重登一次再开 IDE。
- **镜像 `mirrorOf=*`**：单一 Nexus group 且无 deploy 需求时 `*` 正确（group 已代理中央仓库）；
  若后续加入独立私有仓库或需要 `mvn deploy`，改为 `*,!<repo-id>` 排除。

## 关键路径速查

| 路径                                                                 | 用途                                                              |
| ------------------------------------------------------------------ | --------------------------------------------------------------- |
| `C:\Users\...\.m2\settings.xml`                                    | 用户级 Maven 配置（单一事实来源）                                            |
| `D:\Software\Maven\apache-maven-3.9.9\conf\settings.xml`           | 全局 Maven 配置（CLI 备用，需与用户级保持一致）                                   |
| `C:\Users\...\Documents\workspace\<project>\.vscode\settings.json` | 项目级 VS Code 设置（Java 扩展读它）                                       |
| `C:\Users\...\AppData\Roaming\Code\User\settings.json`             | VS Code 用户级设置                                                   |
| `%APPDATA%\jdtls-<hash>`                                           | 各工程 JDT 语言服务器工作区（删掉可强制重导）                                       |
| `%TEMP%\jdtls_decompiled`                                          | 依赖 class 反编译临时目录（被清后旧标签页报 File not found，关闭即可）                  |
| `D:\Software\Maven\repo`                                           | 本地仓库（`com\xiaoantech\xiaoantech-dependencies\2.0-SNAPSHOT` 在其中） |

## 排查方法论要点

1. **先 CLI 后 IDE**：用 `mvn validate` / `mvn help:effective-settings` 验证 settings 是否正确，
   缩小范围到 IDE 层。
2. **多个 `mirrorOf=*` 是反模式**：Maven 只取第一个匹配的 mirror，其余永不生效；
   私有+公共混用时优先用 Nexus group 单一 mirror。
3. **mirror 不携带 snapshot 策略**：SNAPSHOT 构件解析不了时，在 profile 里声明
   `<snapshots><enabled>true</enabled></snapshots>` 的 repository 并激活。
4. **探测 Nexus 用 Basic Auth**：无凭据 401 = 需认证；带凭据 404 = 该仓库不含构件；
   `/service/rest/v1/repositories` 可列出所有仓库区分 group / hosted / proxy。
5. **工作区可重建**：`%APPDATA%\jdtls-<hash>` 残留旧 classpath 导致
   "Project configuration is not up-to-date" 时，删除重导是根治手段。
6. **凭据走环境变量**：settings 里写 `${env.NEXUS_USER}`，不落盘明文密码；
   注意 `setx` 后需注销重登才传播到已运行进程。

## 相关文档

- [Nexus 仓库类型（hosted / proxy / group）](../devops/nexus.md)
