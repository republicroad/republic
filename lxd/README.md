
## lxd

### lxd-ui
https://documentation.ubuntu.com/lxd/en/latest/howto/access_ui/



### 基本操作

- 查看remote 

      lxc remote list

```shell
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
|         NAME         |                        URL                        |   PROTOCOL    |  AUTH TYPE  | PUBLIC | STATIC | GLOBAL |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| images               | https://images.linuxcontainers.org                | simplestreams | none        | YES    | NO     | NO     |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| local (current)      | unix://                                           | lxd           | file access | NO     | YES    | NO     |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| mirror-images        | https://mirrors.tuna.tsinghua.edu.cn/lxc-images/  | simplestreams | none        | YES    | NO     | NO     |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| ubuntu               | https://cloud-images.ubuntu.com/releases          | simplestreams | none        | YES    | YES    | NO     |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| ubuntu-daily         | https://cloud-images.ubuntu.com/daily             | simplestreams | none        | YES    | YES    | NO     |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| ubuntu-minimal       | https://cloud-images.ubuntu.com/minimal/releases/ | simplestreams | none        | YES    | YES    | NO     |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+
| ubuntu-minimal-daily | https://cloud-images.ubuntu.com/minimal/daily/    | simplestreams | none        | YES    | YES    | NO     |
+----------------------+---------------------------------------------------+---------------+-------------+--------+--------+--------+

```


- 添加清华的lxd源

      lxc remote add mirror-images https://mirrors.tuna.tsinghua.edu.cn/lxc-images/ --protocol=simplestreams --public


- 查看源对应的镜像和容器

      lxc image list mirror-images: | less  # less 可以对结果进行翻页


- 从images中创建一个容器并命名

       lxc init images:rockylinux/8 rockylinux-test  
       # 从另一个 remote 创建容器(清华源, 加快创建速度) 
       lxc init mirror-images:rockylinux/8 rockylinux-test2

- 从images中创建一个容器, 命名并启动

       lxc launch images:rockylinux/8 rockylinux-test-8


### trust

    lxc config trust list  # 查看证书
    lxc config trust add lxd-ui-192.168.0.39.crt  # 添加证书

### lxc network

创建桥接网卡(分配外网地址)
- https://youtu.be/xCcrgUldEGo?t=738
- https://seanblanchfield.com/2023/05/bridge-networking-in-lxd
- https://blog.simos.info/-how-to-make-your-lxd-containers-get-ip-addresses-from-your-lan-using-a-bridge/
- https://docs.rockylinux.org/books/lxd_server/05-lxd_images/