# Podman Windows WSL2 网络配置最佳实践

## 目录

- [环境概述](#环境概述)
- [.wslconfig 配置](#wslconfig-配置)
- [Podman Machine 网络模式](#podman-machine-网络模式)
- [代理配置](#代理配置)
- [容器访问宿主机](#容器访问宿主机)
- [自动启动脚本](#自动启动脚本)
- [常见问题排查](#常见问题排查)

---

## 环境概述

| 组件 | 版本/说明 |
|---|---|
| Windows | 10/11 |
| WSL | WSL2 (NAT 模式) |
| Podman | 5.x+ |
| 代理软件 | Clash Verge Rev (mihomo) |

---

## .wslconfig 配置

路径：`C:\Users\<用户名>\.wslconfig`

```ini
[wsl2]
networkingMode=nat
dnsTunneling=true
autoProxy=true
firewall=true

[experimental]
autoMemoryReclaim=gradual
```

### 配置说明

| 配置项 | 值 | 说明 |
|---|---|---|
| `networkingMode` | `nat` | **必须使用 NAT 模式**，mirrored 模式与 Podman 不兼容 |
| `dnsTunneling` | `true` | VPN 环境下 DNS 正常解析 |
| `autoProxy` | `true` | 自动传递宿主机系统代理到 WSL2（仅代理绑定 `0.0.0.0` 时生效） |
| `firewall` | `true` | 启用 Windows 防火墙，需配合防火墙规则使用 |
| `autoMemoryReclaim` | `gradual` | 渐进回收 WSL2 内存，减少占用 |

### 为什么不能用 mirrored 模式

| 问题 | 说明 |
|---|---|
| 容器端口 loopback 不可达 | `nc -w5 127.0.0.1 8080` 失败 |
| iptables MASQUERADE 被覆盖 | WSL2 的 `WSLOUTPUT` 链覆写 Podman 的包标记 |
| `host.containers.internal` 解析异常 | DNS 解析指向错误 IP |

修改 `.wslconfig` 后需重启 WSL：

```powershell
wsl --shutdown
```

---

## Podman Machine 网络模式

### 获取宿主机 IP

```bash
# WSL2 NAT 模式下宿主机网关 IP
ip route show default | awk '{print $3}'
# 例如：172.20.16.1
```

### 容器内 /etc/hosts

Podman 启动容器时，会根据 `containers.conf` 的 `host_containers_internal_ip` 自动写入：

```
172.20.16.1 host.containers.internal
172.20.16.1 host.docker.internal
```

---

## 代理配置

### 代理软件要求

代理必须绑定到 `0.0.0.0` 才能被 WSL2 访问。

| 代理软件 | 默认绑定 | 需要修改 |
|---|---|---|
| Clash Verge Rev | `0.0.0.0`（`allow-lan: true`） | ✅ 无需修改 |
| Clash | `127.0.0.1` | ❌ 需改为 `0.0.0.0` |
| V2Ray | `127.0.0.1` | ❌ 需改为 `0.0.0.1` |

#### Clash Verge Rev 配置验证

文件路径：`%APPDATA%\io.github.clash-verge-rev.clash-verge-rev\config.yaml`

```yaml
mixed-port: 7890
allow-lan: true  # 关键：绑定所有网卡
```

### WSL2 内测试代理连通性

```bash
# 获取宿主机 IP
HOST_IP=$(ip route show default | awk '{print $3}')

# 测试 HTTP 代理
curl -x http://$HOST_IP:7890 http://httpbin.org/ip --max-time 5 -s

# 测试 HTTPS 代理
curl -x http://$HOST_IP:7890 https://www.google.com --max-time 5 -s -o /dev/null -w "%{http_code}"
```

预期返回：

```json
{
  "origin": "你的公网IP"
}
```

## Podman 配置文件位置

### 配置文件加载顺序（Podman 6）

Podman 6 采用新的配置解析机制，主配置文件**仅加载最高优先级的一个**，不再合并所有路径。

#### 主配置文件（从低到高优先级）

```
1. /usr/share/containers/containers.conf          # 发行版默认
2. /etc/containers/containers.conf                # 系统级
3. $XDG_CONFIG_HOME/containers/containers.conf    # 用户级（最高）
```

> `$XDG_CONFIG_HOME` 未设置时回退到 `$HOME/.config`

#### Drop-in 目录（Podman 6 新增）

```
/usr/share/containers/containers.conf.d/
/etc/containers/containers.conf.d/
$XDG_CONFIG_HOME/containers/containers.conf.d/
```

Drop-in 文件按文件名字母顺序加载，后面的覆盖前面的。

#### Windows 路径映射

| Linux 路径 | Windows 路径 |
|---|---|
| `$XDG_CONFIG_HOME/containers/` | `%USERPROFILE%\.config\containers\` |
| `/etc/containers/` | WSL2 内部 |

#### Podman 5 vs 6 对比

| 特性 | Podman 5 | Podman 6 |
|---|---|---|
| 主配置文件 | 合并所有路径 | 仅加载最高优先级的一个 |
| Drop-in 支持 | ❌ | ✅ |
| 同名文件处理 | 合并 | 后者覆盖前者 |

### containers.conf 配置

路径：`C:\Users\<用户名>\.config\containers\containers.conf`（用户级，最高优先级）

```toml
[containers]
http_proxy = true
host_containers_internal_ip = "172.20.16.1"
env = [
  "HTTP_PROXY=http://172.20.16.1:7890",
  "HTTPS_PROXY=http://172.20.16.1:7890",
  "NO_PROXY=localhost,127.0.0.1"
]

[engine]
env = [
  "HTTP_PROXY=http://172.20.16.1:7890",
  "HTTPS_PROXY=http://172.20.16.1:7890",
  "NO_PROXY=localhost,127.0.0.1"
]
```

> **注意**：配置文件位于 Windows 侧，Podman Machine 重启后仍有效。WSL2 侧的 `~/.config/containers/containers.conf` 会在 Machine 重建时丢失。

### 配置说明

| 配置项 | 作用 |
|---|---|
| `http_proxy = true` | 自动将主机代理环境变量传入容器 |
| `host_containers_internal_ip` | 容器内 `host.containers.internal` 解析到此 IP |
| `[containers].env` | 所有容器的默认环境变量 |
| `[engine].env` | Podman 引擎（拉取镜像等）的环境变量 |

### [engine] 和 [containers] 独立生效

两个配置段作用范围不同，**必须同时配置**才能覆盖 build 和 run：

| 命令 | 代理来源 | 配置段 |
|---|---|---|
| `podman pull` | `[engine]` → `env` | ✅ |
| `podman push` | `[engine]` → `env` | ✅ |
| `podman build` | `[engine]` → `env` | ✅ |
| `podman search` | `[engine]` → `env` | ✅ |
| `podman run` | `[containers]` → `env` | ✅ |
| `podman create` | `[containers]` → `env` | ✅ |

```toml
# 缺少 [engine] → podman pull/build 无法使用代理
# 缺少 [containers] → podman run 容器内无代理变量
[containers]
env = ["HTTP_PROXY=...", "HTTPS_PROXY=..."]  # 仅对 run/create 生效

[engine]
env = ["HTTP_PROXY=...", "HTTPS_PROXY=..."]  # 仅对 pull/push/build/search 生效
```

> **常见问题**：
> - 只配置 `[containers]` 段 → `podman pull` 和 `podman build` 无法访问外网
> - 只配置 `[engine]` 段 → `podman run` 容器内无代理变量

### 禁止代理泄漏到容器

如果容器不需要代理：

```bash
podman run --http-proxy=false myimage
```

或在 `containers.conf` 中全局禁用：

```toml
[containers]
http_proxy = false
```

---

## 容器访问宿主机

### 方式对比

| 方式 | 说明 | 可靠性 |
|---|---|---|
| `host.containers.internal` | Podman 自动管理，需配置 `host_containers_internal_ip` | ⭐⭐⭐ |
| `host.docker.internal` | Docker 兼容，Podman 同样支持 | ⭐⭐⭐ |
| `--add-host host-gateway:xxx` | 运行时指定 | ⭐⭐ |
| WSL 网关 IP（如 `172.20.16.1`） | 直接使用 IP | ⭐（IP 可能变化） |

### 容器内测试

```bash
# 启动测试容器
podman run --rm curlimages/curl curl -s http://host.docker.internal:7890

# 测试宿主机 API
podman run --rm curlimages/curl curl -s http://host.containers.internal:3000/api
```

---

## 防火墙配置

### 添加 WSL2 网段规则

管理员 PowerShell 执行：

```powershell
# 允许 WSL2 NAT 网段访问 Clash 代理端口
netsh advfirewall firewall add rule name="Clash for WSL2" dir=in action=allow protocol=TCP localport=7890 remoteip=172.20.0.0/16
```

### 规则管理

```powershell
# 查看规则
netsh advfirewall firewall show rule name="Clash for WSL2"

# 删除规则
netsh advfirewall firewall delete rule name="Clash for WSL2"
```

### 注意事项

| 要点 | 说明 |
|---|---|
| `networkingMode=nat` | WSL2 使用 `172.x.x.x` 网段 |
| `networkingMode=mirrored` | WSL2 与宿主机共享 `192.168.x.x` 网段 |
| 网段变化 | WSL2 重启后网段可能变化，规则使用 CIDR 范围 |
| 多端口 | 每个端口需单独添加规则 |

---

## 自动启动脚本

### 脚本路径

```
C:\Users\<用户名>\Documents\workspace\republic\src\blog\Update-Podman-Host.ps1
```

### 功能

1. 检测 Podman Machine 状态，未运行则启动
2. 获取 WSL2 宿主机网关 IP
3. 更新 `/etc/hosts`（`host.docker.internal` + `host.containers.internal`）
4. 更新 `containers.conf`（`host_containers_internal_ip`）

### 开机自启

快捷方式位置：

```
C:\Users\<用户名>\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\Update-Podman-Host.lnk
```

### 手动执行

```powershell
powershell -ExecutionPolicy Bypass -File "C:\Users\<用户名>\Documents\workspace\republic\src\blog\Update-Podman-Host.ps1"
```

---

## 常见问题排查

### 问题 1：WSL2 无法访问代理

| 检查项 | 命令 |
|---|---|
| 代理是否绑定 0.0.0.0 | `netstat -an \| findstr "7890"` |
| 防火墙是否放行 | `netsh advfirewall firewall show rule name="Clash for WSL2"` |
| 宿主机 IP 是否正确 | `wsl -- ip route show default` |
| 端口是否可达 | `wsl -- curl -x http://<IP>:7890 http://httpbin.org/ip --max-time 5` |

### 问题 2：host.containers.internal 解析错误

```bash
# 检查容器内 /etc/hosts
podman run --rm alpine cat /etc/hosts

# 手动指定
podman run --rm --add-host host.containers.internal:172.20.16.1 alpine ping host.containers.internal
```

### 问题 3：容器内无法访问宿主机服务

```bash
# 检查宿主机服务是否监听所有网卡
netstat -an | findstr "LISTENING" | findstr "0.0.0.0"

# 检查防火墙
netsh advfirewall firewall show rule name=all dir=in | findstr "8080"
```

### 问题 4：代理连接超时

```bash
# 检查 DNS 解析
nslookup httpbin.org

# 直接测试连通性
ping 172.20.16.1

# 检查代理日志（Clash Verge Rev）
# 设置 → 日志
```

### 问题 5：VPN 环境 DNS 失效

确保 `.wslconfig` 中 `dnsTunneling=true`：

```ini
[wsl2]
dnsTunneling=true
```

---

## 配置检查清单

- [ ] `.wslconfig` 使用 `networkingMode=nat`
- [ ] `.wslconfig` 设置 `dnsTunneling=true`
- [ ] 代理软件绑定 `0.0.0.0`（`allow-lan: true`）
- [ ] Windows 防火墙添加 WSL2 网段规则
- [ ] `containers.conf` 配置 `host_containers_internal_ip`（Windows 侧）
- [ ] `containers.conf` 配置代理环境变量（Windows 侧）
- [ ] 启动脚本放置到 Startup 文件夹
- [ ] 测试容器内可访问宿主机服务

---

## 参考链接

### Podman 官方文档

- [Podman - Restricted Environments (Proxy)](https://podman-desktop.io/docs/proxy) - Podman Desktop 代理配置
- [containers.conf(5) - Podman](https://github.com/containers/container-libs/blob/main/common/docs/containers.conf.5.md) - containers.conf 官方文档
- [containers-config(5) - Podman 6](https://github.com/podman-container-tools/container-libs/blob/main/common/docs/containers-config.5.md) - **Podman 6 配置加载顺序**
- [containers.conf(5) - Arch Linux](https://man.archlinux.org/man/containers.conf.5.en) - containers.conf man page
- [podman-run(1)](https://docs.podman.io/en/stable/markdown/podman-run.1.html) - podman run 命令文档
- [podman-build(1)](https://docs.podman.io/en/stable/markdown/podman-build.1.html) - podman build 命令文档
- [podman-create(1)](https://docs.podman.io/en/stable/markdown/podman-create.1.html) - podman create 命令文档
- [podman(1) - Environment Variables](https://docs.podman.io/en/stable/markdown/podman.1.html) - 环境变量配置

### Podman 6 设计文档

- [Podman 6 配置解析设计文档](https://github.com/podman-container-tools/podman/blob/main/contrib/design-docs/config-file-parsing.md) - Podman 6 新配置解析机制

### 相关 Issues

- [Podman Issue #28351 - host.containers.internal](https://github.com/containers/podman/issues/28351) - WSL2 下 host.containers.internal 解析问题
- [Podman Issue #28477 - host.docker.internal](https://github.com/containers/podman/issues/28477) - WSL2 下 host.docker.internal 连接问题
- [WSL2 Issue #13868 - mirrored 模式问题](https://github.com/microsoft/wsl/issues/13868) - WSL2 mirrored 模式与容器网络不兼容
