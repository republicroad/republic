# podman

## 最佳实践

### ubuntu
podman 在 linux 上的最佳实践
[Installing podman 5+ on ubuntu older versions](https://github.com/containers/podman/discussions/25582#discussioncomment-12803424)  

## install

### ubuntu
```bash
sudo apt install podman
```

podman 是一个 rootless 的容器管理系统，基本功能和 docker 类似. 
相对比docker的优势 [Podman vs Docker 2026: Security, Performance & Which to Choose](https://last9.io/blog/podman-vs-docker/)：

1. 默认提供 rootless 且无守护进程(daemon), 安全性好
2. 提供 pod 的概念，更容易和k8s之类的容器编排调度系统集成
3. 完全开源, 提供了兼容 docker 的标准的容器格式规范

### win11

[podman-windows](podman/podman-windows.md)
### macos
在macOS 上 使用Homebrew 安装qemu(这是 Podman 在 macOS 上运行所必需的虚拟化工具。)
```bash
brew install qemu  
```

在 macOS 上使用 Homebrew 安装 Podman  
```bash
brew install podman  
```
  
初始化一个新的 Podman 虚拟机（由于 Podman 运行在虚拟机中，因此在 macOS 上需要此步骤） 
```bash
podman machine init  
```

启动 Podman 虚拟机  
```bash
podman machine start
```

命令验证安装是否成功
```bash
podman info
```

‌验证连接
```bash
podman system connection list
```

## image

> podman pull redis

## container

> podman pull postgres:16

创建容器

```bash
podman run --name pg16 -e POSTGRES_USER=postgres -e POSTGRES_PASSWORD=fccdjny -p 5432:5432 -d postgres
```


创建容器容器指定网络

```bash
podman run -d --name my-container --network my-network nginx
```

创建网络请参考: [创建网络](podman.md#创建网络)
## exec

非交互式运行:  

> podman exec pg16 date

交互式运行:  
进入一个正在运行的容器中的shell，用于调试  

> podman exec -it pg16 bash

-i 表示交互式操作
-t 表示使用一个 terminal 终端

操作此pg16容器中的psql客户端
> podman exec -it pg16 psql -U postgres

## attach

把一个容器以交互式后台运行后，容器正在运行:

```bash
podman run -itd --name a1 alpine:latest sh
podman run -itd --name n1 nginx bash
```

可以通过 `podman ps` 看到这个正在后台运行交互式shell的容器，可以通过 attach 命令重新去控制这个shell:
```bash
ryefccd@republic:~$ podman attach a1
/ # cat /etc/issue 
Welcome to Alpine Linux 3.23
Kernel \r on \m (\l)

/ # 
```

注意，在attach的shell中如果执行 `exit` 那么容器便退出了。如果只是希望退出当前的shell控制，而不是让容器退出，不要输入 `exit`，要输入  ==**Ctrl-P  Ctrl-Q**== 来退出attach的容器.

## network

### 查看podman的网络配置
[Basic Networking Guide for Podman](https://github.com/containers/podman/blob/main/docs/tutorials/basic_networking.md#default-network)    

> podman network ls

```bash
ryefccd@republic:~$ podman network ls
NETWORK ID    NAME        DRIVER
2f259bab93aa  podman      bridge
```


查看 podman 网络设置中名为 podman 的网桥配置
> podman network inspect podman 

```bash
ryefccd@republic:~$ podman network inspect podman 
[
     {
          "name": "podman",
          "id": "2f259bab93aaaaa2542ba43ef33eb990d0999ee1b9924b557b7be53c0b7a1bb9",
          "driver": "bridge",
          "network_interface": "podman0",
          "created": "2026-01-06T14:29:56.005594914+08:00",
          "subnets": [
               {
                    "subnet": "10.88.0.0/16",
                    "gateway": "10.88.0.1"
               }
          ],
          "ipv6_enabled": false,
          "internal": false,
          "dns_enabled": false,
          "ipam_options": {
               "driver": "host-local"
          }
     }
]
```


如文件所示: **dns_enabled : false** 表示未开启dns解析.
所以不能通过容器名字去在网络访问容器. 如果需要开启容器名字的dns解析，请参考下一节.


### 通过容器名字作为域名解析


[podman 默认不支持容器名字作为dns的解析](https://github.com/containers/podman/blob/main/docs/tutorials/basic_networking.md#default-network:~:text=It%20does%20not%20support%20dns%20resolution%20because%20of%20backwards%20compatibility%20with%20Docker)


查看 podman 默认的网络配置, 这个配置默认是保存在内存中的.

```
podman network inspect podman | jq .[] > ~/.local/share/containers/storage/networks/podman.json
```

如果需要修改, 需要把此配置导出 `~/.local/share/containers/storage/networks/podman.json` 文件, 修改其中的**dns_enabled**的配置开启，重启容器即可通过容器名字访问(ping).
```json
{
...
	"dns_enabled": true,
...
}
```


> [!NOTE] alpine 镜像带有ping命令
> 准备一个带有 ping 命令的容器  
> >  podman pull alpine
> >  podman run -d  --network podman --name a1 alpine sleep infinity

> [!WARNING] ubuntu容器设置ICMP数据包权限
> ubnutu 容器如果需要 ping 命令，需要单独设置 ICMP 数据包的权限  
> > podman pull ubuntu
> > podman run -d    --cap-add=NET_RAW --name ub ubuntu sleep infinity
> > podman run -itd --cap-add=NET_RAW --name ub ubuntu bash



> podman run -d --network podman --name a1 alpine sleep infinity
> podman run -d --network podman --name a2 alpine sleep infinity
> podman run -d  --name a3 alpine sleep infinity

> podman inspect -f '{{.NetworkSettings.IPAddress}}' a1
> podman inspect -f '{{.NetworkSettings.IPAddress}}' a2
> podman inspect -f '{{.NetworkSettings.IPAddress}}' a3

可以看到 a1 和  a2 容器都分配了ip. a3 没有分配ip, 这是因为podman 创建容器需要显示的传递网络配置才能分配ip和容器名作为域名访问.

```bash
ryefccd@republic:~$ podman inspect -f '{{.NetworkSettings.IPAddress}}' a1
10.88.0.2
ryefccd@republic:~$ podman inspect -f '{{.NetworkSettings.IPAddress}}' a2
10.88.0.3
ryefccd@republic:~$ podman inspect -f '{{.NetworkSettings.IPAddress}}' a3


```

> podman exec a1 ping -c 3 a2

```bash
ryefccd@republic:~$ podman exec a1 ping -c 3 a2
PING a2 (10.88.0.11): 56 data bytes
64 bytes from 10.88.0.11: seq=0 ttl=42 time=0.049 ms
64 bytes from 10.88.0.11: seq=1 ttl=42 time=0.107 ms
64 bytes from 10.88.0.11: seq=2 ttl=42 time=0.116 ms

--- a2 ping statistics ---
3 packets transmitted, 3 packets received, 0% packet loss
round-trip min/avg/max = 0.049/0.090/0.116 ms

```

### 给容器一个固定ip

> podman run -d  --network podman --ip 10.88.0.6 --name a6 alpine sleep infinity
> podman run -d  --network podman --ip 10.88.0.7 --name a7 alpine sleep infinity


### 创建网络


**Subnet:** Specify a custom IP rangenet

```bash
podman network create --subnet 192.168.10.0/24 net192

podman network create --subnet 10.0.0.0/24 --ip-range 10.0.0.100-10.0.0.200 my-custom-net
```

**Gateway:** Define the gateway IP for the network

```bash
podman network create --gateway 192.168.10.1 custom-net
```

**Driver:** Choose a specific network driver, such as `macvlan` or `ipvlan` (rootful only)

```bash
podman network create -d macvlan -o parent=eth0 macvlan-net
```

**Internal:** Create a network that is isolated from the host and external internet

```bash
podman network create --internal private-net
```


注意, 新创建的网络默认开启了 dns_enabled: true 的配置

```bash
ryefccd@republic:~$ podman network inspect net192
[
     {
          "name": "net192",
          "id": "fdc405841b2ed10679ffe3eacf9e6780af8f852bd9cd27e11355ed3eb751383b",
          "driver": "bridge",
          "network_interface": "podman3",
          "created": "2026-01-07T15:29:51.040501876+08:00",
          "subnets": [
               {
                    "subnet": "192.168.10.0/24",
                    "gateway": "192.168.10.1"
               }
          ],
          "ipv6_enabled": false,
          "internal": false,
          "dns_enabled": true,
          "ipam_options": {
               "driver": "host-local"
          }
     }
]

```
用此网络设备的容器都可以通过容器名互相访问(ping)了.
## podman compose

### ubuntu

注意, 不要使用 `apt install podman-compose` 来安装, 这样不是最新版本. 使用如下步骤单独安装 podman-compose 最新的 standalone 版本:
```bash
curl -o ~/.local/bin/podman-compose https://raw.githubusercontent.com/containers/podman-compose/main/podman_compose.py
chmod +x ~/.local/bin/podman-compose
```

查看 podman 和 podman-compose的版本:
```bash
ryefccd@republic:~/brde$ podman compose --version
>>>> Executing external compose provider "/home/ryefccd/.local/bin/podman-compose". Please see podman-compose(1) for how to disable this message. <<<<

podman-compose version 1.5.0
podman version 5.4.1
```
确定 podman compose 的 provider  "~/.local/bin/podman-compose" 是来自我们上面手动安装即可。

### windows 

powershell 中执行下列命令:

```powershell
# windows 安装 podman-compose
pip install podman-compose
# docker-compose.exe 优先级高于 podman-compose, 所以需要删除 docker-compose.exe
Remove-Item "$(where.exe docker-compose.exe)" -Force
```

### macos

安装podman-compose 使得compose执行指令是用的podman的指令而不是docker的指令
```bash
brew install podman-compose
```

### 编排命令

podman-compose兼容 docker-compose, 用来做多容器编排管理.
默认文件是 **`compose.yaml`**, **`compose.yml`**, **`docker-compose.yaml`** or **`docker-compose.yml`**
```bash
podman compose up -d
```
如果是以其他文件命名, 可以使用 `-f` 来指定相关编排文件
```bash

podman compose -f my-alternative-name.yml up
```

只更新其中一个容器, 比如haproxy:

```bash
podman compose up -d --force-recreate haproxy
```

### 多仓库编排

https://docs.docker.com/reference/compose-file/build/#context

## 示例

### postgresql 容器

[**Running PostgreSQL with Podman**](https://medium.com/@mehmetozanguven/running-postgresql-with-podman-4b71e31761b2)

### atlasgo 数据库模式变更管理
```bash
apt install podman podman-docker
sudo apt install podman podman-docker
```

podman 是一个 rootless 的容器管理系统，基本功能和 docker 类似. 只是默认提供 rootless 的模式，安全性比较好. 
podman-docker 会在 $PATH 路径下设置一个 docker 的脚本命令，实际也是指向 podman 的执行文件. 这个库目的是兼容 docker 去适配一些开发工具行为。比如我们使用的 atlasgo 这个用于管理数据库表字段变更的工具.

>  atlas migrate diff --env sqlalchemy --dev-url "docker://postgres/16"

此处的 docker://postgres/16 会在命令运行时使用 docker 命令程序去运行 postgres:16 的镜像来做数据库模式变更迁移.



## 参考

### ubuntu podman 容器服务被其他主机访问(防火墙)

除了要使用 -p 将容器端口曝露到主机端口上，还需要在主机上配置防火墙，给服务端口放行.
比如为了给 haproxy 计数器服务的容器所在的主机曝露服务，需要运行以下脚本:
```bash
sudo ufw status          # 查看防火墙规则
sudo ufw allow 8888/tcp  # 将本机器8888端口曝露给网络上其他机器访问
```

[UFW: Uncomplicated Firewall — Cheat Sheet](https://blog.rtsp.us/ufw-uncomplicated-firewall-cheat-sheet-a9fe61933330)

### docker 镜像源

[境内 Docker 镜像状态监控](https://status.anye.xyz/)  
[毫秒镜像 - 专为中国开发者提供的专业容器仓库服务](https://1ms.run/)  

docker 的配置文件(/etc/docker/daemon.json)：

```/etc/docker/daemon.json
{
  "registry-mirrors": ["docker.1ms.run", "https://mirror.gcr.io",]
}
```

###  wsl2 proxy 设置

windows 中使用 podman 需要使用 wsl2 作为 podman machine.
在windows 中 C:\Users\${USER} 创建 .wslconfig 文件并写入以下配置识别系统代理
```ini
[experimental]
autoMemoryReclaim=gradual
networkingMode=mirrored
dnsTunneling=true
firewall=true
autoProxy=true
```


### `EXPOSE` vs. `PUBLISH` (`-p`)

It is crucial to understand that `EXPOSE` is different from publishing a port using the `-p` or `--publish` flag with the `docker run` command: 

- **`EXPOSE` (Dockerfile instruction):** Declares an internal container port. It is a build-time instruction that sets metadata.
- **`PUBLISH` (`-p` flag):** Maps a specific port on the host machine to a port inside the container at runtime, making the service accessible from the outside world. 

You can publish any container port using `-p`, even if it is not listed in the `EXPOSE` instruction in the Dockerfile.


### nginx in container

[What is the difference between nginx daemon on/off option?](https://stackoverflow.com/questions/25970711/what-is-the-difference-between-nginx-daemon-on-off-option)  

In Docker, `nginx -g 'daemon off;'` is the standard and recommended way to run NGINX in the **foreground**, which ensures that the Docker container remains running. 

Why `daemon off` is necessary in Docker

Docker operates on the principle that one container runs one foreground process (PID 1). 

- **Standard NGINX behavior:** By default, NGINX runs as a daemon (`daemon on`), which means the initial process quickly spawns worker processes in the background and then exits.
- **Docker's reaction:** If the primary process (PID 1) exits, Docker assumes the container's task is complete and stops the container immediately.
- **The solution:** Setting `daemon off;` tells NGINX to stay in the foreground, becoming the primary process that Docker monitors. As long as NGINX is running, the container stays alive. 

How to use `daemon off`

You can configure NGINX with `daemon off` in a few ways:

- **In a `Dockerfile`:** Modify your NGINX configuration file (`nginx.conf`) to include the directive, or pass it as a command-line argument in the `CMD` instruction. The official NGINX Docker images handle this automatically in their entrypoint scripts, but if you provide a custom `CMD` or `ENTRYPOINT`, you may need to explicitly include it.
    
    dockerfile
    
    ```
    # Example Dockerfile snippet
    CMD ["nginx", "-g", "daemon off;"]
    ```
    
- **In a `docker run` command:**
    
    bash
    
    ```
    docker run -d --name my-nginx -p 8080:80 nginx:latest nginx -g 'daemon off;'
    ```
    
- **In a `docker-compose.yaml` file:**
    
    yaml
    
    ```
    services:
      web:
        image: nginx:latest
        ports:
          - "8080:80"
        command: [nginx, '-g', 'daemon off;']
    ```

### windows podman compose

```powershell
# windows 安装 podman-compose
# echo "$(where.exe podman-compose)"
pip install podman-compose

# docker-compose.exe 优先级高于 podman-compose, 所以需要删除 docker-compose.exe
Remove-Item "$(where.exe docker-compose.exe)" -Force
# Remove-Item "$HOME\\AppData\\Local\\Microsoft\\WindowsApps\\docker-compose.exe" -Force
# Remove-Item "C:\\Users\\RYefccd\\AppData\\Local\\Microsoft\\WindowsApps\\docker-compose.exe" -Force
```