
## c standard lib


### strftime

- utc




- gmt


    date_time = time.strftime("%a, %d %b %Y %H:%M:%S %Z", time.gmtime())  
    #output: date and time: Sun, 22 Sep 2019 10:01:53 GMT  
    #用于 cookie 的 expires 的设置