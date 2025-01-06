# Vector Configuration

vector 是一个集日志, metrics 功能于一身, 充当 agent, server和消费者多种角色于一体的数据搜集工具。可以用于搜集程序日志，搜集机器，容器运行指标，将结果进行转化处理后输出至下游对象存储，clickhose, influxdb, openobserve(es) 等系统的工具。

在我们的业务中，主要用来搜集程序运行日志，业务数据日志，以及机器监控指标的任务。尤其是在多个vpc内进行相关日志和指标的搜集，中继，持久化等任务。

## config files


### json日志

vector json日志配置文件

``` 
{{#include vector_data.yaml}} 
```


``` 
{{#include vector_data.service}} 
```

### 运行日志

vector 运行日志配置文件
``` 
{{#include vector_log.yaml}} 
```


``` 
{{#include vector_log.service}} 
```


## deploy best practice

### Create env file

### Create systemd service file

cp systemd service file to `/usr/lib/systemd/system/ ` dir.

```bash
sudo cp /home/ryefccd/.vector/etc/systemd/vector.service /usr/lib/systemd/system/
```


### Enable service

```bash
ryefccd@republic:~/.vector$ ls /lib/systemd/system |grep vector
vector_data.service
vector_log.service

systemctl list-unit-files
systemctl daemon-reload
systemctl list-unit-files

systemctl enable vector_data.service
systemctl enable vector_log.service
```


运行结果示例:  
```
systemctl daemon-reload
ryefccd@republic:~/.vector$ systemctl enable vector_data.service
Created symlink /etc/systemd/system/multi-user.target.wants/vector_data.service → /lib/systemd/system/vector_data.service.

ryefccd@republic:~/.vector$ systemctl enable vector_log.service
Created symlink /etc/systemd/system/multi-user.target.wants/vector_log.service → /lib/systemd/system/vector_log.service.
```


###  start

```
systemctl start vector_data.service

systemctl start vector_log.service
```

### stop

```
systemctl stop vector
```

### status

```
systemctl status vector
```

### Test service


## 资料

### [multiline-messages](https://vector.dev/docs/reference/configuration/sources/file/#multiline-messages)

运行日志中多行日志配置.