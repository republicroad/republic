
# zig

## started

[Getting Started](https://ziglang.org/learn/getting-started/)  
[zig.guide](https://zig.guide/)
[zig.show/](https://zig.show/)  

[有了 C++、D 和 Rust，为什么还需要 Zig？](https://ziglang.org/zh-CN/learn/why_zig_rust_d_cpp/)
[代码范例](https://ziglang.org/zh-CN/learn/samples/)

book:  
[zig-cookbook](https://cookbook.ziglang.cc/)  
[zig-book](https://pedropark99.github.io/zig-book/)
## install

### zig official

https://zig.guide/getting-started/installation  

### zvm

https://github.com/tristanisham/zvm  


## build

[Zig 构建系统](https://ziglang.org/learn/build-system/#installing-artifacts)

初始化项目

> zig init  

```zig
PS C:\Users\RYefccd\Documents\workspace\republic\langsrc\zig\hello-world> zig init       
info: created build.zig
info: created build.zig.zon
info: created src\main.zig
info: created src\root.zig
info: see `zig build --help` for a menu of options

```


运行项目

> zig build run


单元测试

>  zig build test


构建项目

>  zig build

## as c compiler

### zig cc

https://ziglang.org/download/0.4.0/release-notes.html#Zig-is-also-a-C-Compiler  

```powershell
PS C:\Users\RYefccd> zig cc --version
clang version 20.1.2 (https://github.com/ziglang/zig-bootstrap 7ddb1d2d582893ffac7c1a7c5c893a02933a6817)
Target: x86_64-unknown-windows-gnu
Thread model: posix
InstalledDir: C:/Users/RYefccd/.zvm/master
```

### 依赖库搜索路径与系统搜索路径


```hello.c

#include <stdio.h> int main(int argc, char **argv) 
{ 
	printf("Hello world\n"); 
	return 0; 
}
```


```bash
$ zig build-exe --c-source hello.c --library c $ ./hello Hello world
```

加入 --verbose-cc 可以查看真个编译的命令输出

```bash
zig build-exe --c-source hello.c --library c --verbose-cc 
zig cc -MD -MV -MF zig-cache/tmp/42zL6fBH8fSo-hello.o.d -nostdinc -fno-spell-checking -isystem /home/andy/dev/zig/build/lib/zig/include -isystem /home/andy/dev/zig/build/lib/zig/libc/include/x86_64-linux-gnu -isystem /home/andy/dev/zig/build/lib/zig/libc/include/generic-glibc -isystem /home/andy/dev/zig/build/lib/zig/libc/include/x86_64-linux-any -isystem /home/andy/dev/zig/build/lib/zig/libc/include/any-linux-any -march=native -g -fstack-protector-strong --param ssp-buffer-size=4 -fno-omit-frame-pointer -o zig-cache/tmp/42zL6fBH8fSo-hello.o -c hello.c -fPIC
```

不要忘记使用 `foo.linkSystemLibrary("c");` 去链接 `libc` 库(默认c库). 注意到zig生成的c编译命令包含 `-nostdinc`, 这个是zig提供一致可靠的构建系统的关键步骤. 注意这个会影响c语言的翻译和zig的[@cImport](https://ziglang.org/documentation/0.4.0/#cImport) 功能.
默认情况下，zig不在系统默认路径去搜索c标准库的头文件，用户必须显示提供 `-isystem`选项.
如果是 zig 构建系统, 可以使用 `linkSystemLibrary` API 来搜索查找系统默认搜索路径.
 [Marc Tiehuis 提议将此功能公开到命令行界面](https://github.com/ziglang/zig/issues/2041#issuecomment-479335822)
 
### libc

```bash
...
Available libcs: 
  aarch64_be-linux-gnu 
  aarch64_be-linux-musl 
  aarch64-linux-gnu 
  aarch64-linux-musleabi 
  armeb-linux-gnueabi 
  armeb-linux-gnueabihf 
  armeb-linux-musleabi 
  armeb-linux-musleabihf 
  arm-linux-gnueabi 
  arm-linux-gnueabihf 
  arm-linux-musleabi 
  arm-linux-musleabihf 
  i386-linux-gnu 
  i386-linux-musl 
  mips64el-linux-gnuabi64 
  mips64el-linux-gnuabin32 
  mips64el-linux-musl 
  mips64-linux-gnuabi64 
  mips64-linux-gnuabin32 
  mips64-linux-musl 
  mipsel-linux-gnu 
  mipsel-linux-musl 
  mips-linux-gnu 
  mips-linux-musl 
  powerpc64le-linux-gnu 
  powerpc64le-linux-musl 
  powerpc64-linux-gnu 
  powerpc64-linux-musl 
  powerpc-linux-gnu 
  powerpc-linux-musl 
  riscv32-linux-musl 
  riscv64-linux-gnu 
  riscv64-linux-musl 
  s390x-linux-gnu 
  s390x-linux-musl 
  sparc-linux-gnu 
  sparcv9-linux-gnu 
  x86_64-linux-gnu 
  x86_64-linux-gnux32 
  x86_64-linux-musl
```

注意, `--library c`意味着不使用任何的系统文件. 

```bash
$ zig build-exe --c-source hello.c --library c 
$ ./hello Hello world 
$ ldd ./hello 
	linux-vdso.so.1 (0x00007ffd03dc9000) 
	libc.so.6 => /lib/libc.so.6 (0x00007fc4b62be000) 
	libm.so.6 => /lib/libm.so.6 (0x00007fc4b5f29000) 
	libpthread.so.0 => /lib/libpthread.so.0 (0x00007fc4b5d0a000) 
	libdl.so.2 => /lib/libdl.so.2 (0x00007fc4b5b06000) 
	librt.so.1 => /lib/librt.so.1 (0x00007fc4b58fe000) 
	/lib/ld-linux-x86-64.so.2 => /lib64/ld-linux-x86-64.so.2 (0x00007fc4b6672000)
```

glibc 不支持静态构建, 但[musl](https://www.musl-libc.org/)却支持静态构建(golang默认支持静态构建).

```bash
$ zig build-exe --c-source hello.c --library c -target x86_64-linux-musl 
$ ./hello Hello world 
$ ldd hello not a dynamic executable
```

[在此示例中，Zig 从源代码构建了 musl libc，然后链接到它。由于缓存系统](https://ziglang.org/download/0.4.0/release-notes.html#Build-Artifact-Caching)的存在，x86_64-linux 版本的 musl libc 仍然可用 ，因此，任何时候再次需要此 libc 时，它都可以立即使用。

这意味着此功能可在任何平台上使用。Windows 和 macOS 用户可以为上述任何目标平台构建 Zig 和 C 代码，并链接到 libc。同样，代码也可以针对其他架构进行交叉编译：

```bash
$ zig build-exe --c-source hello.c --library c -target aarch64v8-linux-gnu 
$ file hello 
hello: ELF 64-bit LSB executable, ARM aarch64, version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-aarch64.so.1, for GNU/Linux 2.0.0, with debug_info, not stripped
```