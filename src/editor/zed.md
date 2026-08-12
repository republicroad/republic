
# Zed 中使用公司内网 Maven 仓库（JDTLS / m2e）

> 记录 `ebike-account-next` 多模块 Maven 工程在 Zed 中编译报错与 go to definition 失效的完整排查过程，以及最终收敛的最佳实践。

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
| Zed Java 扩展 | 官方 `java` 6.8.25（jdtls 1.60.0，路径 `C:\Users\...\AppData\Local\Zed\extensions\work\java`） |
| 本地仓库 | `D:\Software\Maven\repo` |
| 用户级 settings | `~/.m2/settings.xml` |
| 全局 settings | `D:\Software\Maven\apache-maven-3.9.9\conf\settings.xml` |

## 现象 1：Non-resolvable parent POM

### 报错

```
Non-resolvable parent POM for com.xiaoantech.ebike:ebike-account-next:0.0.1-SNAPSHOT:
The following artifacts could not be resolved:
com.xiaoantech:xiaoantech-dependencies:pom:2.0-SNAPSHOT (absent)
```

### 根因

Zed 的 Java 语言服务器（JDTLS，内嵌 m2e Maven）**只读用户级 `~/.m2/settings.xml`**，
**不读** `D:\Software\Maven\apache-maven-3.9.9\conf\settings.xml`。

公司私有仓库的镜像 / profile / 凭据只配置在 `D:\...\conf\settings.xml` 里，因此 CLI 用
`-s` 指定该文件能通过，而 Zed 解析不到父 POM。

> VS Code 能用的原因是 `.vscode/settings.json` 里通过
> `java.configuration.maven.userSettings` / `globalSettings` 显式把 IDE 指向了全局配置。
> Zed 默认没有这一层指向。

### 解决（方案 A）

把内网仓库所需配置全部收敛到用户级 `~/.m2/settings.xml`：

```xml
<settings>
  <localRepository>D:\Software\Maven\repo</localRepository>

  <servers>
    <server>
      <id>nexus-public</id>
      <username>${env.NEXUS_USER}</username>
      <password>${env.NEXUS_PASSWORD}</password>
    </server>
  </servers>

  <mirrors>
    <mirror>
      <id>nexus-public</id>
      <name>nexus maven-public group</name>
      <url>https://mvn.xiaoantech.com/repository/maven-public/</url>
      <mirrorOf>*</mirrorOf>
    </mirror>
    <mirror>
      <id>maven-default-http-blocker</id>
      <mirrorOf>external:http:*</mirrorOf>
      <url>http://0.0.0.0/</url>
      <blocked>true</blocked>
    </mirror>
  </mirrors>

  <profiles>
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
  </profiles>

  <activeProfiles>
    <activeProfile>maven-public</activeProfile>
  </activeProfiles>
</settings>
```

### 验证

```bash
mvn -s ~/.m2/settings.xml validate -f pom.xml   # 8 模块 SUCCESS
mvn -s ~/.m2/settings.xml compile -f pom.xml    # 8 模块 SUCCESS
```

## 现象 2：go to definition 无法跳转

编译已通过，但 F12 / Ctrl+点击 跳不到定义。

### 根因 2a：双 Java 扩展冲突

同时安装了官方 `java` 扩展与社区 `java-eclipse-jdtls` 扩展，后者占用 `java` 语言，
但它的 `bin\jdtls.bat` 缺失，导致：

```
Failed to start language server "java"
```

**解决**：卸载社区扩展 `java-eclipse-jdtls`，只保留官方 `java`。

### 根因 2b：脏 JDTLS 工作区残留 "not up-to-date"

卸载社区扩展后仍无法跳转。原因是 JDTLS 工作区（`%APPDATA%\jdtls-<hash>`）里残留了
父 POM 解析失败时期的残缺 classpath，标记为：

```
Project configuration is not up-to-date with pom.xml
```

JDTLS 通过 LSP 通知 `language/actionableNotification` 请求界面弹出
"Update project configuration"，但 **Zed 不处理该通知**（日志可见
`unhandled notification language/actionableNotification`），于是 classpath 永不重建，
`textDocument/definition` 返回空。

### 解决

1. 关闭 Zed，删除本项目对应的 JDTLS 工作区目录：
   ```
   C:\Users\...\AppData\Roaming\jdtls-3c0079a1922676883ef8c3d4e1ada36d7c0953a3
   ```
2. 在 Zed 全局设置 `C:\Users\...\AppData\Roaming\Zed\settings.json` 中加 LSP 初始化选项：
   ```jsonc
   "lsp": {
     "jdtls": {
       "initialization_options": {
         "settings": {
           "java.configuration.updateBuildConfiguration": "automatic"
         }
       }
     }
   }
   ```
3. 重启 Zed，等待全新导入（日志中 `Progressive import: reporting 8 new project(s)` →
   `build jobs finished`），go to definition 恢复。

## 最佳实践收尾

- **单一事实来源**：镜像 / profile / 凭据 / localRepository 全部收敛在
  `~/.m2/settings.xml`。CLI、Zed、VS Code、IntelliJ 的 Maven 默认都读它，
  IDE 里不需要再配 `java.configuration.maven.userSettings` / `globalSettings` 覆盖
  （因此从 Zed 配置中删除这两项）。
- **凭据不入文件**：改用环境变量 `setx NEXUS_USER ...` / `setx NEXUS_PASSWORD ...`，
  settings 中写 `${env.NEXUS_USER}` / `${env.NEXUS_PASSWORD}`。
- **Windows 环境变量传播注意**：`setx` 只写注册表，已运行的 explorer.exe 不会自动刷新。
  从开始菜单/桌面启动的进程继承的是 explorer 的旧环境，拿不到新变量；
  需要**注销重登一次**（或重启）后，Explorer 启动的 Zed 才生效。
- **镜像 `mirrorOf=*`**：单一 Nexus group 且无 deploy 需求时 `*` 正确（group 已代理中央仓库）；
  若后续加入独立私有仓库或需要 `mvn deploy` 到其它 repo，改为 `*,!<repo-id>` 排除。

## 关键路径速查

| 路径 | 用途 |
|---|---|
| `C:\Users\...\.m2\settings.xml` | 用户级 Maven 配置（单一事实来源） |
| `D:\Software\Maven\apache-maven-3.9.9\conf\settings.xml` | 全局 Maven 配置（CLI 备用，需与用户级保持一致） |
| `C:\Users\...\AppData\Roaming\Zed\settings.json` | Zed 全局设置（LSP 初始化选项） |
| `C:\Users\...\AppData\Local\Zed\logs\Zed.log` | Zed 主日志（搜 `Failed to start language server`、`actionableNotification`） |
| `%APPDATA%\jdtls-<hash>\.metadata\.log` | 各工程 JDTLS 工作区日志（搜 `Importing Maven project`、`not-up-to-date`、`build jobs`） |
| `D:\Software\Maven\repo` | 本地仓库（`com\xiaoantech\xiaoantech-dependencies\2.0-SNAPSHOT` 在其中） |

## 排查方法论要点

1. **先 CLI 后 IDE**：用 `mvn -s <settings>` 验证 settings 是否正确，缩小范围到 IDE 层。
2. **认清 m2e 行为**：JDTLS 内嵌 Maven，只读用户级 settings + 自带的全局 settings，
   与独立安装的 Maven 的 `conf/settings.xml` 无关。
3. **工作区可重建**：`%APPDATA%\jdtls-<hash>` 损坏/残留旧 classpath 时删除让 IDE 重导，
   是"项目配置过期"类问题的根治手段。
4. **只保留一个 Java 扩展**：官方 `java` 与社区 `java-eclipse-jdtls` 不能共存。
5. **反编译临时文件是噪声**：go to definition 跳入依赖 class 时会解压到
   `%TEMP%\jdtls_decompiled\...`，临时目录被清后旧标签页会报
   `Cannot read the source ... File not found`，关闭标签页即可，与工程无关。
