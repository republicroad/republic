
# tcp 服务端与客户端示例

这是一个在linux和windows下都可以使用 gcc 编译 tcp 服务端和客户端示例.


编译服务端并运行:

```bash
# 编译服务端
gcc  -o server.exe  simple_server.c  -lWs2_32  # windows
gcc  -o server.exe  simple_server.c            # linux
# 运行服务端
$ ./server.exe

```


编译客户端并运行:

```bash
# 编译客户端
gcc  -o client.exe  simple_client.c  -lWs2_32  # windows
gcc  -o client.exe  simple_client.c            # linux
# 运行客户端
$ ./client.exe  

```




## shell技巧

只保留.c文件

```bash
shopt -s extglob
rm !(*.c|*.md)
```


## 问题示例

下面是错误示例.

    gcc  -o simples.exe -c simples.c  -lWs2_32

```shell
$ gcc  -o simples.exe -c simples.c  -lWs2_32
$ file simples.exe 
simples.exe: Intel amd64 COFF object file, no line number info, not stripped, 7 sections, symbol offset=0x50e, 30 symbols, 1st section name ".text"
```

gcc -o 只是表示输出的文件名字. -c 表示编译和汇编，但是不链接, 所以得到的不是可执行文件.


```shell
gcc --help

  -B <directory>           Add <directory> to the compiler's search paths.
  -v                       Display the programs invoked by the compiler.
  -###                     Like -v but options quoted and commands not executed.
  -E                       Preprocess only; do not compile, assemble or link.
  -S                       Compile only; do not assemble or link.
  -c                       Compile and assemble, but do not link.
  -o <file>                Place the output into <file>.
```

正确的编译后, -o 表示指定输出的文件名字. gcc 默认完成编译和链接, 如果使用 -c 进行编译和汇编得到的是 .o 文件，
最后还是需要将中间结果再次使用 gcc 编译为最终的可执行文件.

    gcc  -o simples.exe simples.c  -lWs2_32

```shell
$ gcc  -o simples.exe  simples.c  -lWs2_32
$ file simples.exe 
simples.exe: PE32+ executable (console) x86-64, for MS Windows, 20 sections
```