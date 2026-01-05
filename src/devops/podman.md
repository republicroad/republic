# podman

## install

```bash
apt install podman
sudo apt install podman
```

podman 是一个 rootless 的容器管理系统，基本功能和 docker 类似. 
相对比docker的优势 ：

1. 默认提供 rootless 且无守护进程(daemon), 安全性好
2. 提供 pod 的概念，更容易和k8s之类的容器编排调度系统集成
3. 完全开源, 提供了兼容 docker 的标准的容器格式规范

## image

> podman pull redis

直接拉取镜像会报错:

```
ryefccd@republic:~$ podman pull redis
Error: short-name "redis" did not resolve to an alias and no unqualified-search registries are defined in "/etc/containers/registries.conf"

```

解决方案有两个:
1. 指定某个可以使用的源

	```bash
	podman pull docker.1ms.run/library/postgres:16
	```

2. 因为国内的docker镜像源不可使用，所以需要更换国内可以使用的源: https://1ms.run/  具体配置参考下一节**镜像源**

## 镜像源

修改podman国内镜像源(兼容docker)

```bash
# 在此文件增加源的配置
sudo vim /etc/containers/registries.conf
```

```toml
# # An array of host[:port] registries to try when pulling an unqualified image, in order.
# unqualified-search-registries = ["example.com"]
unqualified-search-registries = ["docker.1ms.run"]
```

设置完源以后，就可以直接使用镜像名字下载了

> podman pull postgres:16
> podman pull postgres:latest

## 镜像别名(shortnames)

在 shortnames.conf 文件中还有一些别名设置，检测到某些常用的软件镜像直接就指定了查找源，因为国内的 docker 域名不能访问，所以也需要替换

```bash
~$ podman pull hello-world
Resolved "hello-world" as an alias (/etc/containers/registries.conf.d/shortnames.conf)
Trying to pull docker.io/library/hello-world:latest...

```

```/etc/containers/registries.conf.d/shortnames.conf
...

  # docker
  "alpine" = "docker.io/library/alpine"
  "docker" = "docker.io/library/docker"
  "registry" = "docker.io/library/registry"
  "hello-world" = "docker.io/library/hello-world"
...

# Ubuntu
  "ubuntu" = "docker.io/library/ubuntu"
  # Oracle Linux
  "oraclelinux" = "container-registry.oracle.com/os/oraclelinux"
  # busybox
  "busybox" = "docker.io/library/busybox"
  # php
  "php" = "docker.io/library/php"
  # python
  "python" = "docker.io/library/python"
  # node
  "node" = "docker.io/library/node"
...
```

查看替换结果:

> sed 's/docker.io/docker.1ms.run/g' /etc/containers/registries.conf.d/shortnames.conf

在原文件直接替换:  

> sudo sed 's/docker.io/docker.1ms.run/g' /etc/containers/registries.conf.d/shortnames.conf -i

替换后结果如下:

```/etc/containers/registries.conf.d/shortnames.conf
...

  # docker
  "alpine" = "docker.1ms.run/library/alpine"
  "docker" = "docker.1ms.run/library/docker"
  "registry" = "docker.1ms.run/library/registry"
  "hello-world" = "docker.1ms.run/library/hello-world"
...

# Ubuntu
  "ubuntu" = "docker.1ms.run/library/ubuntu"
  # Oracle Linux
  "oraclelinux" = "container-registry.oracle.com/os/oraclelinux"
  # busybox
  "busybox" = "docker.1ms.run/library/busybox"
  # php
  "php" = "docker.1ms.run/library/php"
  # python
  "python" = "docker.1ms.run/library/python"
  # node
  "node" = "docker.1ms.run/library/node"
...
```



## container

> podman run --name pg16 -e POSTGRES_USER=postgres -e POSTGRES_PASSWORD=fccdjny -p 5432:5432 -d postgres

## exec

> podman exec -it pg16 psql -U postgres


## podman desktop

[podman desktop download](https://podman-desktop.io/downloads)  

完成安装后可以在桌面程序上管理容器, pod, 网络和存储卷，也能和k8s进行交互.

podman desktop 无法识别在/etc/containers/registries.conf中配置的源, 界面中设置源也不生效:

![设置镜像源的优先级](attach/Pasted%20image%2020260105173455.png)

直接在 podman desktop 输入镜像名字拉取时会报错(如果在命令行执行 podman pull hello-world). 

![](attach/Pasted%20image%2020260105173438.png)


因为podman默认建议使用全路径容器限定名，所以在 podman desktop 上拉取镜像时最好指定源的域名路径: docker.1ms.run

![](attach/Pasted%20image%2020260105173232.png)


不管在界面还是命令行拉取的镜像，在界面都可以管理:

![](attach/Pasted%20image%2020260105173835.png)


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

在windows 中 C:\Users\${USER} 创建 .wslconfig 文件并写入以下配置识别系统代理
```ini
[experimental]
autoMemoryReclaim=gradual
networkingMode=mirrored
dnsTunneling=true
firewall=true
autoProxy=true

```

