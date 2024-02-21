
# vscode

## windows

### git bash 

在windows启用  [git bash](https://stackoverflow.com/a/50159674)


### c/c++

windows上进行c的开发需要安装c/c++的编译器和调试器. 
- [mingw-w64](http://mingw-w64.org/)
- [MSVC](https://visualstudio.microsoft.com/downloads/#remote-tools-for-visual-studio-2022)


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
[学生成绩管理系统](../lang/c/02_c_standard.md#学生成绩管理系统)


![](attach/Pasted%20image%2020240221181400.png)