
## haproxy slide window


### 代码结构

```bash
(.venv) ryefccd@republic:~/workspace/brde$ tree -L 2 conf/haproxy/
conf/haproxy/
├── haproxy.cfg            # haproxy 配置文件
├── haproxy_demo.cfg       # haproxy slide window 配置文件
├── readme.md              # 自述文件
└── sw_openapi             # haproxy 导出的 restful 接口的 openapi 文档
    ├── swagger-ui         # openapi 接口结构页面渲染js和css依赖文件
    ├── sw_func.html       # openapi(swagger) 页面入口
    └── sw_func.json       # restful 接口的spec定义文件

2 directory, 5 files

```


### 部署说明

将 haproxy.cfg  文件 和 sw_openapi 文件夹复制到 /etc/haproxy 文件夹中.  
**根据实际情况修改 haproxy 的监听端口**(bind :port).

```bash
frontend fe_api
    bind :88
    
    use_backend rate_10s   if { path /rate_10s }  #### \{ 与 path 之间要空格, uri 与 \} 之间也要保空格
    use_backend rate_1m    if { path /rate_1m  }
    use_backend rate_5m    if { path /rate_5m  }
    use_backend rate_1h    if { path /rate_1h  }
    use_backend rate_1d    if { path /rate_1d  }
    use_backend rate_7d    if { path /rate_7d  }
    ...

```


最后结构如下所示:  


```bash
root@ub20:~# tree -L 2 /etc/haproxy/
/etc/haproxy/
├── errors
│   ├── 400.http
│   ├── 403.http
│   ├── 408.http
│   ├── 500.http
│   ├── 502.http
│   ├── 503.http
│   └── 504.http
├── haproxy.cfg
└── sw_openapi
    ├── sw_func.html
    ├── sw_func.json
    └── swagger-ui
```

最后执行 `systemctl reload haproxy` 即可重载服务.



### 测试步骤


测试脚本:

```bash
# 在最近10s的窗口移动步长, 30s窗口长度之内做 fccdabc 值进行频率计数 
curl "http://127.0.0.1/rate_10s?mykey=fccdabc"
# 在最近1m的窗口移动步长, 3m窗口长度之内做 fccdabc 值进行频率计数 
curl "http://127.0.0.1/rate_1m?mykey=fccdabc"
# 在最近1h的窗口移动步长, 3h窗口长度之内做 fccdabc 值进行频率计数 
curl "http://127.0.0.1/rate_1h?mykey=fccdabc"

# 对mykey进行查看, 查看当前的剩余过期时间ttl和上次计数修改时间
curl -i -v -XGET "http://127.0.0.1/?mykey=fccdabc"
```

以后可以在修改数据前使用 haproxy 的变量把ttl和上次计数修改时间记录下来. 最后和当前的最新计数返回. 这些信息有助于去记录数据的分布.


压测命令:

```bash
wrk -t12 -c400 -d30s --latency "http://10.84.71.214/rate_10s?mykey=fccdabc"

# 对照组
wrk -t12 -c400 -d30s --latency "http://10.84.71.214/?mykey=fccdabc"
```


调试 stick table:

```bash
echo "show table rate_10s" | socat unix:/run/haproxy/admin.sock -

每隔一秒刷新 stick table 中的数据.
watch -n 1 'echo "show table rate_10s" | socat unix:/run/haproxy/admin.sock -'
```


## haproxy distinct count ratelimit

测试脚本:

```bash
curl "http://150.158.144.155:8888/group_distinct_1m?group=deviced1&distinct=fccd1"
...

curl "http://150.158.144.155:8888/group_distinct_1m?group=deviced1&distinct=fccd2"
...

curl "http://150.158.144.155:8888/group_distinct_1m?group=deviced1&distinct=fccd3"
...

curl "http://150.158.144.155:8888/group_distinct_1m?group=deviced1&distinct=fccd4"
...

curl "http://150.158.144.155:8888/group_distinct_1m?group=deviced1&distinct=fccd5"
...
```



```bash
watch -n 1 'echo "show table group_distinct_1m" | sudo socat unix:/run/haproxy/admin.sock -'

Every 1.0s: echo "show table group_distinct_1m" | sudo socat unix:/run/haproxy/admin.sock -                                                                                                              VM-16-16-ubuntu: Mon May 20 23:21:03 2024

# table: group_distinct_1m, type: string, size:1048576, used:6
0x56518c58ae10: key=deviced1 use=0 exp=130356 shard=0 gpc0=31 gpc0_rate(60000)=17 gpc1=5 gpc1_rate(60000)=2
0x56518c53a5f0: key=deviced1:fccd1 use=0 exp=93409 shard=0 gpc0=1 gpc0_rate(60000)=1 gpc1=0 gpc1_rate(60000)=0
0x56518c537d50: key=deviced1:fccd2 use=0 exp=97194 shard=0 gpc0=1 gpc0_rate(60000)=1 gpc1=0 gpc1_rate(60000)=0
0x56518c537eb0: key=deviced1:fccd3 use=0 exp=107137 shard=0 gpc0=9 gpc0_rate(60000)=6 gpc1=0 gpc1_rate(60000)=0
0x56518c5f4770: key=deviced1:fccd4 use=0 exp=117528 shard=0 gpc0=4 gpc0_rate(60000)=2 gpc1=0 gpc1_rate(60000)=0
0x56518c5f4ab0: key=deviced1:fccd5 use=0 exp=130356 shard=0 gpc0=16 gpc0_rate(60000)=10 gpc1=0 gpc1_rate(60000)=0
```


压测命令:

```bash
wrk -t12 -c400 -d30s --latency "http://150.158.144.155:8888/group_distinct_1m?group=deviced1&distinct=fccd1"
```

### roadmap

现在是使用 haproxy 来进行计数, 目前用的都是 inc, 如果以后 sum 的场景, 这个就需要 add 操作.  
[sc-add-gpc](https://www.haproxy.com/documentation/haproxy-configuration-manual/latest/#4-sc-add-gpc)  
[sc-inc-gpc](https://www.haproxy.com/documentation/haproxy-configuration-manual/latest/#4-sc-inc-gpc)  


#### sc-add-gpc

```shell
backend rolling_sum10s
    stick-table type binary len 16 size 1m expire 20s store gpc(1),gpc_rate(1,20s)   # ,gpc1,gpc1_rate(10s)
    acl is_post method POST

    # 在 sc-inc-gpc0 之前获取 ttl(expire) 和 idle 信息.

    http-request set-var(txn.mykey) url_param(mykey)
    http-request set-var(txn.num) url_param(num)
    http-request set-var(txn._mykey) var(txn.mykey),digest(md5)
    http-request set-var(txn.keyttl) var(txn._mykey),table_expire
    http-request set-var(txn.keyttl) int(0) unless { var(txn.keyttl) -m found }   ## 如果前面没有设置 txn.keyttl table 中没有这个记录, 是第一次出现, 这里进行初始化.
    http-request set-var(txn.keyidle) var(txn._mykey),table_idle
    http-request set-var(txn.keyidle) int(0) unless { var(txn.keyidle) -m found }  ## 如果前面没有设置 txn.keyttl table 中没有这个记录, 是第一次出现, 这里进行初始化.
    # stick-table  type string  size 1m  expire 30s  store gpc0,gpc0_rate(10s),gpc1,gpc1_rate(10s)
    # 在 sc-inc-gpc0 之后, table key 的 ttl 和 idle 时间会重置.
    #  track-scX 和 sc-inc-gpc0(X) 里面的 X 是 sc0, sc1, sc2 中的一个. 
    http-request track-sc0 var(txn._mykey) #if is_post  # track-sc0 会刷新 idle 和 ttl 的值.
    #http-request sc-inc-gpc0(0) if is_post  # sc-inc-xxx 对键对应的值进行累加
    http-request sc-add-gpc(0,0) int(10)  # if is_post  # var(txn.num)
    http-request set-var(txn.counter) var(txn._mykey),table_gpc(0,)    # table_gpc0
    http-request set-var(txn.counter) int(0) unless { var(txn.counter) -m found }
    http-request set-var(txn.rate) var(txn._mykey),table_gpc_rate(0,)
    http-request set-var(txn.rate) int(0) unless { var(txn.rate) -m found }
    http-request return status 200 content-type application/json lf-string '{"counter":%[var(txn.rate)],"mykey":"%[var(txn.mykey)]","ttl":%[var(txn.keyttl)],"idle":%[var(txn.keyidle)],"ip": "%[src]","port": %cp,"date":"%[date,utime(%Y-%m-%dT%H:%M:%S%z)]", "timestamp":"%[date]"}' hdr Access-Control-Allow-Origin "*"

```