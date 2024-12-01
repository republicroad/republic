
# gcc 编译


    gcc  -o simples.exe -c simples.c  -lWs2_32

```shell
$ gcc  -o simples.exe -c simples.c  -lWs2_32
$ file simples.exe 
simples.exe: Intel amd64 COFF object file, no line number info, not stripped, 7 sections, symbol offset=0x50e, 30 symbols, 1st section name ".text"
```

    gcc  -o simples.exe simples.c  -lWs2_32

```shell
$ gcc  -o simples.exe  simples.c  -lWs2_32
$ file simples.exe 
simples.exe: PE32+ executable (console) x86-64, for MS Windows, 20 sections
```


```bash
gcc  -o simplec.exe  simplec.c  -lWs2_32
gcc  -o simples.exe  simples.c  -lWs2_32
```


只保留.c文件

```bash
shopt -s extglob
rm !(*.c|*.md)
```