# haproxy ratelimit(stick table)

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
echo "show table rate_limit_table" | socat unix:/run/haproxy/admin.sock -

每隔一秒刷新 stick table 中的数据.
watch -n 1 'echo "show table rate_limit_table" | socat unix:/run/haproxy/admin.sock -'
```
