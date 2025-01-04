
# dbeaver

## 安装

https://dbeaver.io/download/  


按照如下步骤安装并配置即可.
1. 下载dbeaver社区版本.
	![](attach/img_v3_02i5_b72a95b3-fa7e-452e-8a84-52d7f4bfffeg.jpg)


2. 解压缩文件，打开可执行文件.
	![](attach/img_v3_02i5_f8892f61-2c53-4ffa-82b4-f1f29607688g.jpg)

3. 新建数据库连接.
	![](attach/img_v3_02i5_78fcebf3-b6c9-48ae-80c2-76a59f4a45eg.jpg)


4. 选择 duckdb.

	![](attach/img_v3_02i5_4c1c8177-b4c3-43a8-9a71-626f68c92dbg.jpg)

	![](attach/img_v3_02i5_20d28306-385b-4936-a279-797d7f26521g.jpg)


5. 填写连接名字

	![](attach/img_v3_02i5_9dc17029-3b61-48d1-879b-dd47e84477dg.jpg)


6. 编辑驱动

	![](attach/img_v3_02i5_0951a42c-094f-488e-8798-15e5517c3f5g.jpg)

	删除原来的默认配置(这些配置需要下载, 如果下载慢, 就删除这些默认配置手动导入 duckdb 的 jar 包即可)

	![](attach/Pasted%20image%2020250104104645.png)

	![](attach/img_v3_02i5_20cb16d9-9039-49e9-b715-09d2c585e71g.jpg)


	![](attach/img_v3_02i5_a7d226db-e954-4604-9796-5f835427fc3g.jpg)

https://duckdb.org/docs/installation/?version=stable&environment=java&download_method=direct
或者在brde中的梯队建设共享目录中去下载 [duckdb_jdbc-1.1.3.jar](https://geetest1024.feishu.cn/file/NPAybkZT8oZcpBxeoZvcRkgIn7g)



![](attach/img_v3_02i5_10e6e048-893d-47d1-8a03-dac6c1e30a7g.jpg)

![](attach/img_v3_02i5_9ea4b1e2-3351-44ba-b112-86887b3681dg.jpg)


![](attach/img_v3_02i5_555984c5-f59a-4d29-a887-1c4b2be4120g.jpg)


![](attach/img_v3_02i5_ce8138a5-a094-4c30-9f0d-f3a27c4d855g.jpg)


配置完成后，运行sql

![](attach/img_v3_02i5_2898b9db-f05b-412c-af07-789d026de49g.jpg)



![](attach/img_v3_02i5_b4ceefe9-778a-4dc7-ad8b-dce6d748c1eg.jpg)


```sql
select * from duckdb_databases();
SELECT * FROM duckdb_secrets();
SELECT * FROM duckdb_settings();
```


# sql-studio
