
## 技巧汇总


### git


- 避免windows提醒文件的换行符. 这个只在windows系统上设置, 不能跨平台设置. 
	```
	git config --global core.autocrlf false
	```


- 设置git 的账号信息, 才能使用 obsidian-git 插件来进行提交.

	![[./obsidian_git_tips01.png]]


- git 设置代理
```
git config --global http.proxy http://192.168.1.201:1080/
git config --global --unset-all http.proxy
```

测试 github 的联通

	ssh -v git@github.com


### markdown 本地图片

相对项目的根路径设置图片文件夹即可.

[nginx lua架构图](/imgs/2024-01-15/nginx_lua.jpg)

![nginx_lua架构图](/imgs/2024-01-15/nginx_lua.jpg)

![nginx_lua时序图](/imgs/2024-01-15/pytW3uzcsfDnEUCq.jpeg)

![nginx_lua时序图](nginx_lua2.jpg)

## 资料链接

[obsidian 插件](https://mdnice.com/writing/5caac9e9eb61489a959db5fd415e4d20)

[手动安装插件](https://www.youtube.com/watch?v=ffGfVBLDI_0)

[git_windows_crlf](https://stackoverflow.com/questions/17628305/windows-git-warning-lf-will-be-replaced-by-crlf-is-that-warning-tail-backwar)