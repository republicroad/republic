
## nginx static files


[nginx-static-file-serving-confusion-with-root-alias](https://stackoverflow.com/questions/10631933/nginx-static-file-serving-confusion-with-root-alias)
### root 

root 表示以此文件目录为web目录的根目录, uri匹配locaion就会在此根目录下进行文件寻找.

```
location /dirtest/ {
	root  /var/www/html/;  # 会把Location放到root路径之后进行文件查找.
	autoindex on;         # 访问此目录返回文件链接; 如果 autoindex off, 访问目录返回403
	index  index.html index.htm;
}
```

如果访问的uri是/dirtest/file.txt, 那么此uri匹配/dirtest/后，最后会在root目录下去寻找 dirtest/file.txt 文件.

	${root} + uri
	/var/www/html/ + /dirtest/file.txt
	/var/www/html//dirtest/file.txt -- > /var/www/html/dirtest/file.txt

### alias

alias表示虚拟目录, 一般用于location中配置, 把uri匹配location后的部分放在 alias 指定的目录中去寻找指定的文件. 虚拟目录本质上就是 uri 的 prefix目录存在与否不要紧, 匹配完location后剩下的就会去指定的目录寻找.

```
location /dirtest {
	alias /var/www/html/;  # 匹配此location, 剩余的路径在此目录进行寻找.
	autoindex on;         # 访问此目录返回文件链接; 如果 autoindex off, 访问目录返回403
	index  index.html index.htm;
}
```

如果访问的uri是/dirtest/f1.txt, 会去匹配location的 /dirtest

	/dirtest/f1.txt  - /dirtest = /f1.txt

剩下的/f1.txt就会和 alias 的部分拼接在一起(字符串拼接, 不是路径运算),

	/var/www/html/ + /f1.txt = /var/www/html//f1.txt
	/var/www/html//f1.txt --> /var/www/html/f1.txt(nginx最终的解释)

注意, uri匹配完location的后剩下的部分和alias拼成最后的访问路径, 所以, 注意如下 location带目录尾斜杠和 alias 目录不带尾斜杠的特殊情况.

```
location /dirtest/ {
	alias /var/www/html;  # 注意, 此目录最后未带尾斜杠.
	autoindex on;         # 访问此目录返回文件链接; 如果 autoindex off, 访问目录返回403
	index  index.html index.htm;
}
```

如果访问的uri是/dirtest/f2.txt, 会去匹配location的 /dirtest/

	/dirtest/f2.txt  - /dirtest/ = f2.txt
	/var/www/html + f2.txt = /var/www/htmlf2.txt

如果上面的 alias 配置是 /var/www/html, 那么最后的访问路径就是 /var/www/htmlf2.txt,
所以无论何时, 如果配置路径时文件夹(目录)，最好带上尾斜杠, 这样可以避免很多低级配置问题. 毕竟, nginx做的是字符串拼接, 而不是真正的目录操作.[目录配置](readme.md#目录配置)

### try_files


[```try_files```](https://serverfault.com/a/1035737)
[Understanding Nginx Try Files](https://fideloper.com/nginx-try-files)


## proxy_pass(反向代理)

### 是否携带uri(尾斜杠)


### 域名解析


[ngx_upstream_jdomain](https://github.com/nicholaschiasson/ngx_upstream_jdomain)




## 最佳实践


https://youtu.be/pkHQCPXaimU

Chapters:
##### 1.nginx介绍
[00:00:00](https://www.youtube.com/watch?v=pkHQCPXaimU&t=0s) Introduction
##### 2.什么是nginx
![](attach/Pasted%20image%2020240307141213.png)
[00:02:01](https://www.youtube.com/watch?v=pkHQCPXaimU&t=121s) What is NGINX? 
##### 3.nginx安装选项
![](attach/Pasted%20image%2020240307141837.png)
[00:06:22](https://www.youtube.com/watch?v=pkHQCPXaimU&t=382s) NGINX Installation Options   
##### 4.  Debian/Ubuntu 安装nginx
```shell
# 创建 /etc/sources.list.d/nginx.list 依据自己的系统架构替换OS、CODENAME
deb http://nginx.org/packages/mainline/OS/ CODENAME nginx
deb-src http://nginx.org/packages/mainline/OS/ CODENAME nginx

	OS: ubuntu /debian 
	CODENAME: 
		--debain: jessie /stretch
		--ubuntu: trusty / xenial / artful / bionic 
		# ubuntu系统代号： 14.04-trusty  16.04-xenial 17.10-artful 18.04-bionic 

wget http://nginx.org/keys/nginx_signing.key
apt-key add nginx_signing.key
apt-get update
apt-get install -y nginx
/etc/init.d/nginx start
``` 
![](attach/Pasted%20image%2020240307143916.png)
[00:08:23](https://www.youtube.com/watch?v=pkHQCPXaimU&t=503s) How to Install NGINX on Debian/Ubuntu   
##### 5. Centos/ Red Hat 安裝nginx
```shell
# 创建 /etc/yum.repos.d/nginx.repo
'''
[nginx]
name=nginx repo
baseurl=http://nginx.org/packages/mainline/OS/OSRELEASE/$basearch/
gpgcheck=0
enabled=1
'''
	OS-- rhel /centos
	OSRELEASE-- 6 或者 7 分别对应6.x 或者 7.x 版本

yum -y install nginx
systemctl enable nginx
systemctl start nginx
firewall-cmd --permanent --zone=public --add-port=80/tcp
firewall-cmd --reload

```
![](attach/Pasted%20image%2020240307144003.png)
[00:11:39](https://www.youtube.com/watch?v=pkHQCPXaimU&t=699s) How to Install NGINX on CentOS/Red Hat 
[00:13:55](https://www.youtube.com/watch?v=pkHQCPXaimU&t=835s) How to Install NGINX Plus   
##### 6.验证安装的nginx
```shell
nginx -v
ps -ef |grep nginx
```
![](attach/Pasted%20image%2020240307145355.png)
[00:14:38](https://www.youtube.com/watch?v=pkHQCPXaimU&t=878s) How to Verify Your NGINX Installation 
##### 7.nginx主要文件，目录和命令
nginx文件、目录
![](attach/Pasted%20image%2020240307145642.png)
[00:17:04](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1024s) NGINX Key Files, Commands and Directories 
nginx命令
``` shell
nginx -h    
nginx version: nginx/1.18.0 (Ubuntu)
Usage: nginx [-?hvVtTq] [-s signal] [-c filename] [-p prefix] [-g directives]
Options:
  -?,-h         : this help
  -v            : show version and exit
  -V            : show version and configure options then exit
  -t            : test configuration and exit
  -T            : test configuration, dump it and exit
  -q            : suppress non-error messages during configuration testing
  -s signal     : send signal to a master process: stop, quit, reopen, reload
  -p prefix     : set prefix path (default: /usr/share/nginx/)
  -c filename   : set configuration file (default: /etc/nginx/nginx.conf)
  -g directives : set global directives out of configuration file

```
![](attach/Pasted%20image%2020240307150110.png)
[00:18:58](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1138s) Key NGINX Commands
##### 8.nginx基础配置
[00:21:22](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1282s) Basic NGINX Configurations
```shell
'''
server {
	listen        80 default_server;
	server_name   www.example.com;

	return        200;
}
'''
server: 定义虚拟服务器内容
listen：指定nginx需要监听的ip/端口，没有ip意味着绑定了系统的所有ip
server_name: 指定虚拟服务的域名
return： 指定nginx直接响应请求
```
![](attach/Pasted%20image%2020240307150705.png)
[00:21:34](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1294s) Simple Virtual Server 
##### 9.web服务基础配置
```shell
'''
server {
	listen        80 default_server;
	server_name   www.example.com;

	location /i/ {
		root   /usr/share/nginx/html;
		# alias /usr/share/nginx/html;
		index  index.html index.htm;
	}
}
'''
index: www.example.com -> /usr/share/nginx/html/index.html
root: www.example.com/i/file.txt -> /usr/share/nginx/html/i/file.txt
alias: www.example.com/i/file.txt -> /usr/share/nginx/html/file.txt

```
![](attach/Pasted%20image%2020240307151519.png)
[00:24:38](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1478s) Basic Web Server Configuration 
##### 10.负载均衡基础配置
```shell
'''
upstream my_upstream {
	server server1.example.com;
	server server2.example.com;
	least_time;
}
server {
	location / {
		proxy_set_header  HOST $host;
		proxy_pass http://my_upstream;
	}
}
'''
upstream: 定义用于负载均衡的服务pool
默认的负载均衡策略为robin round
其他策略：
	least_conn：选择活连接数最少的服务器
	least_time: 连接计数和服务响应时间决定，只在nginx plus 中可用
proxy_pass: 将虚拟服务器链接到上游
默认情况下，nginx 将 HOST header重写为代理服务器的name和端口， proxy_set_header 覆盖并传递原始客户端的HOST header
```
![](attach/Pasted%20image%2020240307152046.png)[00:28:02](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1682s)Basic Load Balancing Configuration  
##### 11.反向代理基础配置
![](attach/Pasted%20image%2020240307153907.png)
[00:32:55](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1975s) Basic Reverse Proxy Configuration  
##### 12.nginx 缓存基础配置
```shell
'''
proxy_cache_path /path/to/cache levels=1:2
				keys_zone=my_cache:10m max_size=10g
				inactive=60m use_temp_path=off;
server {
	location / {
		proxy_cache my_cache;
		proxy_set_header  HOST $host;
		proxy_pass http://my_upstream;
	}
}
'''
proxy_cache_path: 指令来设置缓存的路径;
	path: 定义缓存存放的位置
	levels: 定义缓存路径的目录等级，最多3级

keys_zone：name表示共享内存名称, size表示共享内存大小，1mb大约可以存放8000个key；
max_size: 设置最大的缓存文件大小;

inactive：在inactive时间内没有被访问的缓存会被淘汰掉，默认是10分钟；

use_temp_path：如果为 off，则 nginx 会将缓存文件直接写入指定的 cache 文件中，而不使用 temp_path 指定的临时存储路径;

proxy_cache: 设置是否开启对后端响应的缓存;
  
```
![](attach/Pasted%20image%2020240307154129.png)
[00:35:22](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2122s) Basic Caching Configuration
##### 13.SSL基础配置
```shell
'''
server {
	listen        80 default_server;
	server_name   www.example.com;
	return  301 https://$server_name$request_uri;
}
server {
	listen  443  ssl default_server;
	server_name   www.example.com;
	ssl_certificate  cert.ctr;
	ssl_certificate_key cert.key;

	location / {
		root   /usr/share/nginx/html;
		index  index.html index.htm;
	}
}
'''
强制所有流量使用 SSL 有利于安全和 SEO（搜索引擎优化）

```
![](attach/Pasted%20image%2020240307155648.png)
[00:38:49](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2329s) Basic SSL Configuration  
##### 14.HTTP/2 基础配置
```shell
'''
server {
	listen  443  ssl http2 default_server;
	server_name   www.example.com;
	
	ssl_certificate  cert.ctr;
	ssl_certificate_key cert.key;
}
'''
http/2 提高了性能，几乎不需要对后端进行任何更改

```
![](attach/Pasted%20image%2020240307160553.png)
[00:40:45](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2445s)Basic HTTP/2 Configuration  
##### 15.在一个IP上复用多个站点
```shell
'''
server {
	listen        80 default_server;
	server_name   www.example1.com;
	# ...
}
server {
	listen        80;
	server_name   www.example2.com;
	# ...
}
server {
	listen        80;
	server_name   www.example3.com;
	# ...
}
'''
```
![](attach/Pasted%20image%2020240307161325.png)
[00:41:55](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2515s)Multiplexing Multiple Sites on One IP 
##### 16.7层反向代理
```shell
'''
server {
	location /service1 {
		proxy_pass http://my_upstream1;
	}

	location /service2 {
		proxy_pass http://my_upstream2;
	}

	location /service3 {
		proxy_pass http://my_upstream3;
	}
}
'''
```
![](attach/Pasted%20image%2020240307161926.png)
[00:43:39](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2619s) Layer 7 Request Routing  
##### 18.主nginx.conf文件配置
```shell
'''
user nginx; 
worker_processes auto;

# ...

http {
	# ...

	keepalive_timeout  300s;
	keepalive_requests 100000;
}
'''
 keepalive_timeout 来指定 KeepAlive 的超时时间（timeout）
 keepalive_requests指令用于设置一个keep-alive连接上可以服务的请求的最大数量，当最大请求数量达到时，连接被关闭。
```
![](attach/Pasted%20image%2020240307162701.png)
[00:45:23](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2723s) Modifications to main nginx.conf  
##### 19. HTTP/1.1 长连接
```shell
'''
upstream my_upstream {
	server server1.example.com;
	keepalive 32;
}
server {
	location / {
		proxy_set_header  HOST $host;
		proxy_http_vesion 1.1;
		proxy_set_header Connection "";
		
		proxy_pass http://my_upstream;
	}
}
'''

```
![](attach/Pasted%20image%2020240307165852.png)
[00:47:02](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2822s) HTTP/1.1 Keepalive to Upstreams  

##### 20. SSL session 缓存
```shell
'''
server {
	listen  443  ssl http2 default_server;
	server_name   www.example.com;
	
	ssl_certificate  cert.ctr;
	ssl_certificate_key cert.key;

	ssl_session_cache   shared:SSL:10m;
	ssl_session_timeout 10m;
}
'''
1MB可以存储大约4000个session
shared 所有worker之间共享缓存。
```

![](attach/Pasted%20image%2020240307171936.png)
[00:48:03](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2883s)SSL Session Caching 
##### 21.高级缓存设置
```shell
'''
proxy_cache_path /path/to/cache levels=1:2
				keys_zone=my_cache:10m max_size=10g
				inactive=60m use_temp_path=off;
server {
	location / {
		proxy_cache my_cache;
		proxy_cache_lock on;
		proxy_cache_revalidate on;
		proxy_cache_use_stale error timeout updating 
				http_500 http_502 http_503 http_504
		proxy_cache_backgroud_update on;

		proxy_set_header  HOST $host;
		proxy_pass http://my_upstream;
	}
}
'''
```
![](attach/Pasted%20image%2020240307173048.png)
[00:48:46](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2926s) Advanced Caching Configuration  
##### 22. gRPC 代理
```shell
'''
server {
	listen  443  ssl http2;
	ssl_certificate  cert.ctr;
	ssl_certificate_key cert.key;

	location / {
		grpc_pass grpc://localhost:50051
	}
}
'''
```
![](attach/Pasted%20image%2020240308102410.png)
[00:49:37](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2977s) gRPC Proxying with SSL Termination  
[00:50:53](https://www.youtube.com/watch?v=pkHQCPXaimU&t=3053s) ~~Active Health Checks(nginx plus)~~  
[00:52:29](https://www.youtube.com/watch?v=pkHQCPXaimU&t=3149s) ~~Sticky Cookie Session Persistence(nginx plus)~~  
##### 23.nginx Stub Status 
```shell
'''
server {
	location /basic_status {
		stub_status;
	}
}
'''
curl http://www.example.com/basic_status
```
![](attach/Pasted%20image%2020240308110530.png)
[00:53:57](https://www.youtube.com/watch?v=pkHQCPXaimU&t=3237s) NGINX Stub Status Module  
[00:54:45](https://www.youtube.com/watch?v=pkHQCPXaimU&t=3285s) ~~NGINX Plus Extended Status~~  
##### 24.nginx access logs
**![](attach/Pasted%20image%2020240308110750.png)**
[00:56:12](https://www.youtube.com/watch?v=pkHQCPXaimU&t=3372s) NGINX Access Logs  
[00:59:26](https://www.youtube.com/watch?v=pkHQCPXaimU&t=3566s) Q & A  


[Top 25 Nginx Web Server Best Security Practices](https://www.cyberciti.biz/tips/linux-unix-bsd-nginx-webserver-security.html)

### 目录配置

nginx中配置目录时, 目录字符串最后加上尾斜杠(/).