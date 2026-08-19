# RustFS 简单部署配置

## 概述

最小化的 Docker Compose 配置，适合快速启动单节点 RustFS 实例。

## docker-compose-simple.yml

```yaml
version: "3.9"

services:
  rustfs:
    image: rustfs/rustfs:latest
    container_name: rustfs-server
    security_opt:
      - "no-new-privileges:true"
    ports:
      - "9000:9000"   # S3 API
      - "9001:9001"   # Console UI
    environment:
      - RUSTFS_VOLUMES=/data/rustfs{0...3}
      - RUSTFS_ADDRESS=0.0.0.0:9000
      - RUSTFS_CONSOLE_ADDRESS=0.0.0.0:9001
      - RUSTFS_CONSOLE_ENABLE=true
      - RUSTFS_ACCESS_KEY=${RUSTFS_ACCESS_KEY:-rustfsadmin}
      - RUSTFS_SECRET_KEY=${RUSTFS_SECRET_KEY:-rustfsadmin}
      - RUSTFS_OBS_LOGGER_LEVEL=info
      - RUSTFS_UNSAFE_BYPASS_DISK_CHECK=${RUSTFS_UNSAFE_BYPASS_DISK_CHECK:-false}
    volumes:
      - rustfs_data_0:/data/rustfs0
      - rustfs_data_1:/data/rustfs1
      - rustfs_data_2:/data/rustfs2
      - rustfs_data_3:/data/rustfs3
      - logs:/app/logs
    networks:
      - rustfs-network
    restart: unless-stopped
    healthcheck:
      test: ["CMD", "sh", "-c", "curl -f http://127.0.0.1:9000/health && curl -f http://127.0.0.1:9001/rustfs/console/health"]
      interval: 30s
      timeout: 10s
      retries: 3
      start_period: 40s

  volume-permission-helper:
    image: alpine
    volumes:
      - rustfs_data_0:/data0
      - rustfs_data_1:/data1
      - rustfs_data_2:/data2
      - rustfs_data_3:/data3
      - logs:/logs
    command: >
      sh -c "
        chown -R 10001:10001 /data0 /data1 /data2 /data3 /logs &&
        echo 'Volume Permissions fixed'
      "
    restart: "no"

networks:
  rustfs-network:

volumes:
  rustfs_data_0:
  rustfs_data_1:
  rustfs_data_2:
  rustfs_data_3:
  logs:
```

## 启动命令

```bash
# 设置自定义凭证（推荐）
export RUSTFS_ACCESS_KEY=your-access-key
export RUSTFS_SECRET_KEY=your-secret-key

# 启动
docker compose -f docker-compose-simple.yml up -d

# 查看状态
docker compose -f docker-compose-simple.yml ps

# 停止
docker compose -f docker-compose-simple.yml down
```

## 服务端口

| 端口 | 用途 |
|------|------|
| 9000 | S3 API |
| 9001 | Console UI |

## 环境变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| `RUSTFS_ACCESS_KEY` | rustfsadmin | 访问密钥 |
| `RUSTFS_SECRET_KEY` | rustfsadmin | 密钥 |
| `RUSTFS_VOLUMES` | /data/rustfs{0...3} | 存储卷路径 |
| `RUSTFS_ADDRESS` | 0.0.0.0:9000 | S3 API 监听地址 |
| `RUSTFS_CONSOLE_ENABLE` | true | 启用 Console |
| `RUSTFS_OBS_LOGGER_LEVEL` | info | 日志级别 |
| `RUSTFS_UNSAFE_BYPASS_DISK_CHECK` | false | 跳过磁盘检查（仅测试用） |

## 验证部署

```bash
# 检查容器状态
docker ps --filter name=rustfs

# 检查健康状态
curl -f http://localhost:9000/health

# 访问 Console UI
open http://localhost:9001
```

## 使用 aws-cli 测试

```bash
# 配置 aws-cli
aws configure set aws_access_key_id ${RUSTFS_ACCESS_KEY}
aws configure set aws_secret_access_key ${RUSTFS_SECRET_KEY}

# 创建 bucket
aws --endpoint-url http://localhost:9000 s3 mb s3://test-bucket

# 上传文件
aws --endpoint-url http://localhost:9000 s3 cp ./file.txt s3://test-bucket/

# 列出 bucket
aws --endpoint-url http://localhost:9000 s3 ls
```
