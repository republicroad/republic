
## lxd

> 请迁移至 incus 项目

### lxd-ui
https://documentation.ubuntu.com/lxd/en/latest/howto/access_ui/

####  1 安装与常用命令
```shell
# 1也可以用 snap install lxd  snap新的包管理工具 ubuntu16及以上 低版本需要查看链接
# https://www.linuxprobe.com/lxd-2-0-install-config.html
 sudo apt-get install lxd  

# 2 LXD 提供了几种存储后端。我们的推荐是 ZFS,提供最快和最可靠的容器体验。ubuntu16及以上
 sudo apt install zfsutils-linux

# 3 初始化 选默认的就好
 sudo lxd init

# 4 添加清华源
lxc remote rename images images_bak
lxc remote add images https://mirrors.tuna.tsinghua.edu.cn/lxc-images/ --protocol=simplestreams --public

# 5 比如查看默认images源下ubuntu:1804在远程镜像里的一些信息 
lxc image list images:ubuntu/18.04 | less  

+-----------------------------------+--------------+--------+--------------------------------------+---------+----------+-------------------------------+
|               ALIAS               | FINGERPRINT  | PUBLIC |             DESCRIPTION              |  ARCH   |   SIZE   |          UPLOAD DATE          |
+-----------------------------------+--------------+--------+--------------------------------------+---------+----------+-------------------------------+
| ubuntu/18.04 (7 more)             | fd42d9695679 | yes    | Ubuntu bionic amd64 (20230829_07:42) | x86_64  | 108.14MB | Aug 29, 2023 at 12:00am (UTC) |
+-----------------------------------+--------------+--------+---------------------------
# 6 拉取远程镜像到本地
lxc image copy images:ubuntu/18.04 local:

# 7 查看本地镜像image 可以看到刚才拉取的镜像
lxc image list
+-------+--------------+--------+--------------------------------------+--------+---------+-----------------------------+
| ALIAS | FINGERPRINT  | PUBLIC |             DESCRIPTION              |  ARCH  |  SIZE   |         UPLOAD DATE         |
+-------+--------------+--------+--------------------------------------+--------+---------+-----------------------------+
|       | 8b430b6d8271 | no     | Ubuntu xenial amd64 (20230829_07:42) | x86_64 | 87.89MB | Feb 1, 2024 at 5:37am (UTC) |
+-------+--------------+--------+--------------------------------------+--------+---------+-----------------------------+

# 8 根据本地镜像，创建一个容器 后面是镜像id  ut18是取的别名
lxc launch 8b430b6d8271 ut18

# 9 当前用户添加到用户组
newgrp lxd

# 10 可以查看本地List containers容器
lxc list 
lxc exec ut18 bash   # 进入容器

# 11 管理容器
lxc stop container-name    # 暂停
lxc delete container-name  # 删除  可以加--force
```



### trust

    lxc config trust list  # 查看证书
    lxc config trust add lxd-ui-192.168.0.39.crt  # 添加证书

### lxc network

创建桥接网卡(分配外网地址)
- https://youtu.be/xCcrgUldEGo?t=738
- https://seanblanchfield.com/2023/05/bridge-networking-in-lxd
- https://blog.simos.info/-how-to-make-your-lxd-containers-get-ip-addresses-from-your-lan-using-a-bridge/
- https://docs.rockylinux.org/books/lxd_server/05-lxd_images/

```shell
lxc stop c1
lxc network attach lxdbr0 c1 eth0 eth0
lxc config device set c1 eth0 ipv4.address 10.99.10.42
lxc start c1
```



### docker in lxd

[how-to-run-docker-inside-lxd-containers](https://ubuntu.com/tutorials/how-to-run-docker-inside-lxd-containers#2-create-lxd-container)


## cloud-images.ubuntu

[cloud-images.ubuntu](https://cloud-images.ubuntu.com/)

```
ryefccd@republic:~$ lxc remote list
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
|         NAME         |                        URL                        |   PROTOCOL    |  AUTH TYPE  | PUBLIC | STATIC | GLOBAL |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| images               | https://mirrors.tuna.tsinghua.edu.cn/lxc-images/  | simplestreams | none        | YES    | NO     | NO     |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| images_bak           | https://images.linuxcontainers.org                | simplestreams | none        | YES    | NO     | NO     |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| local (current)      | unix://                                           | lxd           | file access | NO     | YES    | NO     |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| ubuntu               | https://cloud-images.ubuntu.com/releases          | simplestreams | none        | YES    | YES    | NO     |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| ubuntu-daily         | https://cloud-images.ubuntu.com/daily             | simplestreams | none        | YES    | YES    | NO     |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| ubuntu-minimal       | https://cloud-images.ubuntu.com/minimal/releases/ | simplestreams | none        | YES    | YES    | NO     |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| ubuntu-minimal-daily | https://cloud-images.ubuntu.com/minimal/daily/    | simplestreams | none        | YES    | YES    | NO     |
```


## lxd init(2019)

 1. snap install lxd --channel=3.0/stable


```shell
ryefccd@fccd:~$ snap install lxd --channel=3.0/stable
lxd (3.0/stable) 3.0.4 from Canonical✓ installed
```

 2. sudo lxd init
```shell
ryefccd@fccd:~$ sudo lxd init
Would you like to use LXD clustering? (yes/no) [default=no]: 
Do you want to configure a new storage pool? (yes/no) [default=yes]: 
Name of the new storage pool [default=default]: 
Name of the storage backend to use (btrfs, ceph, dir, lvm, zfs) [default=zfs]: 
Create a new ZFS pool? (yes/no) [default=yes]: 
Would you like to use an existing block device? (yes/no) [default=no]: 
Size in GB of the new loop device (1GB minimum) [default=42GB]: 
Would you like to connect to a MAAS server? (yes/no) [default=no]: 
Would you like to create a new local network bridge? (yes/no) [default=yes]: 
What should the new bridge be called? [default=lxdbr0]: 
What IPv4 address should be used? (CIDR subnet notation, “auto” or “none”) [default=auto]: 
What IPv6 address should be used? (CIDR subnet notation, “auto” or “none”) [default=auto]: 
Would you like LXD to be available over the network? (yes/no) [default=no]: yes
Address to bind LXD to (not including port) [default=all]: 
Port to bind LXD to [default=8443]: 
Trust password for new clients: 
Again: 
Would you like stale cached images to be updated automatically? (yes/no) [default=yes] 
Would you like a YAML "lxd init" preseed to be printed? (yes/no) [default=no]: yes
config:
  core.https_address: '[::]:8443'
  core.trust_password: fccdjny
networks:
- config:
    ipv4.address: auto
    ipv6.address: auto
  description: ""
  managed: false
  name: lxdbr0
  type: ""
storage_pools:
- config:
    size: 42GB
  description: ""
  name: default
  driver: zfs
profiles:
- config: {}
  description: ""
  devices:
    eth0:
      name: eth0
      nictype: bridged
      parent: lxdbr0
      type: nic
    root:
      path: /
      pool: default
      type: disk
  name: default
cluster: null
```
 3. 重命名原来的 image 源, 并添加清华的源

```shell
lxc remote rename images images_bak
lxc remote add images https://mirrors.tuna.tsinghua.edu.cn/lxc-images/ --protocol=simplestreams --public
```

 4. lxc image copy images:ubuntu/16.04 local:
```shell
ryefccd@fccd:~$ lxc image list
+-------+--------------+--------+-----------------------------------------------+--------+----------+------------------------------+
| ALIAS | FINGERPRINT  | PUBLIC |                  DESCRIPTION                  |  ARCH  |   SIZE   |         UPLOAD DATE          |
+-------+--------------+--------+-----------------------------------------------+--------+----------+------------------------------+
|       | 368bb7174b67 | no     | ubuntu 18.04 LTS amd64 (release) (20190722.1) | x86_64 | 177.56MB | Jul 29, 2019 at 2:07am (UTC) |
+-------+--------------+--------+-----------------------------------------------+--------+----------+------------------------------+
|       | 4bfe62583826 | no     | ubuntu 14.04 LTS amd64 (release) (20190514)   | x86_64 | 122.40MB | Jul 29, 2019 at 4:52am (UTC) |
+-------+--------------+--------+-----------------------------------------------+--------+----------+------------------------------+
|       | 5337a61fbc44 | no     | Ubuntu trusty amd64 (20190728_07:42)          | x86_64 | 74.81MB  | Jul 29, 2019 at 4:00am (UTC) |
+-------+--------------+--------+-----------------------------------------------+--------+----------+------------------------------+
|       | 8b430b6d8271 | no     | ubuntu 16.04 LTS amd64 (release) (20190628)   | x86_64 | 158.72MB | Jul 29, 2019 at 4:07am (UTC) |
+-------+--------------+--------+-----------------------------------------------+--------+----------+------------------------------+
```

 5. lxc launch local:8b430b6d8271 redisai

```shell
ryefccd@fccd:~$ lxc list
+---------+---------+----------------------+-----------------------------------------------+------------+-----------+
|  NAME   |  STATE  |         IPV4         |                     IPV6                      |    TYPE    | SNAPSHOTS |
+---------+---------+----------------------+-----------------------------------------------+------------+-----------+
| redisai | RUNNING | 10.213.178.59 (eth0) | fd42:8fa4:9940:a6aa:216:3eff:fe16:efaa (eth0) | PERSISTENT | 0         |
+---------+---------+----------------------+-----------------------------------------------+------------+-----------+
| whtest  | STOPPED |                      |                                               | PERSISTENT | 0         |
+---------+---------+----------------------+-----------------------------------------------+------------+-----------+
```

 6. lxc exec redisai -- sudo --user ubuntu --login
 7. 制作本地镜像
	
	 lxc publish my-container --alias my-new-image
	 lxc publish my-container/some-snapshot --alias some-image

