
# system_program

## libc

[C_standard_library Implementations](https://en.wikipedia.org/wiki/C_standard_library#Implementations)
[Writing C software without the standard library](https://gist.github.com/tcoppex/443d1dd45f873d96260195d6431b0989)
[forcing-elf-binary-to-use-another-libc-so](https://stackoverflow.com/questions/38590573/forcing-elf-binary-to-use-another-libc-so)
[Multiple glibc libraries on a single host](https://stackoverflow.com/questions/847179/multiple-glibc-libraries-on-a-single-host)

### obtain libc version


#### ldd 

```
root@ub18:~# ldd --version  | head -n1
ldd (Ubuntu GLIBC 2.27-3ubuntu1.6) 2.27
```


#### 