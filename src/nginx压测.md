### 测试服务器配置

|   |   |   |   |   |   |
|---|---|---|---|---|---|
|用途|公网IP|内网IP|硬件配置|软件配置|其他|
|应用服务器|101.126.32.237|10.0.1.10|cpu：Intel(R) Xeon(R) Platinum 8457C<br><br>内存：总内存15732，可用内存：14945|操作系统<br><br>No LSB modules are available.<br><br>Distributor ID: Ubuntu<br><br>Description: Ubuntu 22.04.1 LTS<br><br>Release: 22.04<br><br>Codename: jammy||
|测试客户端|101.126.7.78|10.0.1.11|cpu：Intel(R) Xeon(R) Platinum 8457C<br><br>内存：总内存15732，可用内存：14945|操作系统<br><br>No LSB modules are available.<br><br>Distributor ID: Ubuntu<br><br>Description: Ubuntu 22.04.1 LTS<br><br>Release: 22.04<br><br>Codename: jammy<br><br>  <br><br>工具<br><br>wrk||




### 服务端

| 软件名 | 版本 | github |
| ---- | ---- | ---- |
| openresty | 1.21.4.3 | https://github.com/openresty/openresty |

nginx配置文件
```
  

#user  nobody;

worker_processes auto;

  

error_log logs/error.log debug;

#error_log  logs/error.log  notice;

#error_log  logs/error.log  info;

  

#pid        logs/nginx.pid;

# nginx 进程打开的最多文件描述符数目，理论值应该是最多打开文件数（ulimit -n）与nginx 进程数相除，但是nginx 分配请求并不是那么均匀，所以最好与ulimit -n 的值保持一致

worker_rlimit_nofile 65535;

  

events {

    worker_connections 1024;

    use epoll;

    multi_accept on;

}

  
  

http {

    include mime.types;

    default_type application/octet-stream;

    resolver 8.8.8.8 114.114.114.114; # Google 的公共 DNS 服务器

    #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '

    #                  '$status $body_bytes_sent "$http_referer" '

    #                  '"$http_user_agent" "$http_x_forwarded_for"';

    access_log logs/access.log;

    sendfile on;

    #tcp_nopush     on;

    #keepalive_timeout  0;

    keepalive_timeout 65;

    #gzip  on;

    server {

        listen 80;

        server_name localhost;

  

        #charset koi8-r;

  

        #access_log  logs/host.access.log  main;

        # ##########################################

        # 在此处增加测试的接口，测试出lua代码的执行效率

        # ##########################################

        # 分阶段测试1,221125.24

        location /test/s1 {

            default_type text/html;

            add_header Content-Type 'text/html; charset=utf-8';

            return 200 "hello world";

        }

        # 分阶段测试2

        location /test/s2 {

            default_type 'text/html';

            add_header Content-Type 'text/html; charset=utf-8';

            content_by_lua 'hello world';

        }

        # 分阶段测试3

        location /test/s3 {

            default_type 'text/html';

            add_header Content-Type 'text/html; charset=utf-8';

            access_by_lua_block {

                ngx.say("hello world")

            }

        }

        # 分阶段测试打10行日志

        location /test/s4_1 {

            default_type 'text/html';

            add_header Content-Type 'text/html; charset=utf-8';

            # 循环十次打印日志

            access_by_lua_block {

                ngx.log(ngx.ERR, "access_by_lua_block:" )

                ngx.say("hello world")

            }

        }

        location /test/s4_2 {

            default_type 'text/html';

            add_header Content-Type 'text/html; charset=utf-8';

            # 十次打印日志

            access_by_lua_block {

                ngx.log(ngx.ERR, "access_by_lua_block:" )

                ngx.log(ngx.ERR, "access_by_lua_block:" )

                ngx.log(ngx.ERR, "access_by_lua_block:" .. )

                ngx.log(ngx.ERR, "access_by_lua_block:" )

                ngx.log(ngx.ERR, "access_by_lua_block:" )

                ngx.log(ngx.ERR, "access_by_lua_block:" )

                ngx.log(ngx.ERR, "access_by_lua_block:" )

                ngx.log(ngx.ERR, "access_by_lua_block:" )

                ngx.log(ngx.ERR, "access_by_lua_block:")

                ngx.log(ngx.ERR, "access_by_lua_block:" )

                ngx.say("hello world")

            }

        }

  

        location /test/s4_3 {

            default_type 'text/html';

            add_header Content-Type 'text/html; charset=utf-8';

            # 循环十次打印日志

            access_by_lua_block {

                local i=0

                while i<10

                do

                ngx.log(ngx.ERR, "access_by_lua_block:" .. tostring(i))

                i=i+1

                end

                ngx.say("hello world")

            }

        }

  

        # 分阶段测试md5测试

        # 获取一个时间，然后对时间进行md5的测试

        location /test/s5 {

            default_type 'text/html';

            add_header Content-Type 'text/html; charset=utf-8';

            access_by_lua_block {

                # ngx.update_time()

                # local et_t = ngx.now()*1000

                # local et_string = tostring(et_t)

                local et_string ="0"

                local et_string_md5 = ngx.md5(et_string)

                # ngx.log(ngx.ERR, "et_string_md5:" .. et_string_md5)

                ngx.say("hello world")

            }

        }

        # 获取一个时间，然后对时间进行md5的测试

        location /test/s6 {

            default_type 'text/html';

            add_header Content-Type 'text/html; charset=utf-8';

            access_by_lua_block {

                local resty_md5 = require "resty.md5"

                local str = require "resty.string"

  

                local md5 = resty_md5:new()

                local ok = md5:update("hel")

                local digest = md5:final()

                str.to_hex(digest)

  

                ngx.say("hello world")

            }

        }  
        # ##########################################

        location / {

            root html;

            index index.html index.htm;

        }

    }

    # ##########################################

}
```


### 测试端

| 测试软件 | 版本 | github地址 | 使用说明 |
| ---- | ---- | ---- | ---- |
| wrk |  | https://github.com/wg/wrk | https://zhuanlan.zhihu.com/p/613730055 |
测试软件 

```
/opt/wrk/wrk -t1 -c50  -d60s  http://10.0.1.10/test/s1
Running 1m test @ http://10.0.1.10/test/s1
  1 threads and 50 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   218.31us    3.13ms 211.01ms   99.92%
    Req/Sec   252.22k     2.57k  261.10k    70.72%
  15082637 requests in 1.00m, 2.87GB read
Requests/sec: 250960.75
Transfer/sec:     48.82MB


/opt/wrk/wrk -t1 -c50  -d60s  http://10.0.1.10/test/s2
Running 1m test @ http://10.0.1.10/test/s2
  1 threads and 50 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   362.30us  178.50us   8.13ms   72.60%
    Req/Sec    67.88k   691.57    69.57k    73.21%
  4058929 requests in 1.00m, 1.33GB read
  Non-2xx or 3xx responses: 4058929
Requests/sec:  67536.38
Transfer/sec:     22.61MB


/opt/wrk/wrk -t1 -c50  -d60s  http://10.0.1.10/test/s3
Running 1m test @ http://10.0.1.10/test/s3
  1 threads and 50 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   284.60us    4.26ms 212.02ms   99.84%
    Req/Sec   248.79k     3.54k  258.88k    87.19%
  14875631 requests in 1.00m, 3.09GB read
Requests/sec: 247515.25
Transfer/sec:     52.64MB


/opt/wrk/wrk -t1 -c50  -d60s  http://10.0.1.10/test/s4_1
Running 1m test @ http://10.0.1.10/test/s4_1
  1 threads and 50 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   821.83us    9.17ms 212.01ms   99.24%
    Req/Sec   245.02k    11.21k  254.24k    99.50%
  14622864 requests in 1.00m, 3.04GB read
Requests/sec: 243712.24
Transfer/sec:     51.83MB


/opt/wrk/wrk -t1 -c50  -d60s  http://10.0.1.10/test/s4_2
Running 1m test @ http://10.0.1.10/test/s4_2
  1 threads and 50 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   434.21us    1.76ms 201.87ms   99.77%
    Req/Sec    67.74k     2.34k   69.53k    99.33%
  4050701 requests in 1.00m, 1.32GB read
  Non-2xx or 3xx responses: 4050701
Requests/sec:  67399.49
Transfer/sec:     22.56MB


/opt/wrk/wrk -t1 -c50  -d60s  http://10.0.1.10/test/s4_3
Running 1m test @ http://10.0.1.10/test/s4_3
  1 threads and 50 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.38ms    3.80ms 212.97ms   98.88%
    Req/Sec    44.51k     2.26k   53.38k    96.67%
  2662010 requests in 1.00m, 566.12MB read
Requests/sec:  44293.33
Transfer/sec:      9.42MB


/opt/wrk/wrk -t1 -c50  -d60s  http://10.0.1.10/test/s5
Running 1m test @ http://10.0.1.10/test/s5
  1 threads and 50 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   439.83us    2.69ms 205.18ms   99.85%
    Req/Sec    67.85k     1.93k   69.83k    99.33%
  4050323 requests in 1.00m, 1.32GB read
  Non-2xx or 3xx responses: 4050323
Requests/sec:  67504.99
Transfer/sec:     22.60MB


/opt/wrk/wrk -t1 -c50  -d60s  http://10.0.1.10/test/s6
Running 1m test @ http://10.0.1.10/test/s6
  1 threads and 50 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   373.68us    5.20ms 212.08ms   99.72%
    Req/Sec   249.41k     6.29k  256.62k    99.00%
  14888230 requests in 1.00m, 3.09GB read
Requests/sec: 248135.40
Transfer/sec:     52.77MB
```




### 结论

| 返回形式 | Path | Qps | Avg | 最大响应时间 | 标准差 |
| ---- | ---- | ---- | ---- | ---- | ---- |
| return 200 "hello world"; | /test/s1 | 250960 | 218.31us | 211.01ms | 3.13ms |
| content_by_lua | /test/s2 | 67536 | 362.30us | 8.13ms | 178.50us |
| access_by_lua_block | /test/s3 | 247515.25 | 284.60us | 212.02ms | 4.26ms |
| access_by_lua_block for 循环十次打印日志 | /test/s4_1 | 243712.24 | 821.83us | 212.01ms | 9.17ms |
| access_by_lua_block 十次打印日志 | /test/s4_2 | 67399.49 | 434.21us | 201.87ms | 1.76ms |
| access_by_lua_block while 循环十次打印日志 | /test/s4_3 | 44293.33 | 1.38ms | 212.97ms | 3.80ms |
| ngx.md5 | /test/s5 | 67504.99 | 2.69ms | 2.69ms | 205.18ms |
| resty_md5 | /test/s6 | 248135.40 | 373.68us | 5.20ms | 212.08ms |

文件下载

![](attach/nginx.conf)