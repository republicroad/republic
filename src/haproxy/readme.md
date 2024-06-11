# haproxy ratelimit(stick table)

haproxy配置文件:
[haproxy.cfg](haproxy_sw/haproxy.cfg)

另外一个文件测试链接:
![](haproxy_sw/haproxy.cfg)

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
