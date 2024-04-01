
## chroot

创建新的 root 目录

	mkdir myroot
	
查看 /bin/bash 执行文件的依赖库

	ldd /bin/bash

```
wh@ubuntu22:~$ ldd /bin/bash
	linux-vdso.so.1 (0x00007ffe9bb83000)
	libtinfo.so.6 => /lib/x86_64-linux-gnu/libtinfo.so.6 (0x00007362954c8000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x0000736295200000)
	/lib64/ld-linux-x86-64.so.2 (0x000073629566c000)

```

把所有的依赖库复制到 myroot 对应的目录中

	mkdir -p myroot/lib/ myroot/lib64/ myroot/bin
	cp /lib/x86_64-linux-gnu/libtinfo.so.6 myroot/lib/
	cp /lib/x86_64-linux-gnu/libc.so.6 myroot/lib/
	cp /lib64/ld-linux-x86-64.so.2 myroot/lib64/
	cp /bin/bash myroot/bin/

进入chroot环境(sudo chroot myroot/ bash)

```
wh@ubuntu22:~$ sudo chroot myroot/ 
bash-5.1# pwd
/
bash-5.1# ls
bin  lib  lib64
bash-5.1# 

```

执行 sudo chroot myroot/ ls 命令也是类似, 把二进制命令和依赖库都复制进去.

```
wh@ubuntu22:~$ sudo chroot myroot/ ls
bin  lib  lib64
```


下面是所有的操作:  
```
wh@ubuntu22:~$ mkdir myroot
wh@ubuntu22:~$ 
wh@ubuntu22:~$ mkdir -p myroot/lib/ myroot/lib64/
wh@ubuntu22:~$ cp /lib/x86_64-linux-gnu/libtinfo.so.6 myroot/lib/
wh@ubuntu22:~$ cp /lib/x86_64-linux-gnu/libc.so.6 myroot/lib/
wh@ubuntu22:~$ cp /lib64/ld-linux-x86-64.so.2 myroot/lib64/
wh@ubuntu22:~$ cp /lib/x86_64-linux-gnu/libselinux.so.1 myroot/lib/
wh@ubuntu22:~$ cp /lib/x86_64-linux-gnu/libpcre2-8.so.0 myroot/lib/

wh@ubuntu22:~$ tree -L 3 myroot/ 
myroot/
├── bin
│   ├── bash
│   └── ls
├── lib
│   ├── libc.so.6
│   ├── libpcre2-8.so.0
│   ├── libselinux.so.1
│   └── libtinfo.so.6
└── lib64
    └── ld-linux-x86-64.so.2

```


### schroot


### [debootstrap](https://wiki.debian.org/Debootstrap)

debootstrap 用来制作

```
wh@ubuntu22:~/chroot_env$ mkdir ub18
wh@ubuntu22:~/chroot_env$ sudo debootstrap --variant=buildd bionic ub18 https://mirrors.tuna.tsinghua.edu.cn/ubuntu/

wh@ubuntu22:~/chroot_env$ sudo chroot ub18/
root@ubuntu22:/# ldd --version
ldd (Ubuntu GLIBC 2.27-3ubuntu1) 2.27
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
Written by Roland McGrath and Ulrich Drepper.
root@ubuntu22:/# cat /etc/issue
Ubuntu 18.04 LTS \n \l

```


### link

- [How to build an Ubuntu chroot environment](https://www.simplified.guide/ubuntu/build-chroot-environment)
- [List of Ubuntu releases](https://wiki.ubuntu.com/Releases "https://wiki.ubuntu.com/Releases")[](https://wiki.ubuntu.com/Releases)
- https://wiki.ubuntu.com/DebootstrapChroot
- https://wiki.debian.org/Schroot
- [chroot-linux-command](https://ioflood.com/blog/chroot-linux-command/)
