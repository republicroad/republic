
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


## proxy_pass

### 是否携带uri(尾斜杠)


## 最佳实践


### 目录配置

nginx中配置目录时, 目录字符串最后加上尾斜杠(/).