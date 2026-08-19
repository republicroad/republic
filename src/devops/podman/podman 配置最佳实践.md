# Podman 配置最佳实践

本文档聚焦 Podman 代理配置，详细说明 `[engine].env` 和 `[containers].env` 的区别与使用场景。

---

## 配置文件概览

```
containers.conf          ← 主配置（代理、引擎行为）
registries.conf          ← 镜像源配置
```

---

## 代理配置详解

Podman 的代理配置分为两个维度，分别作用于不同阶段：

### `[engine].env` — Podman 引擎进程

**作用对象**：Podman 守护进程自身

**生效场景**：
- `podman pull` 拉取镜像
- `podman push` 推送镜像
- `podman build` 拉取基础镜像（`FROM` 指令）
- `podman search` 搜索镜像仓库
- 所有需要访问外部 registry 的引擎操作

**本质**：Podman 引擎进程的环境变量，影响所有由引擎发起的网络请求。

```toml
[engine]
env = [
  "http_proxy=http://127.0.0.1:7890",
  "https_proxy=http://127.0.0.1:7890",
  "no_proxy=localhost,127.0.0.1,docker.1ms.run,docker.m.daocloud.io,docker.nju.edu.cn"
]
```

### `[containers].env` — 容器进程

**作用对象**：每个容器内部的进程

**生效场景**：
- `podman run` 启动的容器
- `podman compose up` 启动的服务容器
- `podman build` 的 `RUN` 指令（构建阶段执行的命令）
- `podman exec` 进入容器后执行的命令

**本质**：注入到容器环境变量中的值，容器内任何进程都能读取。

> **重要**：在 rootless podman + WSL2 mirrored 网络下，容器已共享 Windows 网络栈，**无需在容器内配置代理**。容器可直连 GitHub、国内镜像站等。仅需 `[engine].env` 配置代理供引擎使用。

### `[containers].http_proxy` — 阻止代理变量传入容器

`[engine].env` 设置的代理变量会被容器继承。要阻止此行为，需在 `[containers]` 中设置：

```toml
[containers]
http_proxy = false  # 禁止将宿主机代理变量传入容器
```

**效果**：容器内无 `http_proxy`、`https_proxy`、`no_proxy` 等变量。

### ⚠️ 已知问题：`podman build` 不支持 `http_proxy = false`

**影响版本**：Podman 6.0.x

**表现**：
- `podman run` 正常：`http_proxy = false` 生效，容器内无代理变量 ✅
- `podman build` 异常：`RUN` 指令仍继承宿主机代理变量 ❌

**相关 Issue**：
- [#29299](https://github.com/podman-container-tools/podman/issues/29299) — `podman build in 6.0.x no longer honors env and http_proxy in containers.conf`
- [#24838](https://github.com/podman-container-tools/podman/issues/24838) — `podman passes down http_proxy when it shouldn't`

**临时解决方案**：

1. **使用 `--http-proxy=false` 命令行参数**（推荐）

   ```bash
   podman build --http-proxy=false -t myimage .
   ```

   在 compose 中通过 `x-podman` 扩展（待 podman-compose 支持）。

2. **在 Dockerfile 中显式清除**

   ```dockerfile
   RUN unset http_proxy https_proxy no_proxy
   ```

3. **在 Dockerfile 中覆盖为空值**

   ```dockerfile
   ENV http_proxy=""
   ENV https_proxy=""
   ENV no_proxy=""
   ```

**验证方法**：

```bash
# 构建后检查镜像中的代理变量
podman run --rm myimage printenv | grep -i proxy
# 预期：无输出
```

> **注意**：`podman-compose` 同样不支持 `--http-proxy` 参数（[issue #782](https://github.com/containers/podman-compose/issues/782)），compose build 无法通过此方式禁用代理。建议使用 Dockerfile 方案。

### 两者对比

| | `[engine].env` | `[containers].env` |
|---|---|---|
| **作用对象** | Podman 引擎进程 | 容器内进程 |
| **影响范围** | 引擎的所有网络操作 | 每个容器的环境变量 |
| **典型场景** | `podman pull nginx` | `RUN curl github.com` |
| **Build 阶段** | 拉取 `FROM` 基础镜像 | 执行 `RUN` 命令 |
| **WSL2 下是否必须** | 墙内 pull 必须 | **不需要**（容器直连外网） |

### `podman build` 的代理流程

```
podman compose build
│
├─ 阶段 1: 拉取基础镜像 (FROM node:20)
│  └─ 使用 [engine].env 的代理设置
│
└─ 阶段 2: 执行 Dockerfile RUN 命令
   └─ 容器直连外网（无需代理）
      └─ RUN git clone github.com/xxx  ← 直连
      └─ RUN curl https://api.xxx.com  ← 直连
```

### `no_proxy` 的作用

`no_proxy` 排除不需要走代理的地址。对于国内镜像站，直连比走代理更快：

```toml
no_proxy = "localhost,127.0.0.1,docker.1ms.run,docker.m.daocloud.io,docker.nju.edu.cn"
```

`[engine].env` 中配置 `no_proxy`：拉取镜像时，镜像站直连，不走代理。

---

## 镜像源配置

参考同目录下 `registries.conf` 文件，配置 Docker Hub 国内镜像加速。

---

## Windows + WSL2 特殊说明

### mirrored 网络模式

Windows 上 Podman 使用 WSL2 后端，需在 `~\.wslconfig` 启用 mirrored 网络：

```ini
[wsl2]
networkingMode=mirrored
```

启用后 WSL 共享 Windows 网络栈，容器可直连外网（GitHub、国内镜像站等）。

### 配置文件路径

```
Windows 侧（编辑位置）:
  %APPDATA%\containers\containers.conf
  %APPDATA%\containers\registries.conf

WSL 侧（Podman 实际读取）:
  /etc/containers/containers.conf
  /etc/containers/registries.conf
```

修改 Windows 侧配置后，需确认 WSL 侧同步生效。

### Rootless Podman + WSL2 网络限制

Rootless podman 容器有独立的网络命名空间，以下地址**从容器内不可达**：

| 目标地址 | 可达性 | 原因 |
|---|---|---|
| `127.0.0.1` | ❌ | 容器自己的 loopback，非宿主机 |
| `host.containers.internal` (169.254.1.2) | ❌ | Podman 虚拟地址，WSL2 下不可达 |
| WSL 主机 IP (如 10.x.x.x) | ❌ | rootless 网络隔离 |
| Podman 网桥 (10.88.0.1) | ❌ | rootless 网络隔离 |

**但容器可直连外网**（mirrored 网络模式下）：

| 目标 | 可达性 | 说明 |
|---|---|---|
| GitHub | ✅ | 直连，无需代理 |
| 国内镜像站 | ✅ | 直连，无需代理 |
| 通用 HTTP/HTTPS | ✅ | 直连，无需代理 |
| Docker Hub (registry-1.docker.io) | ❌ | 被墙，需走镜像源 |

**结论**：
- `[containers].env` **不需要**配置代理（容器直连外网）
- `[engine].env` **需要**配置代理（`podman pull` 拉取基础镜像时引擎进程需要）
- 如需容器访问 `127.0.0.1` 上的服务（如 Windows Clash），必须使用 `--network host`

---

## 常见问题

### Q: 容器内无法访问外网

**原因**：WSL2 未启用 mirrored 网络模式

**检查**：`.wslconfig` 是否配置 `networkingMode=mirrored`

**解决**：添加配置后重启 WSL：`wsl --shutdown && podman machine start`

### Q: `podman pull` 超时

**检查**：引擎是否有代理环境变量

```bash
podman info | grep -i proxy
```

**解决**：确认 `[engine].env` 已配置代理

### Q: Build 时 `RUN` 命令无法访问外网

**原因**：WSL2 mirrored 网络未启用，或目标站点被墙

**解决**：
1. 确认 `.wslconfig` 已启用 `networkingMode=mirrored`
2. 如目标是 Docker Hub，需配置镜像源（`registries.conf`）

### Q: 容器需要访问 Windows 上的 `127.0.0.1` 服务

**原因**：Rootless podman 容器网络隔离，`127.0.0.1` 是容器自己的 loopback

**解决**：使用 `--network host` 共享宿主机网络

```bash
podman run --network host ...
```

### Q: `podman build` 的 `RUN` 仍继承宿主机代理

**原因**：Podman 6.0.x 的 [已知 bug](https://github.com/podman-container-tools/podman/issues/29299)，`[containers].http_proxy = false` 在 build 阶段不生效

**表现**：容器内 `printenv | grep -i proxy` 能看到 `http_proxy` 变量

**解决**：
1. 使用 `--http-proxy=false` CLI 参数（推荐）
2. 在 Dockerfile 中添加 `ENV http_proxy=""`
3. 等待上游修复（[#29299](https://github.com/podman-container-tools/podman/issues/29299)）

**注意**：`podman-compose` 不支持 `--http-proxy` 参数，需用 Dockerfile 方案。

---

## 完整配置示例

```toml
[containers]
http_proxy = false  # 阻止 [engine].env 的代理变量传入容器（仅对 podman run 生效，podman build 有 bug）

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

> **Build 阶段注意**：由于 Podman 6.0.x 的 [bug](https://github.com/podman-container-tools/podman/issues/29299)，`podman build` 的 `RUN` 指令仍会继承 `[engine].env` 中的代理变量。如需禁止，请在 Dockerfile 中添加 `ENV http_proxy=""` 或使用 `--http-proxy=false` CLI 参数。

参考同目录下 `containers.conf` 文件。
