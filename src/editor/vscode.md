
# vscode

## windows

### git bash 

在windows启用  [git bash](https://stackoverflow.com/a/50159674)


### c/c++

windows上进行c的开发需要安装c/c++的编译器和调试器. 
- [mingw-w64](http://mingw-w64.org/)
- [MSVC](https://visualstudio.microsoft.com/downloads/#remote-tools-for-visual-studio-2022)

 [msys2(Software Distribution and Building Platform for Windows)](https://www.msys2.org/) 

#### 安装 c/c++ extentions

在 vscode 中安装 c extension.

![](attach/Pasted%20image%2020240221160105.png)

#### 安装gcc(mingw-w64)编译器

1. 安装MSYS2. [下载地址](https://github.com/msys2/msys2-installer/releases/download/2024-01-13/msys2-x86_64-20240113.exe)
2. 安装完后在弹出的命令行里安装工具链

	```
	pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
	```

3. 一路回车和Y键就会安装c语言的编译器(gcc)和调试器(gdb).

	C:\msys64\ucrt64\bin 在此目录下可以看到已经装好的编译器和调试器

	![](attach/Pasted%20image%2020240221153014.png)

4. 为了更够在任何目录去调用这些命令, 需要把gcc和gdb所在的目录添加到系统环境变量PATH中. 
	- 在设置中搜索环境变量, 选择**编辑账户的环境变量**.
		![](attach/Pasted%20image%2020240221153724.png)
	- 选择**PATH**, 点击编辑 
		![](attach/Pasted%20image%2020240221153949.png)
	- 选择新建后点击浏览, 在打开的浏览文件夹选择C:\msys64\ucrt64\bin, 点击确定
		![](attach/Pasted%20image%2020240221154306.png)
		![](attach/Pasted%20image%2020240221154710.png)

5. 打开 ucrt 命令行, 输入 gcc --version 可验证是否安装成功.
	![](attach/Pasted%20image%2020240221155253.png)

	![](attach/Pasted%20image%2020240221155443.png)

参考资料:	  
[Installing the MinGW-w64 toolchain](https://code.visualstudio.com/docs/cpp/config-mingw#_installing-the-mingww64-toolchain)
[MSYS2 Environments](https://www.msys2.org/docs/environments/)

#### 运行程序

代码如下:

![](attach/Pasted%20image%2020240221181400.png)


## ubuntu


### 基础依赖

安装好 [vscode]() 和  [C++ extension for VS Code](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)


### 编译调试

首先安装开发工具链(编译器, 连接器)和调试器.

```
sudo apt install build-essential gdb
```

确定 gcc 和 gdb 成功安装.



```
ryefccd@republic:~$ gcc -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/11/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none:amdgcn-amdhsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 11.4.0-1ubuntu1~22.04' --with-bugurl=file:///usr/share/doc/gcc-11/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,m2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-11 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-bootstrap --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --enable-libphobos-checking=release --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --enable-cet --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-11-XeT9lY/gcc-11-11.4.0/debian/tmp-nvptx/usr,amdgcn-amdhsa=/build/gcc-11-XeT9lY/gcc-11-11.4.0/debian/tmp-gcn/usr --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu --with-build-config=bootstrap-lto-lean --enable-link-serialization=2
Thread model: posix
Supported LTO compression algorithms: zlib zstd
gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04) 
```

```
ryefccd@republic:~$ gdb -v
GNU gdb (Ubuntu 12.1-0ubuntu1~22.04) 12.1
Copyright (C) 2022 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

```



> [!NOTE] dpkg
> 也可以用 dpkg -L 来查看这个库包含哪里文件.  
> dpkg -L build-essential   
> [dpkg](../devops/package_manager.md#dpkg)  



参考链接:
[Using C++ on Linux in VS Code](https://code.visualstudio.com/docs/cpp/config-linux)
