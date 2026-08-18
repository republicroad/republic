# Podman on Windows 最佳实践

## 安装

可以按命令行步骤安装，也可以直接安装 Podman Desktop，由其引导完成 WSL2 和 Podman 的安装。

### 前置条件：启用 WSL2

```powershell
# 以管理员身份运行 PowerShell

# 1. 启用 WSL 功能
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart

# 2. 启用虚拟机平台
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart

# 3. 重启电脑
Restart-Computer

# 4. 重启后，设置 WSL2 为默认版本
wsl --set-default-version 2

# 5. 安装 WSL2 Linux 内核更新包（如提示）
wsl --update
```

> **要求**：Windows 10 1903+ (Build 18362+) 或 Windows 11。推荐 Windows 11 22H2+ 以使用 mirrored 网络模式。

### 方式一：命令行安装

#### 安装 Podman

```powershell
# 方式一：winget（推荐）
winget install -e --id RedHat.Podman

# 方式二：choco
choco install podman -y

# 方式三：scoop
scoop install podman
```

#### 初始化 Podman Machine

```powershell
# 创建 WSL 虚拟机（首次使用）
podman machine init

# 启动
podman machine start

# 验证
podman --version
podman machine ls
```

`podman machine init` 会：
1. 下载 Fedora CoreOS 镜像到 `~\.local\share\containers\podman\machine\wsl\`
2. 创建 WSL 发行版 `podman-machine-default`
3. 在 `%APPDATA%\containers\` 生成默认配置文件
4. 设置 systemd 启动 podman 服务

### 方式二：Podman Desktop

[Podman Desktop](https://podman-desktop.io/) 是 Podman 的图形化管理工具，提供容器、镜像、Pod 的可视化管理，以及 Machine 的创建和配置。

#### 下载安装

```powershell
# 方式一：winget（推荐）
winget install -e --id RedHat.Podman-Desktop

# 方式二：从官网下载安装包
# https://podman-desktop.io/downloads
```

#### 在 Desktop 中初始化 WSL Podman Machine

1. 启动 Podman Desktop
2. 首次启动会检测到未安装 Podman Engine，点击 **Install** 按钮
3. 在弹出的 **Install Podman** 页面：
   - 选择 **Install on WSL**（推荐）
   - 选择 WSL 发行版（默认 Ubuntu，可选 Fedora 等）
   - 点击 **Install** 开始安装
4. 安装完成后，Podman Desktop 自动创建并启动 WSL Podman Machine
5. 在首页看到绿色 **Podman Engine** 状态即表示成功

#### 在 Desktop 中配置 Machine

- 点击左下角齿轮图标 → **Resources** → **Podman Machine**
- 可查看/修改 CPU、内存、磁盘等资源分配
- 点击 **Create new** 可创建额外的 Machine（如需要 rootful 模式）

> **注意**：Podman Desktop 和命令行使用同一个 Podman Engine，配置互通。通过 Desktop 创建的 Machine 在命令行 `podman machine ls` 中同样可见。

### 验证安装

```powershell
# 检查 podman 版本
podman --version

# 检查 machine 状态
podman machine ls

# 检查 WSL 发行版
wsl --list --verbose

# 测试拉取镜像
podman pull docker.io/library/hello-world
podman run docker.io/library/hello-world
```

---

## 配置

### 配置文件目录

```
C:\Users\<user>\AppData\Roaming\containers\     ← Podman 读取（bind mount 到 WSL）
├── containers.conf                              # 主配置（代理、引擎）
├── registries.conf                              # 镜像源配置
├── registries.conf.d/                           # 镜像源 drop-in 配置
├── certs.d/                                     # TLS 证书
└── podman-connections.json                      # machine 连接信息

C:\Users\<user>\.wslconfig                       ← WSL2 全局配置

C:\Users\<user>\.config\containers\              ← Podman CLI 客户端 + machine 元数据
└── podman\machine\wsl\                          # machine 配置（不要手动编辑）
```

> **关键**：`%APPDATA%\containers\` 是 Podman machine 唯一读取的配置目录。修改后通过 bind mount 实时同步到 WSL `/etc/containers/`，无需重启 machine。

### .wslconfig

**路径**：`C:\Users\<user>\.wslconfig`

```ini
[wsl2]
networkingMode=mirrored
```

启用 WSL2 mirrored 网络模式后，WSL 共享 Windows 网络栈：

| 场景 | mirrored 前 | mirrored 后 |
|---|---|---|
| WSL 内 `127.0.0.1:7890` | ❌ 连接拒绝 | ✅ 访问 Windows Clash |
| WSL 内 DNS 解析 | WSL 独立 DNS | 共享 Windows DNS |
| WSL 内直连外网 | NAT 转发 | 共享 Windows 网卡 |

修改后需重启 WSL：`wsl --shutdown && podman machine start`

### registries.conf

**路径**：`%APPDATA%\containers\registries.conf`

```toml
unqualified-search-registries = ["docker.io"]

[[registry]]
location = "docker.io"
[[registry.mirror]]
location = "docker.1ms.run"
[[registry.mirror]]
location = "docker.m.daocloud.io"
[[registry.mirror]]
location = "docker.nju.edu.cn"

# ghcr.io（GitHub Container Registry）直连延迟约 0.5s，国内无需镜像源。
# 如直连变慢，取消注释以下配置启用镜像加速。
# [[registry]]
# location = "ghcr.io"
# [[registry.mirror]]
# location = "ghcr.1ms.run"
```

**字段说明**：

| 字段 | 作用 |
|---|---|
| `unqualified-search-registries` | 无前缀镜像名的搜索顺序（如 `pull nginx` 会去这里找） |
| `[[registry]]` | 定义一个 registry 的行为 |
| `[[registry.mirror]]` | 该 registry 的镜像站（pull 时依次尝试） |

**pull 流程**：`podman pull otel/opentelemetry-collector-contrib:0.115.1`

```
1. 解析为 docker.io/otel/...
2. 不直连 docker.io，依次尝试镜像站：
   → docker.1ms.run/v2/otel/...
   → docker.m.daocloud.io/v2/otel/...
   → docker.nju.edu.cn/v2/otel/...
3. 全部失败才回退直连 docker.io
```

**验证**：`podman info | grep -A 15 "registries:"`

### containers.conf

**路径**：`%APPDATA%\containers\containers.conf`

```toml
[containers]
env = [
  "http_proxy=http://127.0.0.1:7890",
  "https_proxy=http://127.0.0.1:7890",
  "no_proxy=localhost,127.0.0.1,docker.1ms.run,docker.m.daocloud.io,docker.nju.edu.cn"
]

[engine]
env = [
  "http_proxy=http://127.0.0.1:7890",
  "https_proxy=http://127.0.0.1:7890",
  "no_proxy=localhost,127.0.0.1,docker.1ms.run,docker.m.daocloud.io,docker.nju.edu.cn"
]

[machine]

[network]

[secrets]

[configmaps]
```

**字段说明**：

| 字段 | 作用 |
|---|---|
| `[containers].env` | 注入到每个容器内的环境变量 |
| `[engine].env` | Podman engine 自身使用的环境变量（pull/push 时） |
| `no_proxy` | 国内镜像站直连，不走代理 |

**前提**：需要 `.wslconfig` 启用 `networkingMode=mirrored`，否则 WSL 内 `127.0.0.1` 无法访问 Windows 的 Clash 代理。

修改后需重启 engine：`podman machine stop && podman machine start`

### 配置生效机制

```
Windows 写入                              WSL 读取
─────────────────────────────────────────────────────────
%APPDATA%\containers\containers.conf  ──bind mount──>  /etc/containers/containers.conf
%APPDATA%\containers\registries.conf  ──bind mount──>  /etc/containers/registries.conf
%APPDATA%\containers\registries.conf.d\*  ──────────>  /etc/containers/registries.conf.d\*

~\.wslconfig                          ──WSL2 启动时──>  WSL 网络配置
```

**不需要重启 podman machine**：修改 `%APPDATA%\containers\` 下的文件后，WSL 内立即可见（bind mount 实时同步）。

**需要重启 podman machine 的场景**：
- 修改 `containers.conf` 中的 `[engine]` 部分
- 修改 `.wslconfig`（需 `wsl --shutdown` + `podman machine start`）

---

## 架构概览

```
┌─────────────────────────────────────────────────────────┐
│  Windows Host                                           │
│                                                         │
│  podman.exe (CLI)  ──named pipe──>  WSL2 podman service │
│                                                         │
│  %APPDATA%\containers\  ──bind mount──>  /etc/containers│
│  ~/.wslconfig                          (WSL2 mirrored) │
└─────────────────────────────────────────────────────────┘
```

Podman machine 使用 WSL2 作为虚拟化后端。Windows 侧的 `podman` 客户端通过 named pipe 与 WSL 内的 `podman-system-service` 通信。

---

## 常用操作

### 查看当前配置

```powershell
# 查看 registry 配置
podman info 2>$null | Select-String -Pattern "registries:" -Context 0,15

# 查看 machine 配置
podman machine inspect

# 进入 WSL 查看
wsl -d podman-machine-default
```

### 修改镜像源

```powershell
# 编辑（立即生效）
notepad "$env:APPDATA\containers\registries.conf"

# 或使用 drop-in（推荐新增镜像站时使用）
# 在 %APPDATA%\containers\registries.conf.d\ 下创建新文件
```

### 修改代理

```powershell
# 编辑 containers.conf
notepad "$env:APPDATA\containers\containers.conf"

# 重启 engine 使生效
podman machine stop && podman machine start
```

### 重建 machine 后恢复配置

```powershell
# 1. 确认 %APPDATA%\containers\ 下的配置还在（bind mount 源不会丢）
dir "$env:APPDATA\containers\"

# 2. 验证 WSL 内配置
podman info 2>$null | Select-String -Pattern "registries:" -Context 0,15
```

---

## Docker Hub 国内镜像源

### 可用镜像站

| 镜像站 | 地址 | 类型 | 说明 |
|---|---|---|---|
| 毫秒镜像 | `docker.1ms.run` | 境内 CDN | 推荐，速度快，支持多源（GCR/K8s 付费） |
| DaoCloud | `docker.m.daocloud.io` | 境内 CDN | 老牌服务，部分地区不稳定 |
| 南京大学 | `docker.nju.edu.cn` | 高校镜像 | 适合学术项目 |
| 腾讯云 | `mirror.ccs.tencentyun.com` | 境内 CDN | 仅腾讯云服务器内部可用 |
| 华为云 | `hub-mirror.c.myhuaweicloud.com` | 境内 CDN | 需华为云账号 |
| 网易 | `hub-mirror.163.com` | 境内 CDN | 较少更新 |
| 中科大 | `docker.mirrors.ustc.edu.cn` | 高校镜像 | 较少更新 |

> 公共加速源近年大量失效，建议至少配置 2-3 个做 fallback。

### 方式一：Podman registries.conf（推荐）

编辑 `%APPDATA%\containers\registries.conf`，Podman 原生支持，pull 时自动按顺序尝试：

```toml
unqualified-search-registries = ["docker.io"]

[[registry]]
location = "docker.io"
[[registry.mirror]]
location = "docker.1ms.run"
[[registry.mirror]]
location = "docker.m.daocloud.io"
[[registry.mirror]]
location = "docker.nju.edu.cn"
```

```bash
# 拉取命令不变，Podman 自动走镜像站
podman pull nginx
podman pull redis:7
```

### 方式二：镜像前缀（直接替换域名）

无需改配置，拉取时替换 registry 域名：

```bash
# Docker Hub
podman pull docker.1ms.run/library/nginx
podman pull docker.1ms.run/library/redis:7

# GHCR（免费）
podman pull ghcr.1ms.run/linuxserver/webtop:latest
```

### 两种方式对比

| | registries.conf | 镜像前缀 |
|---|---|---|
| 配置 | 改 `registries.conf` | 改拉取命令 |
| 透明性 | 对用户透明，原命令即可 | 需手动改命令 |
| 失败回退 | 自动尝试下一个镜像站 | 无，失败即失败 |
| Compose 兼容 | 自动生效 | 需改 `image:` 字段 |

> **实践建议**：日常使用 registries.conf；遇到特定镜像站拉取失败时临时用镜像前缀切换。

---

## 注意事项

1. **两个目录不要混用**
   - `%APPDATA%\containers\` — Podman machine 读取（bind mount）
   - `~\.config\containers\` — Podman CLI 客户端 + machine 元数据
   - 不要在 `~\.config\containers\` 下放 `containers.conf` 或 `registries.conf`，podman machine 不读

2. **代理地址**
   - mirrored 模式下用 `127.0.0.1:7890`（WSL 共享 Windows 网络栈）
   - 非 mirrored 模式下需用 WSL 网关 IP（`ip route show default | awk '{print $3}'`）

3. **镜像站故障自动切换**
   - Podman 按 `registries.conf` 中 mirror 列表顺序尝试
   - 一个失败自动跳到下一个，无需手动干预

4. **no_proxy 必须排除国内镜像站**
   - 镜像站是国内 CDN，走代理反而变慢或失败

5. **Podman Desktop 搜索不走镜像源（所有平台）**
   - Podman Desktop 的镜像搜索功能硬编码了 `index.docker.io`，不读取 `registries.conf`，所有平台（Linux/macOS/Windows）行为一致
   - `podman search` CLI 同样不走镜像源，镜像源仅对 `pull` 生效
   - 这是 [已知限制](https://github.com/podman-desktop/podman-desktop/issues/13915)，墙内搜索会超时
   - **解决方法**：忽略搜索超时，直接输入完整镜像名进行 pull（如 `docker.io/library/nginx`）
