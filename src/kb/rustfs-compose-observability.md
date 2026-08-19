# RustFS 可观测性部署配置

## 概述

完整的可观测性配置，包含 Prometheus、Grafana、Tempo、Loki、Jaeger 和 OpenTelemetry Collector。

## 架构

```
┌─────────────────────────────────────────────────────────────┐
│                      RustFS Server                          │
│                   (S3 API + Console)                        │
└─────────────────────────────────────────────────────────────┘
                              │
                              │ OTLP (gRPC/HTTP)
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                 OpenTelemetry Collector                     │
│          (接收、处理、导出遥测数据)                            │
└─────────────────────────────────────────────────────────────┘
                    │           │           │
                    ▼           ▼           ▼
              ┌─────────┐ ┌─────────┐ ┌─────────┐
              │Prometheus│ │  Tempo  │ │  Loki   │
              │ (Metrics)│ │ (Traces)│ │ (Logs)  │
              └─────────┘ └─────────┘ └─────────┘
                    │           │           │
                    └───────────┼───────────┘
                                ▼
                        ┌─────────────┐
                        │   Grafana   │
                        │ (Dashboard) │
                        └─────────────┘
```

## docker-compose.yml

```yaml
version: "3.9"

services:
  # RustFS 主服务
  rustfs:
    image: rustfs/rustfs:latest
    container_name: rustfs-server
    security_opt:
      - "no-new-privileges:true"
    ports:
      - "9000:9000"   # S3 API
      - "9001:9001"   # Console
    environment:
      - RUSTFS_VOLUMES=/data/rustfs{0..3}
      - RUSTFS_ADDRESS=0.0.0.0:9000
      - RUSTFS_CONSOLE_ADDRESS=0.0.0.0:9001
      - RUSTFS_CONSOLE_ENABLE=true
      - RUSTFS_ACCESS_KEY=${RUSTFS_ACCESS_KEY:-rustfsadmin}
      - RUSTFS_SECRET_KEY=${RUSTFS_SECRET_KEY:-rustfsadmin}
      - RUSTFS_OBS_LOGGER_LEVEL=info
      - RUSTFS_OBS_ENDPOINT=http://otel-collector:4318
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
    depends_on:
      otel-collector:
        condition: service_started
        required: false

  # OpenTelemetry Collector
  otel-collector:
    image: otel/opentelemetry-collector-contrib:latest
    container_name: otel-collector
    command: ["--config=/etc/otelcol-contrib/otel-collector.yml"]
    volumes:
      - ./config/otel-collector.yml:/etc/otelcol-contrib/otel-collector.yml:ro
    ports:
      - "1888:1888"    # pprof
      - "8888:8888"    # Prometheus metrics
      - "8889:8889"    # Application metrics
      - "13133:13133"  # Health check
      - "4317:4317"    # OTLP gRPC
      - "4318:4318"    # OTLP HTTP
      - "55679:55679"  # zPages
    networks:
      - rustfs-network
    restart: unless-stopped
    depends_on:
      - tempo
      - jaeger
      - prometheus
      - loki

  # Prometheus (Metrics)
  prometheus:
    image: prom/prometheus:latest
    container_name: prometheus
    ports:
      - "9090:9090"
    volumes:
      - ./config/prometheus.yml:/etc/prometheus/prometheus.yml:ro
      - prometheus_data:/prometheus
    command:
      - "--config.file=/etc/prometheus/prometheus.yml"
      - "--storage.tsdb.path=/prometheus"
      - "--storage.tsdb.retention.time=30d"
      - "--web.enable-lifecycle"
      - "--web.enable-otlp-receiver"
      - "--web.enable-remote-write-receiver"
    networks:
      - rustfs-network
    restart: unless-stopped

  # Grafana (Dashboard)
  grafana:
    image: grafana/grafana:latest
    container_name: grafana
    ports:
      - "3000:3000"
    environment:
      - GF_SECURITY_ADMIN_USER=admin
      - GF_SECURITY_ADMIN_PASSWORD=admin
      - GF_INSTALL_PLUGINS=grafana-pyroscope-datasource
    volumes:
      - grafana_data:/var/lib/grafana
      - ./config/grafana/provisioning:/etc/grafana/provisioning:ro
      - ./config/grafana/dashboards:/etc/grafana/dashboards:ro
    networks:
      - rustfs-network
    restart: unless-stopped
    depends_on:
      - prometheus
      - tempo
      - loki

  # Tempo (Traces)
  tempo:
    image: grafana/tempo:2.10.5
    command: ["-config.file=/etc/tempo.yaml"]
    volumes:
      - ./config/tempo.yaml:/etc/tempo.yaml:ro
      - tempo_data:/var/tempo
    ports:
      - "3200:3200"    # Tempo API
      - "4317"         # OTLP gRPC
      - "4318"         # OTLP HTTP
    networks:
      - rustfs-network
    restart: unless-stopped

  # Loki (Logs)
  loki:
    image: grafana/loki:latest
    container_name: loki
    volumes:
      - ./config/loki.yaml:/etc/loki/local-config.yaml:ro
      - loki_data:/loki
    ports:
      - "3100:3100"
    command: ["-config.file=/etc/loki/local-config.yaml"]
    networks:
      - rustfs-network
    restart: unless-stopped

  # Jaeger (Distributed Tracing)
  jaeger:
    image: jaegertracing/jaeger:latest
    container_name: jaeger
    environment:
      - TZ=Asia/Shanghai
      - SPAN_STORAGE_TYPE=badger
      - BADGER_EPHEMERAL=false
      - BADGER_DIRECTORY_VALUE=/badger/data
      - BADGER_DIRECTORY_KEY=/badger/key
      - COLLECTOR_OTLP_ENABLED=true
    volumes:
      - jaeger_data:/badger
    ports:
      - "16686:16686"  # Jaeger UI
      - "14269:14269"  # Admin/Metrics
    networks:
      - rustfs-network
    restart: unless-stopped

networks:
  rustfs-network:

volumes:
  rustfs_data_0:
  rustfs_data_1:
  rustfs_data_2:
  rustfs_data_3:
  logs:
  prometheus_data:
  grafana_data:
  tempo_data:
  loki_data:
  jaeger_data:
```

## 配置文件

### config/otel-collector.yml

```yaml
receivers:
  otlp:
    protocols:
      grpc:
        endpoint: 0.0.0.0:4317
      http:
        endpoint: 0.0.0.0:4318

processors:
  batch:
    timeout: 1s
    send_batch_size: 1024

exporters:
  prometheus:
    endpoint: "0.0.0.0:8889"
    namespace: rustfs

  otlp/tempo:
    endpoint: tempo:4317
    tls:
      insecure: true

  loki:
    endpoint: http://loki:3100/loki/api/v1/push

  jaeger:
    endpoint: jaeger:14250
    tls:
      insecure: true

service:
  pipelines:
    metrics:
      receivers: [otlp]
      processors: [batch]
      exporters: [prometheus]
    traces:
      receivers: [otlp]
      processors: [batch]
      exporters: [otlp/tempo, jaeger]
    logs:
      receivers: [otlp]
      processors: [batch]
      exporters: [loki]
```

### config/prometheus.yml

```yaml
global:
  scrape_interval: 15s
  evaluation_interval: 15s

scrape_configs:
  - job_name: 'rustfs'
    static_configs:
      - targets: ['rustfs:9000']
        labels:
          service: rustfs

  - job_name: 'otel-collector'
    static_configs:
      - targets: ['otel-collector:8888']
        labels:
          service: otel-collector

  - job_name: 'prometheus'
    static_configs:
      - targets: ['localhost:9090']
        labels:
          service: prometheus
```

### config/tempo.yaml

```yaml
server:
  http_listen_port: 3200

distributor:
  receivers:
    otlp:
      protocols:
        grpc:
          endpoint: 0.0.0.0:4317
        http:
          endpoint: 0.0.0.0:4318

storage:
  trace:
    backend: local
    local:
      path: /var/tempo/traces
    wal:
      path: /var/tempo/wal
```

### config/loki.yaml

```yaml
auth_enabled: false

server:
  http_listen_port: 3100

common:
  path_prefix: /loki
  storage:
    filesystem:
      chunks_directory: /loki/chunks
      rules_directory: /loki/rules
  replication_factor: 1
  ring:
    kvstore:
      store: inmemory

schema_config:
  configs:
    - from: 2020-10-24
      store: boltdb-shipper
      object_store: filesystem
      schema: v11
      index:
        prefix: index_
        period: 24h
```

## 启动命令

```bash
# 仅启动 RustFS（不启动可观测性栈）
docker compose up -d rustfs

# 启动 RustFS + 可观测性栈
docker compose --profile observability up -d

# 查看所有服务
docker compose ps

# 查看日志
docker compose logs -f
```

## 服务端口

| 端口 | 服务 | 用途 |
|------|------|------|
| 9000 | RustFS | S3 API |
| 9001 | RustFS | Console UI |
| 3000 | Grafana | Dashboard |
| 9090 | Prometheus | Metrics UI |
| 16686 | Jaeger | Tracing UI |
| 4317 | OTLP | gRPC 接收 |
| 4318 | OTLP | HTTP 接收 |

## Grafana Dashboard

访问 http://localhost:3000

- 用户名: admin
- 密码: admin

## 监控指标

RustFS 暴露的关键指标：

- `rustfs_s3_requests_total` - S3 请求总数
- `rustfs_s3_request_duration_seconds` - S3 请求延迟
- `rustfs_disk_usage_bytes` - 磁盘使用量
- `rustfs_network_bytes_total` - 网络流量
