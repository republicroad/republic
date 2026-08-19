# RustFS 集群部署配置

## 概述

4 节点分布式集群配置，模拟真实分布式环境，用于测试高可用和数据分布。

## 架构

```
┌─────────────────────────────────────────────────────────────────┐
│                         Client                                  │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                          node0                                  │
│                  (S3 API: 9000)                                 │
└─────────────────────────────────────────────────────────────────┘
        │                   │                   │
        ▼                   ▼                   ▼
┌──────────────┐   ┌──────────────┐   ┌──────────────┐
│    node1     │   │    node2     │   │    node3     │
│ (S3: 9001)   │   │ (S3: 9002)   │   │ (S3: 9003)   │
└──────────────┘   └──────────────┘   └──────────────┘
```

## docker-compose.cluster.yaml

```yaml
version: "3.9"

services:
  # Node 0 - 主节点
  node0:
    image: rustfs/rustfs:latest
    container_name: node0
    hostname: node0
    environment:
      - RUSTFS_VOLUMES=http://node{0...3}:9000/data/rustfs{0...3}
      - RUSTFS_ADDRESS=0.0.0.0:9000
      - RUSTFS_CONSOLE_ENABLE=true
      - RUSTFS_ACCESS_KEY=${RUSTFS_ACCESS_KEY:-rustfsadmin-local}
      - RUSTFS_SECRET_KEY=${RUSTFS_SECRET_KEY:-rustfssecret-local}
      - RUSTFS_OBS_LOGGER_LEVEL=info
    ports:
      - "9000:9000"
    volumes:
      - node0_data:/data
    networks:
      - rustfs-cluster
    restart: unless-stopped
    healthcheck:
      test: ["CMD", "curl", "-f", "http://127.0.0.1:9000/health"]
      interval: 30s
      timeout: 10s
      retries: 3
      start_period: 40s

  # Node 1
  node1:
    image: rustfs/rustfs:latest
    container_name: node1
    hostname: node1
    environment:
      - RUSTFS_VOLUMES=http://node{0...3}:9000/data/rustfs{0...3}
      - RUSTFS_ADDRESS=0.0.0.0:9000
      - RUSTFS_CONSOLE_ENABLE=true
      - RUSTFS_ACCESS_KEY=${RUSTFS_ACCESS_KEY:-rustfsadmin-local}
      - RUSTFS_SECRET_KEY=${RUSTFS_SECRET_KEY:-rustfssecret-local}
      - RUSTFS_OBS_LOGGER_LEVEL=info
    ports:
      - "9001:9000"
    volumes:
      - node1_data:/data
    networks:
      - rustfs-cluster
    restart: unless-stopped
    healthcheck:
      test: ["CMD", "curl", "-f", "http://127.0.0.1:9000/health"]
      interval: 30s
      timeout: 10s
      retries: 3
      start_period: 40s

  # Node 2
  node2:
    image: rustfs/rustfs:latest
    container_name: node2
    hostname: node2
    environment:
      - RUSTFS_VOLUMES=http://node{0...3}:9000/data/rustfs{0...3}
      - RUSTFS_ADDRESS=0.0.0.0:9000
      - RUSTFS_CONSOLE_ENABLE=true
      - RUSTFS_ACCESS_KEY=${RUSTFS_ACCESS_KEY:-rustfsadmin-local}
      - RUSTFS_SECRET_KEY=${RUSTFS_SECRET_KEY:-rustfssecret-local}
      - RUSTFS_OBS_LOGGER_LEVEL=info
    ports:
      - "9002:9000"
    volumes:
      - node2_data:/data
    networks:
      - rustfs-cluster
    restart: unless-stopped
    healthcheck:
      test: ["CMD", "curl", "-f", "http://127.0.0.1:9000/health"]
      interval: 30s
      timeout: 10s
      retries: 3
      start_period: 40s

  # Node 3
  node3:
    image: rustfs/rustfs:latest
    container_name: node3
    hostname: node3
    environment:
      - RUSTFS_VOLUMES=http://node{0...3}:9000/data/rustfs{0...3}
      - RUSTFS_ADDRESS=0.0.0.0:9000
      - RUSTFS_CONSOLE_ENABLE=true
      - RUSTFS_ACCESS_KEY=${RUSTFS_ACCESS_KEY:-rustfsadmin-local}
      - RUSTFS_SECRET_KEY=${RUSTFS_SECRET_KEY:-rustfssecret-local}
      - RUSTFS_OBS_LOGGER_LEVEL=info
    ports:
      - "9003:9000"
    volumes:
      - node3_data:/data
    networks:
      - rustfs-cluster
    restart: unless-stopped
    healthcheck:
      test: ["CMD", "curl", "-f", "http://127.0.0.1:9000/health"]
      interval: 30s
      timeout: 10s
      retries: 3
      start_period: 40s

networks:
  rustfs-cluster:
    driver: bridge

volumes:
  node0_data:
  node1_data:
  node2_data:
  node3_data:
```

## 启动命令

```bash
# 设置凭证
export RUSTFS_ACCESS_KEY=your-access-key
export RUSTFS_SECRET_KEY=your-secret-key

# 启动集群
docker compose -f docker-compose.cluster.yaml up -d

# 查看集群状态
docker compose -f docker-compose.cluster.yaml ps

# 查看所有节点日志
docker compose -f docker-compose.cluster.yaml logs -f

# 查看单个节点日志
docker compose -f docker-compose.cluster.yaml logs node0
```

## 服务端口

| 节点 | 容器端口 | 宿主机端口 |
|------|----------|------------|
| node0 | 9000 | 9000 |
| node1 | 9000 | 9001 |
| node2 | 9000 | 9002 |
| node3 | 9000 | 9003 |

## 环境变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| `RUSTFS_ACCESS_KEY` | rustfsadmin-local | 访问密钥 |
| `RUSTFS_SECRET_KEY` | rustfssecret-local | 密钥 |
| `RUSTFS_VOLUMES` | http://node{0...3}:9000/data/rustfs{0...3} | 分布式存储卷 |

## 验证集群

```bash
# 检查所有节点健康状态
for port in 9000 9001 9002 9003; do
  echo "Node on port $port:"
  curl -f http://localhost:$port/health && echo " OK" || echo " FAILED"
done

# 使用 aws-cli 测试
export AWS_ACCESS_KEY_ID=${RUSTFS_ACCESS_KEY}
export AWS_SECRET_ACCESS_KEY=${RUSTFS_SECRET_KEY}

# 创建 bucket（通过任意节点）
aws --endpoint-url http://localhost:9000 s3 mb s3://test-cluster

# 上传文件
aws --endpoint-url http://localhost:9000 s3 cp ./file.txt s3://test-cluster/

# 列出所有 bucket
aws --endpoint-url http://localhost:9000 s3 ls
```

## 数据分布

RustFS 集群使用 Reed-Solomon 纠删码进行数据保护：

- **默认配置**: RS(4,2) - 4 个数据分片 + 2 个校验分片
- **容错能力**: 允许任意 2 个节点故障
- **存储效率**: 66.7% (4/6)

## 节点故障恢复

```bash
# 停止单个节点
docker compose -f docker-compose.cluster.yaml stop node2

# 重新启动
docker compose -f docker-compose.cluster.yaml start node2

# 完全重建集群（会丢失数据）
docker compose -f docker-compose.cluster.yaml down -v
docker compose -f docker-compose.cluster.yaml up -d
```

## 扩展集群

要扩展到更多节点，需要：

1. 添加新的服务定义
2. 更新 `RUSTFS_VOLUMES` 配置
3. 重新启动集群

```yaml
# 添加 node4 示例
node4:
  image: rustfs/rustfs:latest
  container_name: node4
  hostname: node4
  environment:
    - RUSTFS_VOLUMES=http://node{0...4}:9000/data/rustfs{0...4}
    # ... 其他配置
  ports:
    - "9004:9000"
  volumes:
    - node4_data:/data
  networks:
    - rustfs-cluster
```
