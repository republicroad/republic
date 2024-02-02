
## lxd

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