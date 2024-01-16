
# git tips


## windows git 


- 避免windows提醒文件的换行符. 这个只在windows系统上设置, 不能跨平台设置. 
	```
	git config --global core.autocrlf false
	```

- 给仓库配置 ssh 和 https 协议

		ssh 协议方便提交(设置无密码的公钥), https 协议方便设置代理(http.proxy)

```shell
$ git remote -v
origin  https://github.com/republicroad/republic.git (fetch)
origin  https://github.com/republicroad/republic.git (push)
originssh       git@github.com:RYefccd/republic.git (fetch)
originssh       git@github.com:RYefccd/republic.git (push)
```



## 常用技巧

- git 设置代理
```
设置代理
git config --global http.proxy http://192.168.1.201:1080/
git config --global http.proxy http://localhost:7890/
移除代理
git config --global --unset-all http.proxy
```

[Configure Git to use a proxy](https://gist.github.com/evantoli/f8c23a37eb3558ab8765)

- 测试 github 的是否连通

	ssh -vT git@github.com
	

- 设置git用户名


	git config --global user.email "xxxxx@xxx.com"
	git config --global user.name "XXXXX"
	
## cmd

- git clone
- git remote -v


## github

### git ssh connection timeout

遇到了 git on ssh 协议的连接超时, 如下图所示:
```bash
$ git pull originssh main
ssh: connect to host github.com port 22: Connection timed out
fatal: Could not read from remote repository.

Please make sure you have the correct access rights
and the repository exists.
```
解决方案如下:
1. 使用 https 协议
2. 设置 ssh config(~/.ssh/config)中增加如下配置. [解决方案](https://stackoverflow.com/a/52817036)
```
Host github.com
 Hostname ssh.github.com
 Port 443
```

使用 ssh -vt 命令调试


	ssh -vT git@github.com

```bash
$ ssh -vT git@github.com
OpenSSH_9.5p1, OpenSSL 3.1.4 24 Oct 2023
debug1: Reading configuration data /c/Users/RYefccd/.ssh/config
debug1: /c/Users/RYefccd/.ssh/config line 1: Applying options for github.com
debug1: Reading configuration data /etc/ssh/ssh_config
debug1: Connecting to ssh.github.com [20.205.243.160] port 443.
debug1: Connection established.
debug1: identity file /c/Users/RYefccd/.ssh/id_rsa type 0
debug1: identity file /c/Users/RYefccd/.ssh/id_rsa-cert type -1
debug1: identity file /c/Users/RYefccd/.ssh/id_ecdsa type -1
debug1: identity file /c/Users/RYefccd/.ssh/id_ecdsa-cert type -1
debug1: identity file /c/Users/RYefccd/.ssh/id_ecdsa_sk type -1
debug1: identity file /c/Users/RYefccd/.ssh/id_ecdsa_sk-cert type -1
debug1: identity file /c/Users/RYefccd/.ssh/id_ed25519 type -1
debug1: identity file /c/Users/RYefccd/.ssh/id_ed25519-cert type -1
debug1: identity file /c/Users/RYefccd/.ssh/id_ed25519_sk type -1
debug1: identity file /c/Users/RYefccd/.ssh/id_ed25519_sk-cert type -1
debug1: identity file /c/Users/RYefccd/.ssh/id_xmss type -1
debug1: identity file /c/Users/RYefccd/.ssh/id_xmss-cert type -1
debug1: identity file /c/Users/RYefccd/.ssh/id_dsa type -1
debug1: identity file /c/Users/RYefccd/.ssh/id_dsa-cert type -1
debug1: Local version string SSH-2.0-OpenSSH_9.5
debug1: Remote protocol version 2.0, remote software version babeld-8e18a363
debug1: compat_banner: no match: babeld-8e18a363
debug1: Authenticating to ssh.github.com:443 as 'git'
debug1: load_hostkeys: fopen /c/Users/RYefccd/.ssh/known_hosts2: No such file or directory
debug1: load_hostkeys: fopen /etc/ssh/ssh_known_hosts: No such file or directory
debug1: load_hostkeys: fopen /etc/ssh/ssh_known_hosts2: No such file or directory
debug1: SSH2_MSG_KEXINIT sent
debug1: SSH2_MSG_KEXINIT received
debug1: kex: algorithm: curve25519-sha256
debug1: kex: host key algorithm: ssh-ed25519
debug1: kex: server->client cipher: chacha20-poly1305@openssh.com MAC: <implicit> compression: none
debug1: kex: client->server cipher: chacha20-poly1305@openssh.com MAC: <implicit> compression: none
debug1: expecting SSH2_MSG_KEX_ECDH_REPLY
debug1: SSH2_MSG_KEX_ECDH_REPLY received
debug1: Server host key: ssh-ed25519 SHA256:+DiY3wvvV6TuJJhbpZisF/zLDA0zPMSvHdkr4UvCOqU
debug1: load_hostkeys: fopen /c/Users/RYefccd/.ssh/known_hosts2: No such file or directory
debug1: load_hostkeys: fopen /etc/ssh/ssh_known_hosts: No such file or directory
debug1: load_hostkeys: fopen /etc/ssh/ssh_known_hosts2: No such file or directory
debug1: checking without port identifier
debug1: load_hostkeys: fopen /c/Users/RYefccd/.ssh/known_hosts2: No such file or directory
debug1: load_hostkeys: fopen /etc/ssh/ssh_known_hosts: No such file or directory
debug1: load_hostkeys: fopen /etc/ssh/ssh_known_hosts2: No such file or directory
debug1: Host 'ssh.github.com' is known and matches the ED25519 host key.
debug1: Found key in /c/Users/RYefccd/.ssh/known_hosts:7
debug1: found matching key w/out port
debug1: check_host_key: hostkey not known or explicitly trusted: disabling UpdateHostkeys
debug1: rekey out after 134217728 blocks
debug1: SSH2_MSG_NEWKEYS sent
debug1: expecting SSH2_MSG_NEWKEYS
debug1: SSH2_MSG_NEWKEYS received
debug1: rekey in after 134217728 blocks
debug1: Will attempt key: /c/Users/RYefccd/.ssh/id_rsa RSA SHA256:zJbCtA07nSI9MjDqcN9EmtZH6ID6+o19Y2jcz80oQLQ
debug1: Will attempt key: /c/Users/RYefccd/.ssh/id_ecdsa
debug1: Will attempt key: /c/Users/RYefccd/.ssh/id_ecdsa_sk
debug1: Will attempt key: /c/Users/RYefccd/.ssh/id_ed25519
debug1: Will attempt key: /c/Users/RYefccd/.ssh/id_ed25519_sk
debug1: Will attempt key: /c/Users/RYefccd/.ssh/id_xmss
debug1: Will attempt key: /c/Users/RYefccd/.ssh/id_dsa
debug1: SSH2_MSG_EXT_INFO received
debug1: kex_input_ext_info: server-sig-algs=<ssh-ed25519-cert-v01@openssh.com,ecdsa-sha2-nistp521-cert-v01@openssh.com,ecdsa-sha2-nistp384-cert-v01@openssh.com,ecdsa-sha2-nistp256-cert-v01@openssh.com,sk-ssh-ed25519-cert-v01@openssh.com,sk-ecdsa-sha2-nistp256-cert-v01@openssh.com,rsa-sha2-512-cert-v01@openssh.com,rsa-sha2-256-cert-v01@openssh.com,ssh-rsa-cert-v01@openssh.com,sk-ssh-ed25519@openssh.com,sk-ecdsa-sha2-nistp256@openssh.com,ssh-ed25519,ecdsa-sha2-nistp521,ecdsa-sha2-nistp384,ecdsa-sha2-nistp256,rsa-sha2-512,rsa-sha2-256,ssh-rsa>
debug1: SSH2_MSG_SERVICE_ACCEPT received
debug1: Authentications that can continue: publickey
debug1: Next authentication method: publickey
debug1: Offering public key: /c/Users/RYefccd/.ssh/id_rsa RSA SHA256:zJbCtA07nSI9MjDqcN9EmtZH6ID6+o19Y2jcz80oQLQ
debug1: Server accepts key: /c/Users/RYefccd/.ssh/id_rsa RSA SHA256:zJbCtA07nSI9MjDqcN9EmtZH6ID6+o19Y2jcz80oQLQ
Authenticated to ssh.github.com ([20.205.243.160]:443) using "publickey".
debug1: channel 0: new session [client-session] (inactive timeout: 0)
debug1: Entering interactive session.
debug1: pledge: network
debug1: client_input_global_request: rtype hostkeys-00@openssh.com want_reply 0
debug1: pledge: fork
Hi RYefccd! You've successfully authenticated, but GitHub does not provide shell access.
debug1: client_input_channel_req: channel 0 rtype exit-status reply 0
debug1: channel 0: free: client-session, nchannels 1
Transferred: sent 2744, received 2840 bytes, in 0.7 seconds
Bytes per second: sent 3962.4, received 4101.0
debug1: Exit status 1
```


待验证的配置

```shell
Host github.com
User git
Hostname ssh.github.com
PreferredAuthentications publickey
IdentityFile ~/.ssh/id_rsa
Port 443

Host gitlab.com
Hostname altssh.gitlab.com
User git
Port 443
PreferredAuthentications publickey
IdentityFile ~/.ssh/id_rsa
```

[github和gitlab ssh 服务器域名](https://stackoverflow.com/a/55149904)