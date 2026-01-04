# podman

## install



## image

> podman pull docker.1ms.run/library/postgres:latest

## container

> podman run --name pg16 -e POSTGRES_USER=postgres -e POSTGRES_PASSWORD=fccdjny -p 5432:5432 -d postgres

## exec
podman exec -it pg16 psql -U postgres


## podman desktop

[podman desktop download](https://podman-desktop.io/downloads)  


## 源

```json
{
  "registry-mirrors": ["https://mirror.gcr.io", "docker.1ms.run"]
}
```

[境内 Docker 镜像状态监控](https://status.anye.xyz/)  
[毫秒镜像 - 专为中国开发者提供的专业容器仓库服务](https://1ms.run/)  

## 参考

###  wsl2 proxy 设置

在windows 中 C:\Users\${USER} 创建 .wslconfig 文件并写入以下配置识别系统代理
```ini
[experimental]
autoMemoryReclaim=gradual
networkingMode=mirrored
dnsTunneling=true
firewall=true
autoProxy=true

```