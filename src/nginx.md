
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
所以无论何时, 如果配置路径时文件夹(目录)，最好带上尾斜杠, 这样可以避免很多低级配置问题. 毕竟, nginx做的是字符串拼接, 而不是真正的目录操作.[目录配置](nginx.md#目录配置)

### try_files


[```try_files```](https://serverfault.com/a/1035737)
[Understanding Nginx Try Files](https://fideloper.com/nginx-try-files)


## proxy_pass(反向代理)

### 是否携带uri(尾斜杠)


## 最佳实践


https://youtu.be/pkHQCPXaimU

Chapters:
[00:00:00](https://www.youtube.com/watch?v=pkHQCPXaimU&t=0s) Introduction  
[00:02:01](https://www.youtube.com/watch?v=pkHQCPXaimU&t=121s) What is NGINX?   
[00:06:22](https://www.youtube.com/watch?v=pkHQCPXaimU&t=382s) NGINX Installation Options   
[00:08:23](https://www.youtube.com/watch?v=pkHQCPXaimU&t=503s) How to Install NGINX on Debian/Ubuntu   
[00:11:39](https://www.youtube.com/watch?v=pkHQCPXaimU&t=699s) How to Install NGINX on CentOS/Red Hat   
[00:13:55](https://www.youtube.com/watch?v=pkHQCPXaimU&t=835s) How to Install NGINX Plus   
[00:14:38](https://www.youtube.com/watch?v=pkHQCPXaimU&t=878s) How to Verify Your NGINX Installation   
[00:17:04](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1024s) NGINX Key Files, Commands and Directories  
[00:18:58](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1138s) Key NGINX Commands  
[00:21:22](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1282s) Basic NGINX Configurations  
[00:21:34](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1294s) Simple Virtual Server  
[00:24:38](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1478s) Basic Web Server Configuration  
[00:28:02](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1682s)Basic Load Balancing Configuration  
[00:32:55](https://www.youtube.com/watch?v=pkHQCPXaimU&t=1975s) Basic Reverse Proxy Configuration  
[00:35:22](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2122s) Basic Caching Configuration  
[00:38:49](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2329s) Basic SSL Configuration  
[00:40:45](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2445s)Basic HTTP/2 Configuration  
[00:41:55](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2515s)Multiplexing Multiple Sites on One IP  
[00:43:39](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2619s) Layer 7 Request Routing  
[00:45:23](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2723s) Modifications to main nginx.conf  
[00:47:02](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2822s) HTTP/1.1 Keepalive to Upstreams  
[00:48:03](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2883s)SSL Session Caching  
[00:48:46](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2926s) Advanced Caching Configuration  
[00:49:37](https://www.youtube.com/watch?v=pkHQCPXaimU&t=2977s) gRPC Proxying with SSL Termination  
[00:50:53](https://www.youtube.com/watch?v=pkHQCPXaimU&t=3053s) ~~Active Health Checks(nginx plus)~~  
[00:52:29](https://www.youtube.com/watch?v=pkHQCPXaimU&t=3149s) Sticky Cookie Session Persistence  
[00:53:57](https://www.youtube.com/watch?v=pkHQCPXaimU&t=3237s) NGINX Stub Status Module  
[00:54:45](https://www.youtube.com/watch?v=pkHQCPXaimU&t=3285s) NGINX Plus Extended Status  
[00:56:12](https://www.youtube.com/watch?v=pkHQCPXaimU&t=3372s) NGINX Access Logs  
[00:59:26](https://www.youtube.com/watch?v=pkHQCPXaimU&t=3566s) Q & A  


[Top 25 Nginx Web Server Best Security Practices](https://www.cyberciti.biz/tips/linux-unix-bsd-nginx-webserver-security.html)

### 目录配置

nginx中配置目录时, 目录字符串最后加上尾斜杠(/).