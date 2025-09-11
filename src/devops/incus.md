
## install and config

### install package

[Incus builds and install](https://github.com/zabbly/incus)
### add user to incus-admin

	sudo adduser YOUR-USERNAME incus-admin
	newgrp incus-admin


### incus初始配置

将当前用户添加至 incus-admin组
>    sudo adduser $USER incus-admin
>    newgrp incus-admin

初始化 incus 配置
>    incus admin init

```yaml
config:
  core.https_address: '[::]:8443'
networks:
- config:
    ipv4.address: 10.0.0.1/8
    ipv4.nat: "true"
    ipv6.address: auto
  description: ""
  name: incusbr0
  type: ""
  project: default
storage_pools:
- config: {}
  description: ""
  name: default
  driver: dir
profiles:
- config: {}
  description: ""
  devices:
    eth0:
      name: eth0
      network: incusbr0
      type: nic
    root:
      path: /
      pool: default
      type: disk
  name: default
projects: []
cluster: null

```

### 更换image源

```bash
incus remote rename images images_bak
incus remote add images https://mirrors.tuna.tsinghua.edu.cn/lxc-images/ --protocol=simplestreams --public
```

查看image源
```bash
incus remote list
# 分页查看image
incus image list images: |less
```


### 下载image

>    incus image copy images:ubuntu/24.10  local:


### 从镜像创建容器

>    incus launch local:ubuntu/24.10 u1
>    incus launch images:ubuntu/24.10 u2

查看本地镜像  
>  incus image list
```bash
ryefccd@republic:~$ incus image list
+-------+--------------+--------+----------------------------------------+--------------+-----------+-----------+----------------------+
| ALIAS | FINGERPRINT  | PUBLIC |              DESCRIPTION               | ARCHITECTURE |   TYPE    |   SIZE    |     UPLOAD DATE      |
+-------+--------------+--------+----------------------------------------+--------------+-----------+-----------+----------------------+
|       | 4924f427183e | no     | Ubuntu oracular amd64 (20250628_07:42) | x86_64       | CONTAINER | 126.26MiB | 2025/09/10 14:21 CST |
+-------+--------------+--------+----------------------------------------+--------------+-----------+-----------+----------------------+
```

给本地镜像命名别名  
> incus image alias create local:ubuntu/24.10 4924f427183e

```bash
ryefccd@republic:~$ incus image list
+--------------+--------------+--------+----------------------------------------+--------------+-----------+-----------+----------------------+
|    ALIAS     | FINGERPRINT  | PUBLIC |              DESCRIPTION               | ARCHITECTURE |   TYPE    |   SIZE    |     UPLOAD DATE      |
+--------------+--------------+--------+----------------------------------------+--------------+-----------+-----------+----------------------+
| ubuntu/24.10 | 4924f427183e | no     | Ubuntu oracular amd64 (20250628_07:42) | x86_64       | CONTAINER | 126.26MiB | 2025/09/10 14:21 CST |
+--------------+--------------+--------+----------------------------------------+--------------+-----------+-----------+----------------------+
```

### 容器运行命令

>    incus exec u1 --  lsb_release -a
>    incus exec u1 --  date

### 进入容器

以 ubuntu 用户进入容器(主要针对ubuntu镜像, 有默认 ubuntu 用户)
>    incus exec u1 -- sudo --user ubuntu --login

以 root 用户进入bash:
>    incus exec u1 bash

## 从 lxd 迁移

[important-notice-for-lxd-users-image-server](https://discuss.linuxcontainers.org/t/important-notice-for-lxd-users-image-server/18479)

lxd 项目已经移出了 linuxcontainer 项目，从 lxd fork 出的 incus 项目继续系统容器项目. 


## 参考资料

[Image server for Incus and LXC](https://images.linuxcontainers.org/)  
[Incus 使用镜像加速的方法:清华大学开源软件镜像站](https://mirrors.tuna.tsinghua.edu.cn/help/lxc-images/)  
