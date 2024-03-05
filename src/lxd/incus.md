
[important-notice-for-lxd-users-image-server](https://discuss.linuxcontainers.org/t/important-notice-for-lxd-users-image-server/18479)

lxd 项目已经移出了 linuxcontainer 项目，从 lxd fork 出的 incus 项目继续系统容器项目. 

## install and config

### install package

[Incus builds and install](https://github.com/zabbly/incus)
### add user to incus-admin

	sudo adduser YOUR-USERNAME incus-admin
	newgrp incus-admin


### Initialize Incus or migrate from lxd

如果是从 lxd 迁移, 不要初始化 incus. 使用 lxd-to-incus 进行迁移.

如果不需要从 lxd 迁移, 执行初始化命令:

	incus admin init

## remote image server

[Image server for Incus and LXC](https://images.linuxcontainers.org/)