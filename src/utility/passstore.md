
密码管理. 当有很多密码需要记忆和保存时, 可以利用此机制来生成随机密码并分类存储. 可以托管在自己的git私有仓库中. 方便在各处使用.

> docs:  
> [passwordstore](https://www.passwordstore.org/)  
> [GPG Keys Management](https://docs.fedoraproject.org/en-US/quick-docs/create-gpg-keys/#:~:text=The%20GPG%20key%20ID%20is,32%20bits%20of%20the%20fingerprint.)  
> https://www.gnupg.org/gph/en/manual/c235.html  
> [creating-a-new-gpg-key-with-subkeys](https://www.void.gr/kargig/blog/2013/12/02/creating-a-new-gpg-key-with-subkeys/)

## requirement

- gpupg
- pass

## install

	apt install pass
	apt install gnupg

## generatekey

生成公钥和私钥

	gpg --full-generate-key

查看公钥

```
root@ub20:~# gpg --list-keys --keyid-format LONG
/root/.gnupg/pubring.kbx
------------------------
pub   rsa3072/2548D4EA89DBA07C 2024-03-06 [SC] [expires: 2026-03-06]
      CCEEFE688CE457C3F747BEF62548D4EA89DBA07C
uid                 [ultimate] RYefccd <847960106@qq.com>
sub   rsa3072/F7CC34E8EADF8F4A 2024-03-06 [E] [expires: 2026-03-06]
```

查看私钥

```
root@ub20:~# gpg --list-secret-keys --keyid-format LONG
/root/.gnupg/pubring.kbx
------------------------
sec   rsa3072/2548D4EA89DBA07C 2024-03-06 [SC] [expires: 2026-03-06]
      CCEEFE688CE457C3F747BEF62548D4EA89DBA07C
uid                 [ultimate] RYefccd <847960106@qq.com>
ssb   rsa3072/F7CC34E8EADF8F4A 2024-03-06 [E] [expires: 2026-03-06]
```
sec 后面的 2548D4EA89DBA07C 就是私钥


> [!NOTE] gpg list explain
> 
> 1. `pub` -- **pub**lic primary key
> 2. `sub` -- public **sub**-key
> 3. `sec` -- **sec**ret primary key
> 4. `ssb` -- **s**ecret **s**u**b**-key
> 
>  [why-does-gnupg-create-4-separate-keys-and-what-does-sub-and-ssb-mean](https://crypto.stackexchange.com/a/66178)



## passstore(linux)

### 初始化密码存储

	pass init [uid]

### 查看密码存储

查看保存的密码:

	pass 或者 pass ls

### 记录密码

	pass insert myemail/gmail/xxxx@gmail.com


```
root@ub20:~# pass insert myemail/gmail/xxxx@gmail.com
mkdir: created directory '/root/.password-store/myemail'
mkdir: created directory '/root/.password-store/myemail/gmail'
Enter password for myemail/gmail/xxxx@gmail.com: 
Retype password for myemail/gmail/xxxx@gmail.com: 

root@ub20:~# pass
Password Store
└── myemail
    └── gmail
        └── xxxx@gmail.com
```


### 查看密码

	pass myemail/gmail/xxxx@gmail.com

输入gpg秘钥的保护密码后可以看到密码


### 复制密码到剪贴板

> [Could not copy data to the clipboard](https://askubuntu.com/a/1370797)  
> [enabling copy/ paste on ubuntu-server 18.04.4](https://askubuntu.com/questions/1296325/enabling-copy-paste-on-ubuntu-server-18-04-4)  
> [how-to-get-clipboard-support-on-a-linux-server-without-x11](https://unix.stackexchange.com/q/216347)

在桌面系统上执行下列命令会复制密码到剪贴板(xclip)

	pass -c myemail/gmail/xxxx@gmail.com

###  密码同步git

把 ~/.password-store/目录记录到git中

	pass git init

```
root@ub20:~# ll ~/.password-store/
total 28
drwx------  4 root root 4096 Mar  6 10:08 ./
drwx------ 10 root root 4096 Mar  6 10:09 ../
drwx------  8 root root 4096 Mar  6 10:08 .git/
-rw-------  1 root root   15 Mar  6 10:08 .gitattributes
-rw-------  1 root root    8 Mar  6 09:46 .gpg-id
drwx------  3 root root 4096 Mar  6 09:53 myemail/
```


## windows

> [pass-winmenu](https://github.com/geluk/pass-winmenu/tree/master)

![](https://camo.githubusercontent.com/38f77b9a896dfc29ed6481ba789885f61fa7e776515c807e2c48ea35761e90a2/68747470733a2f2f692e696d6775722e636f6d2f69686176654a742e676966)


安装完成(需要额外下载.net运行时依赖), 这个程序内置了gpg, 但是没有pass命令, 需要创建和passstore兼容的存储方式.

```
powershell> mkdir $HOME\.password-store
```

gpg生成秘钥, 或者[导入之前的gpg秘钥](https://github.com/geluk/pass-winmenu/tree/master?tab=readme-ov-file#accessing-an-existing-password-store-on-a-different-host).

gpg导入秘钥注意事项: 
![](attach/Pasted%20image%2020240308155958.png)



查看gpg key 的用户名或者email

```
PS C:\Users\fhj\.password-store> gpg --list-keys --keyid-format LONG C:/opt/pass-winmenu/lib/GnuPG/home/pubring.kbx 
---------------------------------------------- 
pub rsa2048/CD83D5B32C8CDBE5 2024-03-06 [SC] 3F05CB3B23702B75A636A207CD83D5B32C8CDBE5 
uid [ultimate] userxxx (xxx)f158273257xx@163.com
sub rsa2048/F303E2F36C2A99E3 2024-03-06 [E]
```


把uid部分中的email存入passstore的元信息中.

```
powershell> echo "f158273257xx@163.com" | Out-File -Encoding utf8 $HOME\.password-store\.gpg-id
```

结果如下:

```
PS C:\Users\fhj\.password-store> ls 

	目录: C:\Users\fhj\.password-store 
	
Mode LastWriteTime Length Name 
---- ------------- ------ ---- 
-a---- 2024/3/6 18:33 25 .gpg-id


PS C:\Users\fhj\.password-store> cat .\.gpg-id 
f158273257xx@163.com
```

$HOME\.password-store 文件中存储的密码就会使用  `f158273257xx@163.com`  对应的 gpg 秘钥来对密码文件进行加密和解密.

可以使用git仓库来进行密码版本追踪. 
```
PS C:\Users\fhj\.password-store> git init
```

在git上创建一个私有空仓库, 根据提示关联remote仓库.

```
git remote add origin https://gitee.com/xxxxx/pass_store.git
```

然后重启 `pass-winmenu.exe` 此程序就可以自动使用git来追踪密码文件(已经用gpg加密)的版本了.


## 资料

1. [management-password-store](https://www.redhat.com/sysadmin/management-password-store)
2. [wl-clipboard](https://askubuntu.com/a/1370797)
3. [index](https://git.zx2c4.com/) : [password-store](https://git.zx2c4.com/password-store/)
4. [password-store about](https://git.zx2c4.com/password-store/about/)



> [!NOTE] relative
> 
>**gpg2**(1), **tr**(1), **git**(1), **xclip**(1), **wl-clipboard**(1), **qrencode**(1).
[Accessing an existing password store on a different host](https://github.com/geluk/pass-winmenu/tree/master?tab=readme-ov-file#accessing-an-existing-password-store-on-a-different-host)


## git https 密码存储

### git credential

自动记忆 git https 下的用户名和密码.
[git-credential-manager install](https://github.com/git-ecosystem/git-credential-manager/blob/release/docs/install.md)

#### git credential manager

**Requires `gpg`, `pass`, and a GPG key pair.** 

	sudoapt install pass

- gpg --gen-key
- gpg --list-keys

```
ryefccd@republic:~$ gpg --list-key
/home/ryefccd/.gnupg/pubring.kbx
--------------------------------
pub   rsa3072 2024-02-19 [SC] [有效至：2024-02-22]
      4DF10999EDA5824AB897BB37AA2A445FFE116F56
uid             [ 绝对 ] ryefccd <847960106@qq.com>
sub   rsa3072 2024-02-19 [E] [有效至：2024-02-22]
```
- pass init [gpg uid]

输入上面的的 uid,  ryefccd 或者 ryefccd <847960106@qq.com>
```
pass init ryefccd
或者
pass init "ryefccd <847960106@qq.com>"
```

- download gcm-linux_amd64.2.4.1.deb    [下载地址:gcm-linux_amd64.2.4.1.deb](https://github.com/git-ecosystem/git-credential-manager/releases/download/v2.4.1/gcm-linux_amd64.2.4.1.deb)
- sudo dpkg -i gcm-linux_amd64.2.4.1.deb
- git config --global credential.credentialStore gpg
- git-credential-manager configure

最后通过 git config --global  -l 查看是否配置

```
...
credential.credentialstore=gpg
credential.helper=
credential.helper=/usr/local/bin/git-credential-manager
credential.https://dev.azure.com.usehttppath=true
...
```

##### pass
在一个git 仓库去执行 git pull, 触发git的密码认证, 输入密码后, 自动把密码记入 passstore 中.用下列命令可也查看密码.

pass git/https/github.com/RYefccd       查看密码.
pass -c git/https/github.com/RYefccd  复制密码至剪贴板.

[the standard unix password manager](https://www.passwordstore.org/)
https://gist.github.com/sgarciav/b709c871fb040e3444e044642e3d8709
https://github.com/git-ecosystem/git-credential-manager/blob/main/docs/credstores.md#gpgpass-compatible-files

#### credential.helper store

1.   scode第一次提交代码前先用git配置记忆用户名密码

	git config credential.helper store

2.  在 vscode 中使用 git 或者命令行中使用 git 推送即可. 第一次使用需要输入用户名和密码, 之后就会保存在 ~/.git-credentials 文件中. 明文存储用户名和密码. 这个不安全.默认格式如下:
	http://username:password@github.com

3.  在 gitlab 中申请 accesstokens
