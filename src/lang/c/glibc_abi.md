
# system_program

## libc

[C_standard_library Implementations](https://en.wikipedia.org/wiki/C_standard_library#Implementations)  
[Writing C software without the standard library](https://gist.github.com/tcoppex/443d1dd45f873d96260195d6431b0989)  
[forcing-elf-binary-to-use-another-libc-so](https://stackoverflow.com/questions/38590573/forcing-elf-binary-to-use-another-libc-so)  
[Multiple glibc libraries on a single host](https://stackoverflow.com/questions/847179/multiple-glibc-libraries-on-a-single-host)  
[how-to-run-new-software-without-updating-glibc](https://unix.stackexchange.com/questions/62940/how-to-run-new-software-without-updating-glibc)  
[how-to-chroot-to-provide-a-new-glibc-version-to-an-app](https://unix.stackexchange.com/questions/202966/how-to-chroot-to-provide-a-new-glibc-version-to-an-app)  
[glibc-improvements-and-what-to-expect-in-future-linux-distributions](https://www.linaro.org/blog/glibc-improvements-and-what-to-expect-in-future-linux-distributions/)  
https://news.ycombinator.com/item?id=29479769

### glibc backward compatibility

[how-the-gnu-c-library-handles-backward-compatibility](https://developers.redhat.com/blog/2019/08/01/how-the-gnu-c-library-handles-backward-compatibility)
[how-compatible-are-different-versions-of-glibc](https://stackoverflow.com/questions/11107263/how-compatible-are-different-versions-of-glibc)
[ABI_checker](https://sourceware.org/glibc/wiki/Testing/ABI_checker)
[Creating and using chroots and containers](https://docs.voidlinux.org/config/containers-and-vms/chroot.html#creating-and-using-chroots-and-containers)

[Dynamically load library from chroot with glibc dependency](https://stackoverflow.com/questions/49094718/dynamically-load-library-from-chroot-with-glibc-dependency)
### chroot

[通过 chroot 重新设置 root 密码](https://www.cnblogs.com/sparkdev/p/8556075.html)

### obtain libc version

#### ldd 

```
root@ub18:~# ldd --version  | head -n1
ldd (Ubuntu GLIBC 2.27-3ubuntu1.6) 2.27
```

#### features.h

```c
GCC_FEATURES=$(gcc -dM -E - <<< "#include <features.h>")

if grep -q __UCLIBC__ <<< "${GCC_FEATURES}"; then
    echo "uClibc"
    grep "#define __UCLIBC_MAJOR__" <<< "${GCC_FEATURES}"
    grep "#define __UCLIBC_MINOR__" <<< "${GCC_FEATURES}"
    grep "#define __UCLIBC_SUBLEVEL__" <<< "${GCC_FEATURES}"
elif grep -q __GLIBC__ <<< "${GCC_FEATURES}"; then
    echo "glibc"
    grep "#define __GLIBC__" <<< "${GCC_FEATURES}"
    grep "#define __GLIBC_MINOR__" <<< "${GCC_FEATURES}"
else
    echo "something else"
fi
```

#### /lib/x86_64-linux-gnu/libc.so.6

	/lib/x86_64-linux-gnu/libc.so.6


#### [How to update libc version on major Linux distros](https://linuxconfig.org/how-to-check-libc-library-version-on-debian-linux)

In case you find your installed `libc` to be out of date, it is simple enough to bring it up to date on any Linux system.
You can use the appropriate command below to update libc with your system’s [package manager](https://linuxconfig.org/comparison-of-major-linux-package-management-systems).
To update libc on [Ubuntu](https://linuxconfig.org/ubuntu-linux-download), [Debian](https://linuxconfig.org/debian-linux-download), and [Linux Mint](https://linuxconfig.org/linux-mint-download):

	$ sudo apt update
	$ sudo apt install libc-bin

To update libc on [Fedora](https://linuxconfig.org/fedora-linux-download), [CentOS](https://linuxconfig.org/centos-linux-download), [AlmaLinux](https://linuxconfig.org/almalinux-download), and [Red Hat](https://linuxconfig.org/red-hat-linux-download):

	$ sudo dnf install glibc

To update libc on [Arch Linux](https://linuxconfig.org/arch-linux-download) and [Manjaro](https://linuxconfig.org/manjaro-linux-download):
	
	$ sudo pacman -Syu glibc

### link to glibc version

[How can I specify the GLIBC version in cargo build for Rust?](https://stackoverflow.com/questions/57749127/how-can-i-specify-the-glibc-version-in-cargo-build-for-rust)
### static link

[static_linking_with_nim](https://www.reddit.com/r/programming/comments/2wk7q6/static_linking_with_nim/)

### glibc heap 

#### Arm Heap Exploitation

[UNDERSTANDING THE GLIBC HEAP IMPLEMENTATION](https://azeria-labs.com/heap-exploitation-part-1-understanding-the-glibc-heap-implementation/)


## asm

### x86_64

[System Calls On x86_64 from User Space](https://github.com/rishiba/doc_syscalls/blob/master/doc/05_calling_system_calls.rst)