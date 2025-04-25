
# zig


[有了 C++、D 和 Rust，为什么还需要 Zig？](https://ziglang.org/zh-CN/learn/why_zig_rust_d_cpp/)
[zig-book 中文版](https://republicroad.github.io/zig-book-cn/zh/1.Zig%20%E7%AE%80%E4%BB%8B.html)  
https://republicroad.github.io/zig-book-cn/  

## install

### zig official

通过官方来安装zig, :

[zig快速入门](https://ziglang.org/zh-CN/learn/getting-started/#direct)
[zig版本下载页](https://ziglang.org/zh-CN/download/)  

### zvm

通过 zvm 来管理和安装不同的zig版本, 这个需要翻墙, 否则速度特别慢.
https://github.com/tristanisham/zvm  

## started

[代码范例](https://ziglang.org/zh-CN/learn/samples/)
[Getting Started](https://ziglang.org/zh-CN/learn/getting-started/)  
https://ziglang.org/learn/overview/  
https://ziglang.org/zh-CN/learn/samples/  
https://ziglang.org/documentation/0.14.0/std/  
[zig.guide](https://zig.guide/)
[zig.show/](https://zig.show/)  


book:  
[zig-cookbook](https://cookbook.ziglang.cc/)  
[zig-book](https://pedropark99.github.io/zig-book/)
[Zig Language Reference](https://ziglang.org/documentation/master/)

https://zig-by-example.com/  
https://github.com/zigcc/awesome-zig  



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
>  


### example

https://rischmann.fr/blog/how-i-built-zig-sqlite  
https://cookbook.ziglang.cc/14-01-sqlite.html  

## build

[Zig Build System](https://ziglang.org/learn/build-system/#installing-artifacts)  
[Zig 构建系统](https://ziglang.org/zh-CN/learn/build-system/)  
[何时使用 Zig 构建系统？](https://ziglang.org/zh-CN/learn/build-system/#build-system)

### zig build-exe

[请下载下列示例程序:](https://republicroad.github.io/republic/diveintosystems/%E7%AC%AC2%E7%AB%A0-%E6%B7%B1%E5%85%A5C%E8%AF%AD%E8%A8%80%E7%BC%96%E7%A8%8B/2.9.%20%E9%AB%98%E7%BA%A7C%E8%AF%AD%E8%A8%80%E6%A6%82%E5%BF%B5/2.9.5.%20%E7%BC%96%E8%AF%91,%20%E9%93%BE%E6%8E%A5%E5%92%8CC%E5%BA%93%E4%BD%BF%E7%94%A8.html)
- [`myprog.c`](https://diveintosystems.org/book/C2-C_depth/_attachments/myprog.c) 主程序
- [`mylib.h`](https://diveintosystems.org/book/C2-C_depth/_attachments/mylib.h) 库头文件
- [`mylib.c`](https://diveintosystems.org/book/C2-C_depth/_attachments/mylib.c) 库实现文件

使用此命令构建可执行文件. 可以支持 zig 文件，c/c++文件, 以及编译器的构建的中间文件.

```bash
$ zig build-exe  --verbose-cc myprog.c mylib.c -lc
$ ./myprog 
Enter two float values: 1 
2
1.000000 and 2.000000
2.000000 is the biggest
```

### zig build-lib

使用 build-lib 构建静态库文件和动态共享库文件. 默认情况下 build-lib 生成的是静态库, 需要使用 `-dynamic` 才会生成动态共享库.

静态库
```bash
$ zig build-lib mylib.c -lc
$ file libmylib.a 
libmylib.a: current ar archive
```

等价的gcc命令:

```bash
gcc -o mylib.o -c mylib.c
# 要构建静态库，请使用归档器 (`ar`)：
ar -rcs libmylib.a mylib.o
```

动态共享库

```bash
$ zig build-lib mylib.c -lc -dynamic
$ file libmylib.so 
libmylib.so: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, with debug_info, not stripped
```

等价的gcc命令:

```bash
gcc -fPIC -o mylib.o -c mylib.c
gcc -shared -o libmylib.so mylib.o
```


得到静态库和动态库以后可以通过c/c++的abi给其他语言调用.

#### 库文件在zig使用

build-exe如何使用动态共享库:

```bash
$ zig build-exe  --verbose-cc myprog.c -lc -lmylib -L.
$ readelf -d myprog

Dynamic section at offset 0x7c0 contains 25 entries:
  标记        类型                         名称/值
 0x000000000000001d (RUNPATH)            Library runpath: [.]
 0x0000000000000001 (NEEDED)             共享库：[./libmylib.so]
 0x0000000000000001 (NEEDED)             共享库：[libc.so.6]
 0x000000000000001e (FLAGS)              BIND_NOW
 0x000000006ffffffb (FLAGS_1)            标志： NOW
 ...
 $ ./myprog 
Enter two float values: 1
2
1.000000 and 2.000000
2.000000 is the biggest
```

build-exe如何使用静态库:

```bash
$ zig build-exe --verbose-cc myprog.c -search_static_first -lc -lmylib -L. 
$ readelf -d myprog

Dynamic section at offset 0x7d0 contains 24 entries:
  标记        类型                         名称/值
 0x000000000000001d (RUNPATH)            Library runpath: [.]
 0x0000000000000001 (NEEDED)             共享库：[libc.so.6]
 0x000000000000001e (FLAGS)              BIND_NOW
 0x000000006ffffffb (FLAGS_1)            标志： NOW
 0x0000000000000015 (DEBUG)              0x0
...
$ ./myprog 
Enter two float values: 1
2
1.000000 and 2.000000
2.000000 is the biggest

```

#### 库文件在gcc使用

gcc如何使用动态库
```bash
$ gcc myprog.c -lmylib -L.
$ ./a.out 
./a.out: error while loading shared libraries: libmylib.so: cannot open shared object file: No such file or directory
$ LD_LIBRARY_PATH=. ./a.out 
Enter two float values: 1
2
1.000000 and 2.000000
2.000000 is the biggest
```

注意, zig build-exe 构建的可执行程序可以直接运行，而 gcc 构建的程序可执行程序在运行时报错. 需要使用 `LD_LIBRARY_PATH=. ./a.out` 来运行程序.

```bash
$ ldd a.out 
        linux-vdso.so.1 (0x00007ffcde02e000)
        libmylib.so => not found
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007c9deea00000)
        /lib64/ld-linux-x86-64.so.2 (0x00007c9deee27000)

$ readelf -d a.out 

Dynamic section at offset 0x2da0 contains 28 entries:
  标记        类型                         名称/值
 0x0000000000000001 (NEEDED)             共享库：[libmylib.so]
 0x0000000000000001 (NEEDED)             共享库：[libc.so.6]
 0x000000000000000c (INIT)               0x1000
 0x000000000000000d (FINI)               0x128c
 0x0000000000000019 (INIT_ARRAY)         0x3d90
 ...
```

zig build-exe 会自动的给运行时程序添加 runpath(增加), 方便运行程序找到非系统路径中的动态共享库. 注意，库搜索的优先级顺序:
rpath  > LD_LIBRARY_PATH > runpath > /usr/local/lib > /usr/lib > /lib > ld.so.conf.d/*
```bash
$ readelf -d myprog

Dynamic section at offset 0x7c0 contains 25 entries:
  标记        类型                         名称/值
 0x000000000000001d (RUNPATH)            Library runpath: [.]
 0x0000000000000001 (NEEDED)             共享库：[./libmylib.so]
 0x0000000000000001 (NEEDED)             共享库：[libc.so.6]
 0x000000000000001e (FLAGS)              BIND_NOW
 0x000000006ffffffb (FLAGS_1)            标志： NOW
 ...
```


因为 linux 机器上可执行文件不会自动在当前路径寻找依赖共享库(windows的exe文件会再当前文件寻找依赖的 dll 库文件), runpath就是linux上非系统库路径的额外搜索路径.
所以一般linux上编译时需要设置 `-Wl,-rpath` 来设置 runpath(注意, 设置 rpath 也是这个配置， 不过需要额外设置`-Wl,--disable-new-dtags`), 如下所示:

- 同时设置 `-Wl,-rpath` 和 `-Wl,--enable-new-dtags` 就是设置 runpath
> gcc myprog.c  -Wl,-rpath='.'  -Wl,--enable-new-dtags -Wall  -lmylib -L.  -I. 

runpath:

```bash
$ gcc myprog.c  -Wl,-rpath='.'  -Wl,--enable-new-dtags -Wall  -lmylib -L.  -I. 
$ readelf -d a.out 

Dynamic section at offset 0x2d90 contains 29 entries:
  标记        类型                         名称/值
 0x0000000000000001 (NEEDED)             共享库：[libmylib.so]
 0x0000000000000001 (NEEDED)             共享库：[libc.so.6]
 0x000000000000001d (RUNPATH)            Library runpath: [.]
 0x000000000000000c (INIT)               0x1000
 0x000000000000000d (FINI)               0x128c
...

$ ldd a.out 
        linux-vdso.so.1 (0x00007ffeb2d79000)
        libmylib.so => ./libmylib.so (0x00007a1f12e01000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007a1f12a00000)
        /lib64/ld-linux-x86-64.so.2 (0x00007a1f12e0d000)
```


- 设置`-Wl,-rpath` 和`-Wl,--disable-new-dtags`就是设置 rpath
> gcc myprog.c -Wl,-rpath='$ORIGIN' -Wl,--disable-new-dtags -Wall  -lmylib -L. -I.

rpath:
```bash
$ gcc myprog.c -Wl,-rpath='.' -Wl,--disable-new-dtags -Wall  -lmylib -L. -I. 
$ ldd a.out 
        linux-vdso.so.1 (0x00007ffdaced9000)
        libmylib.so => ./libmylib.so (0x00007ac41a5cc000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007ac41a200000)
        /lib64/ld-linux-x86-64.so.2 (0x00007ac41a5d8000)
$ readelf -d a.out 

Dynamic section at offset 0x2d90 contains 29 entries:
  标记        类型                         名称/值
 0x0000000000000001 (NEEDED)             共享库：[libmylib.so]
 0x0000000000000001 (NEEDED)             共享库：[libc.so.6]
 0x000000000000000f (RPATH)              Library rpath: [.]
 0x000000000000000c (INIT)               0x1000
 0x000000000000000d (FINI)               0x128c
 0x0000000000000019 (INIT_ARRAY)         0x3d80
 ...
```

[How to set RPATH and RUNPATH with GCC/LD?](https://stackoverflow.com/questions/52018092/how-to-set-rpath-and-runpath-with-gcc-ld)

gcc如何使用静态库:
 
zig build-lib 默认构建的静态库在有全局变量时会添加 ubsan 相关检测标记，导致符号链接失败. 

```bash
$ zig build-lib mylib.c -lc
$ gcc myprog.c -lmylib -L.
/usr/bin/ld: ./libmylib.a(/home/ryefccd/.cache/zig/o/716ba062456398fc264fc49631553d67/mylib.o): in function `bigger':
/home/ryefccd/docs/republic/langsrc/zig/c/mylib.c:20: undefined reference to `__ubsan_handle_add_overflow'
collect2: error: ld returned 1 exit status

$ nm libmylib.a

/home/ryefccd/.cache/zig/o/716ba062456398fc264fc49631553d67/mylib.o:
0000000000000000 T bigger
0000000000000000 B total_times
                 U __ubsan_handle_add_overflow

```


加入 `-fubsan-rt` 命令行选项后, 生成的静态库中包含一个 ubsan_rt.o 对象文件.
可以被 gcc 静态链接
```bash
$ zig build-lib mylib.c -lc -fubsan-rt
$ gcc myprog.c -lmylib -L.
$ ./a.out 
Enter two float values: 1
2
1.000000 and 2.000000
2.000000 is the biggest

$ nm libmylib.a

/home/ryefccd/.cache/zig/o/716ba062456398fc264fc49631553d67/mylib.o:
0000000000000000 T bigger
0000000000000000 B total_times
                 U __ubsan_handle_add_overflow

/home/ryefccd/.cache/zig/o/8a5e029d67189932d0d94221a6a6319b/ubsan_rt.o:
                 U abort
0000000000000ba6 r __anon_10184
0000000000000bc1 r __anon_10197
00000000000000e3 r __anon_10201
0000000000000bcb r __anon_10207
...
```

因为 gcc ... -lxxx 这种方式是优先链接动态库，没有动态库就去链接静态库，如果需要手动指定静态库可以手动指定静态库文件:
```bash
zig build-lib mylib.c -lc -fubsan-rt
gcc myprog.c libmylib.a -L.
```


[Use both static and dynamically linked libraries in gcc](https://stackoverflow.com/a/809821)  
[Telling gcc directly to link a library statically](https://stackoverflow.com/questions/6578484/telling-gcc-directly-to-link-a-library-statically)
### zig build-obj

生成编译文件, 类似于 `gcc -c`生成的`.o`文件

```bash
$ zig build-obj mylib.c -lc
$ file mylib.o
mylib.o: ELF 64-bit LSB relocatable, x86-64, version 1 (SYSV), with debug_info, not stripped
```

等价的 gcc 命令如下所示:

```bash
$ gcc -c mylib.c -o mylib_gcc.o
$ file mylib_gcc.o 
mylib_gcc.o: ELF 64-bit LSB relocatable, x86-64, version 1 (SYSV), not stripped
$ file mylib.o
mylib.o: ELF 64-bit LSB relocatable, x86-64, version 1 (SYSV), not stripped
$ md5sum mylib_gcc.o 
d59f037c79ece46ef2d2162a2d664678  mylib_gcc.o
$ md5sum mylib.o 
d59f037c79ece46ef2d2162a2d664678  mylib.o
```

#### -fPIC vs -fno-PIC

和elf格式汇总的 GOT 表有关.
当target为如下 x86_64-linux-gnu 和 x86_64-linux-musl 有如下行为, x86_64-linux-gnu 架构需要生成位置无关代码(position independent code)， 

x86_64-linux-gnu:
```bash
$ zig build-obj -lc -fno-PIC mylib.c -target x86_64-linux-gnu
error: unable to create module 'mylib': the selected target requires position independent code
$ zig build-obj -lc -fPIC mylib.c -target x86_64-linux-gnu
$ readelf --relocs mylib.o | egrep '(GOT|PLT|JU?MP_SLOT)'
000000000015  000f0000002a R_X86_64_REX_GOTP 0000000000000000 total_times - 4
000000000040  001000000004 R_X86_64_PLT32    0000000000000000 __ubsan_handle_ad[...] - 4
00000000004a  000f0000002a R_X86_64_REX_GOTP 0000000000000000 total_times - 4
$ 
$ zig build-obj -lc  mylib.c 
$ readelf --relocs mylib.o | egrep '(GOT|PLT|JU?MP_SLOT)'
000000000015  000f0000002a R_X86_64_REX_GOTP 0000000000000000 total_times - 4
000000000040  001000000004 R_X86_64_PLT32    0000000000000000 __ubsan_handle_ad[...] - 4
00000000004a  000f0000002a R_X86_64_REX_GOTP 0000000000000000 total_times - 4
```


x86_64-linux-musl:

```bash
$ zig build-obj -lc -fno-PIC mylib.c -target x86_64-linux-musl
$ readelf --relocs mylib.o | egrep '(GOT|PLT|JU?MP_SLOT)'
000000000041  001000000004 R_X86_64_PLT32    0000000000000000 __ubsan_handle_ad[...] - 4
$ zig build-obj -lc -fPIC mylib.c -target x86_64-linux-musl
$ readelf --relocs mylib.o | egrep '(GOT|PLT|JU?MP_SLOT)'
000000000015  000f0000002a R_X86_64_REX_GOTP 0000000000000000 total_times - 4
000000000040  001000000004 R_X86_64_PLT32    0000000000000000 __ubsan_handle_ad[...] - 4
00000000004a  000f0000002a R_X86_64_REX_GOTP 0000000000000000 total_times - 4
```



### zig build

基本的命令 `zig build-exe`、`zig build-lib`、`zig build-obj` 和 `zig test` 通常已经足够。然而，有时项目需要另一层抽象来管理从源代码构建的复杂性。

hello.zig
```zig
const std = @import("std"); 
pub fn main() !void { 
	std.debug.print("Hello World!\n", .{}); 
}
```


build.zig
```zig
const std = @import("std");

pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{
        .name = "hello",
        .root_source_file = b.path("hello.zig"),
        .target = b.graph.host,
    });

    b.installArtifact(exe);

    const run_exe = b.addRunArtifact(exe);

    const run_step = b.step("run", "Run the application");
    run_step.dependOn(&run_exe.step);
}
```

可以使用专门的 build 脚本来完成复杂的项目构建. 这样方便管理构建的整个声明周期.

> zig build run --summary all


#### [zig build explained (3 Part Series)](https://zig.news/xq/series/2)

[1zig build explained - part 1](https://zig.news/xq/zig-build-explained-part-1-59lf "Published Jul 26 '21")
[2zig build explained - part 2](https://zig.news/xq/zig-build-explained-part-2-1850 "Published Aug 15 '21")
[3zig build explained - part 3](https://zig.news/xq/zig-build-explained-part-3-1ima "Published Nov 21 '21")

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


```c
#include <stdio.h>
int main(int argc, char **argv) 
{ 
	printf("Hello world\n"); 
	return 0; 
}
```


```bash
$ zig build-exe hello.c --library c 
$ ./hello  
Hello world
```

加入 --verbose-cc 可以查看真个编译的命令输出

```bash
zig build-exe hello.c --library c --verbose-cc 

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
$ zig build-exe hello.c --library c 
$ ./hello 
Hello world 
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
$ zig build-exe hello.c --library c -target x86_64-linux-musl   
$ ./hello Hello world 
$ ldd hello not a dynamic executable
```

[在此示例中，Zig 从源代码构建了 musl libc，然后链接到它。由于缓存系统](https://ziglang.org/download/0.4.0/release-notes.html#Build-Artifact-Caching)的存在，x86_64-linux 版本的 musl libc 仍然可用 ，因此，任何时候再次需要此 libc 时，它都可以立即使用。

这意味着此功能可在任何平台上使用。Windows 和 macOS 用户可以为上述任何目标平台构建 Zig 和 C 代码，并链接到 libc。同样，代码也可以针对其他架构进行交叉编译：

```bash
$ zig build-exe hello.c --library c -target aarch64v8-linux-gnu 
$ file hello 
hello: ELF 64-bit LSB executable, ARM aarch64, version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-aarch64.so.1, for GNU/Linux 2.0.0, with debug_info, not stripped
```


### $ORIGIN的缺陷

> gcc myprog.c  -Wl,-rpath='$ORIGIN'  -Wl,--enable-new-dtags -Wall  -lmylib -L.  -I.

'$ORIGIN' 会被解释为当前机器的绝对路径. 这样不方便移植.

runpath

```bash
$ gcc myprog.c -Wl,-rpath='$ORIGIN'  -Wall  -lmylib -L. -I. 
$ readelf -d a.out 

Dynamic section at offset 0x2d90 contains 29 entries:
  标记        类型                         名称/值
 0x0000000000000001 (NEEDED)             共享库：[libmylib.so]
 0x0000000000000001 (NEEDED)             共享库：[libc.so.6]
 0x000000000000001d (RUNPATH)            Library runpath: [$ORIGIN]
 0x000000000000000c (INIT)               0x1000
 0x000000000000000d (FINI)               0x128c
 ...
$ 
$ ldd a.out 
        linux-vdso.so.1 (0x00007ffc0cf3c000)
        libmylib.so => /home/ryefccd/docs/republic/langsrc/zig/c/./libmylib.so (0x0000716165c0d000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x0000716165800000)
        /lib64/ld-linux-x86-64.so.2 (0x0000716165c19000)
```

rpath
```bash
$ gcc myprog.c -Wl,-rpath='$ORIGIN' -Wl,--disable-new-dtags -Wall  -lmylib -L. -I. 
$ readelf -d a.out 

Dynamic section at offset 0x2d90 contains 29 entries:
  标记        类型                         名称/值
 0x0000000000000001 (NEEDED)             共享库：[libmylib.so]
 0x0000000000000001 (NEEDED)             共享库：[libc.so.6]
 0x000000000000000f (RPATH)              Library rpath: [$ORIGIN]
 0x000000000000000c (INIT)               0x1000
 0x000000000000000d (FINI)               0x128c
 ...
 
$ ldd a.out 
        linux-vdso.so.1 (0x00007fff90185000)
        libmylib.so => /home/ryefccd/docs/republic/langsrc/zig/c/./libmylib.so (0x000074f178f78000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x000074f178c00000)
        /lib64/ld-linux-x86-64.so.2 (0x000074f178f84000)

```
