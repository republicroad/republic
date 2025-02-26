
# 网络命令

整理 linux 服务器中的用于查看网络设备，构建网络连接，查看连接状态，展示网络数据包等。这些命令一般用于排查网络连接信息，网络设备状态，一般在linux的命令行中使用。
## iproute2 

### ip

ip命令用来对网卡设备进行管理。
### ss

#### query socket connection 

On a web server it makes sense to see the open connections on HTTPS (port 443).

`ss -nt sport = :443`

To query multiple ports

`ss -nt '( sport = :443 or sport = :80 )'`

A slightly shorter version is by defining the side ‘src’ (source) or ‘dst’ (destination)

`ss -nt '( src :443 or src :80 )'`

#### By destination

To see active connections with a specific destination, define an expression including the IP address or address. For example to see connections on the 192.168.x.x network:

`ss dst 192.168/16`

#### kill connection 

`sudo ss -K -nt dst = 10.84.71.178`

可以用这个命令来替换 tcpkill 命令来模拟杀掉连接(RST包重置连接)

https://linux-audit.com/cheat-sheets/ss/  
https://www.cyberciti.biz/tips/linux-investigate-sockets-network-connections.html  

## socat

**The socat utility is a relay proxy for bidirectional data transfers between two independent data channels.**

There are many different types of channels that `socat`can be linked, including:

- Fillet
- pipe
- Devices (serial line, pseudo-terminal, etc)
- Socket (UNIX, IP4, IP6 - RAW, UDP, TCP)
- SSL socket
- CONNECT proxy connections
- File descriptors (stdin, etc.)
- The GNU line editor (readline)
- Programs
- Combinations of two of these



https://www.managedserver.eu/introduction-to-socat-a-multipurpose-forwarding-tool-for-linux/  
https://www.baeldung.com/linux/socat-command  
https://medium.com/@ria.banerjee005/a-guide-to-socat-for-penetration-testing-8b8db7b0458d  
https://gtfobins.github.io/gtfobins/socat/  

https://www.redhat.com/en/blog/getting-started-socat  
https://medium.com/@girish1729/11-socat-command-lines-you-can-copy-paste-into-your-shell-cb162cfc8076  
https://linuxcommandlibrary.com/man/socat  
https://www.query.ai/resources/blogs/creating-a-secure-encrypted-channel-with-socat/  
https://systemoverlord.com/2018/01/20/socat-as-a-handler-for-multiple-reverse-shells.html  
https://learntheshell.com/cheatsheets/socat/  
### socat as server

```bash
socat -v    TCP-LISTEN:5778,reuseaddr,fork EXEC:/bin/bash
```

```bash
socat -v    TCP-LISTEN:5778,reuseaddr,fork EXEC:date
```


### socat as client

```bash
socat  - TCP4:10.84.71.178:5778
```

## netcat


https://www.digitalocean.com/community/tutorials/how-to-use-netcat-to-establish-and-test-tcp-and-udp-connections#how-to-communicate-through-netcat  


## tcpdump

https://learntheshell.com/cheatsheets/tcpdump/  

# 资料

[Linux cheat sheets](https://linux-audit.com/cheat-sheets/  )  