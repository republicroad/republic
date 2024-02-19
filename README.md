# republic
大梦谁先觉, 平生我自知.

[github地址](https://github.com/republicroad/republic/tree/main)

> [文档地址](https://republicroad.github.io/republic/)

## tech


### lang

从 c 开始.

### lxd

linux 上系统容器和虚拟机软件. 方便模拟各种发行版. 搭建集群, 进行分布式测试.


### devops

运维开发, 熟悉各种系统, 包管理等.


### git


## 
##  organazation

这里查看 republic 组织成员. 处理组织人员管理.

	https://github.com/orgs/republicroad/people


## 协作流程

首先安装 git, 然后使用 git clone 此文档.

	# 如果是使用 git clone git@github.com:RYefccd/republic.git 
	# 那么默认的 origin 就是 git@github.com:RYefccd/republic.git, 对这个进行重命名
	# 如果用 https 协议 clone 的话, 那么重新设置 originssh 即可
	git remote rename origin originssh
	git remote add origin https://github.com/republicroad/republic.git

最后效果如下, 把仓库的 ssh 协议和https协议都配置完成.
```shell
$ git remote -v
origin  https://github.com/republicroad/republic.git (fetch)
origin  https://github.com/republicroad/republic.git (push)
originssh       git@github.com:RYefccd/republic.git (fetch)
originssh       git@github.com:RYefccd/republic.git (push)
```


## mdbook

将下载的 mdbook 压缩包解压得到可执行文件 mdbook.exe, 把此文件放到系统的默认搜索目录下.

	echo $PATH   linux, mac
	echo %PATH%  windows

可以看到系统搜索目录.

	比如 window11 可以放到用户AppData中的WindowsApps文件夹中.
	C:\Users\RYefccd\AppData\Local\Microsoft\WindowsApps

可以用一下命令来把markdown文件构建成html页面并在网页打开.

	mdbook.exe build republic -o

或者可以开启一个本地服务, 可以边修改文档边刷新:

	mdbook.exe serve republic


### mdbook action


1. 克隆仓库 

	git clone https://github.com/republicroad/republic.git

	```bash
	# 代码结构如下
	wh@VM-16-16-ubuntu:~/workspace$ tree -L 2 .
	.
	├── build_book.sh
	└── republic
	    ├── attach
	    ├── book.toml
	    ├── Excalidraw
	    ├── excalidrawlib
	    ├── journal
	    ├── LICENSE
	    ├── obsidian_config
	    ├── README.md
	    └── src
	```
	
2. mdbook 构建 

	mdbook build republic


```bash
wh@VM-16-16-ubuntu:~/workspace$ mdbook build republic
2024-02-04 16:55:57 [INFO] (mdbook::book): Book building has started
2024-02-04 16:55:57 [INFO] (mdbook::book): Running the html backend
```


```bash
wh@VM-16-16-ubuntu:~/workspace$ tree -L 2 .
.
├── build_book.sh
└── republic
    ├── attach
    ├── book  # 多了一个book文件夹
    ├── book.toml
    ├── Excalidraw
    ├── excalidrawlib
    ├── journal
    ├── LICENSE
    ├── obsidian_config
    ├── README.md
    └── src
```

3. nginx 配置
```
        location /book/ {
            #root  html;
            root /home/wh/workspace/republic/;
            index  index.html index.htm;
        }

```

http://150.158.144.155/book/