# DuckDB 分析 TB 级数据完全指南

> 整理时间：2026-08-20

---

## 目录

1. [最佳实践](#1-最佳实践)
2. [机器规格要求](#2-机器规格要求)
3. [阿里云机器费用](#3-阿里云机器费用)
4. [MaxCompute vs DuckDB+OSS 对比](#4-maxcompute-vs-duckdboss-对比)
5. [DuckDB 扩展处理 PB 级数据](#5-duckdb-扩展处理-pb-级数据)
6. [分布式计算：Shuffle 原理与扩展方案](#6-分布式计算shuffle-原理与扩展方案)

---

## 1. 最佳实践

### 1.1 数据格式与存储

- **使用 Parquet 格式**（非 CSV）：列式存储 + 压缩，扫描速度快 10-15x
- **分区存储**：按日期/类别分区，启用 `hive_partitioning = true`，实现分区裁剪
- **Parquet 文件大小 128MB~1GB**，Row Group 设置 100K~1M 行

```sql
COPY (SELECT * FROM raw) TO 'output/'
(FORMAT PARQUET, PARTITION_BY (dt), ROWS_PER_GROUP 100000);
```

### 1.2 内存与线程调优

```sql
SET memory_limit = '24GB';           -- 70-80% 系统内存
SET threads = 4;                     -- 物理核心数的一半（避免超线程竞争）
SET temp_directory = '/mnt/ssd/tmp'; -- spill 必须用 SSD
SET preserve_insertion_order = false; -- 大数据导入必须关闭
```

关键：**线程数减少 → 单线程内存增加 → 减少 spill to disk**，实测可从 47min 降到 9min。

### 1.3 Parquet 读取优化

```sql
SET parquet_file_reader_count = 2;    -- 减少 reader 争抢 page cache
SET parquet_prefetch_mode = true;     -- 异步预取下一个 row group
SET force_compression = 'zstd';       -- 中间结果压缩比 snappy 好 2-3x
```

### 1.4 查询优化

- **用 `EXPLAIN ANALYZE`** 定位瓶颈，关注 cardinality 估计偏差
- **避免 `SELECT *`**，只选需要的列
- **避免 `count(DISTINCT)`**，改用多级聚合

```sql
-- 差：直接 count(DISTINCT session_id) → 巨大 hash table
-- 好：先 GROUP BY 聚合到 session 级，再聚合到 user 级
CREATE TABLE sessions AS
SELECT user_id, session_id, count(*) AS page_depth,
       bool_or(page_depth = 1) AS is_bounce
FROM clicks WHERE ts >= current_date - 30
GROUP BY user_id, session_id;

SELECT user_id, sum(page_depth) AS pv, count(*) AS sessions,
       round(sum(CASE WHEN is_bounce THEN 1 ELSE 0 END)::FLOAT / count(*), 4) AS bounce_rate
FROM sessions GROUP BY user_id;
```

- **过滤条件直接作用于分区列**，避免 `WHERE EXTRACT(YEAR FROM dt) = 2026`
- 用 `ANALYZE` 更新统计信息，防止优化器选错 join 策略

### 1.5 超内存处理 (Out-of-Core)

DuckDB 原生支持 spill to disk，但多个 blocking operator（JOIN + GROUP BY + ORDER BY）同时出现仍可能 OOM。建议：

- 拆分复杂查询为多步
- 避免 `list()`、`string_agg()` 等不支持 spill 的聚合
- 中间结果落盘为 Parquet 再读取

### 1.6 远程文件读取 (S3/GCS)

```sql
SET threads = CPU_CORES * 2~5;  -- 远程 IO 场景可超配线程
-- 只选列 + 加过滤，减少网络传输
```

### 1.7 数据有序性

排序后的数据 → zonemap 跳过更多 row group → 扫描量大幅减少。实测排序后扫描行数从 280M 降到 35M（89% pruning）。

### 1.8 性能提升汇总

| 优化项 | 预期提升 |
|--------|---------|
| CSV → Parquet | 5-15x |
| 线程/内存调优 | 5x |
| Parquet 参数优化 | 1.6x |
| 多级聚合替代 DISTINCT | 5-8x |
| 排序物化表 | 3-5x |
| **累计** | **~150x** |

---

## 2. 机器规格要求

### 2.1 最低可用配置

| 资源 | 推荐 | 说明 |
|------|------|------|
| **CPU** | 8-16 核 | DuckDB 单节点并行，核心数直接决定扫描速度 |
| **内存** | 32-64 GB | 内存越大 spill to disk 越少，性能越稳定 |
| **磁盘** | NVMe SSD ≥ 1TB | spill 临时文件必须用 SSD，HDD 随机写会拖慢 2-5x |
| **文件系统** | XFS (Linux) | 大文件性能最优 |

### 2.2 TB 级数据的理想配置

| 资源 | 推荐 |
|------|------|
| **CPU** | 32-64 核 |
| **内存** | 128-256 GB |
| **磁盘** | 2TB+ NVMe SSD |
| **网络** | 10Gbps+（如读取远程 S3） |

### 2.3 关键原则

DuckDB 是单节点引擎，不像 Spark 可以水平扩展。TB 级分析的核心瓶颈：

1. **内存 > CPU > 磁盘速度**
   - 内存决定能否避免 spill，是第一优先级
   - 32GB 内存可处理 TB 级 Parquet（列式 + 分区 + 过滤后实际扫描量远小于原始大小）
   - 如果是全表扫描无过滤，需要更多内存

2. **SSD 是硬性要求**
   - DuckDB 的 out-of-core 机制依赖 spill to disk
   - temp_directory 必须指向 SSD，否则性能骤降

3. **线程数不要设太高**
   - 实测设为物理核心数的一半效果最好（8核设4线程）
   - 更多线程 = 每线程内存更少 = 更多 spill

### 2.4 实际场景参考

| 场景 | 原始数据量 | 实际扫描量 | 32GB 内存够吗 |
|------|-----------|-----------|-------------|
| 分区裁剪 + 过滤 | 10TB | 100-500GB | 够 |
| Parquet 列裁剪 | 5TB | 500GB-1TB | 基本够 |
| 全表聚合无过滤 | 2TB | 2TB | 需要 64GB+ |

### 2.5 一句话总结

**32GB 内存 + 8核 + NVMe SSD 是 TB 级分析的起步线**。查询有良好的分区和过滤，16GB 内存也能跑；全表扫描建议 64GB+。

---

## 3. 阿里云机器费用

### 3.1 推荐实例：通用型 g9i（ecs.g9i.2xlarge）

| 计费方式 | 价格 |
|---------|------|
| **按量付费** | ~¥1.99/小时（约 ¥1,430/月） |
| **包年包月 1年** | ~¥7,552/年（约 ¥629/月） |
| **包年包月 2年** | ~¥5,268/年（约 ¥439/月） |
| **包年包月 3年** | ~¥3,665/年（约 ¥305/月） |

### 3.2 存储费用（额外）

| 磁盘类型 | 规格 | 包月价格 |
|---------|------|---------|
| ESSD 云盘 | 20GB（系统盘） | ~¥40/月 |
| ESSD 云盘（数据盘） | 500GB | ~¥500/月 |
| ESSD 云盘（数据盘） | 1TB | ~¥1,000/月 |

### 3.3 总费用估算（含 500GB 数据盘）

| 方案 | 月费 | 年费 |
|------|------|------|
| 按量付费 | ~¥1,930 | ~¥23,160 |
| 包年 1年 | ~¥1,130 | ~¥13,552 |
| 包年 3年 | ~¥805 | ~¥9,665 |

### 3.4 省钱建议

1. **用活动价**：阿里云经常有新人/企业活动，g9i 8核32G 活动价可低至 **¥5,000/年**
2. **数据放 OSS/S3**：数据存对象存储（~¥0.12/GB/月），DuckDB 通过 `httpfs` 直接查 Parquet，省掉大磁盘费用
3. **弹性使用**：非 7x24 运行的分析任务用按量付费，跑完释放
4. **抢占式实例**：中断容错的批处理任务可用抢占式，价格约按量的 **1-2 折**

### 3.5 对比 Spark 集群

| 方案 | 月费 | 适合场景 |
|------|------|---------|
| 单台 g9i 8核32G | ~¥630/月 | TB 级 Parquet 分析（分区+过滤后） |
| 3节点 Spark 集群 | ~¥3,000+/月 | 同等数据量，但运维复杂 |

**结论**：TB 级 DuckDB 分析，一台 8核32G 的 g9i 包年约 **¥630/月**，性价比远高于 Spark 集群。

---

## 4. MaxCompute vs DuckDB+OSS 对比

### 4.1 架构差异

| 维度 | MaxCompute | DuckDB + OSS |
|------|-----------|--------------|
| **架构** | 全托管分布式数仓（Serverless） | 嵌入式单节点引擎 + 对象存储 |
| **扩展方式** | 弹性 CU 自动扩缩 | 单机垂直扩展，数据放 OSS 水平存储 |
| **运维** | 零运维 | 需自建实例、管理调度 |
| **数据格式** | MaxCompute 内部列式（专有） | Parquet/CSV/JSON 开放格式 |
| **SQL 能力** | 完整 SQL + UDF + MapReduce + Spark | 标准 SQL，无 UDF 扩展 |

### 4.2 成本对比（以处理 1TB 数据/天为例）

| 费用项 | MaxCompute（按量） | MaxCompute（包月 50CU） | DuckDB + ECS g9i |
|--------|-------------------|----------------------|------------------|
| **计算** | ~¥300/天 × 30 = ¥9,000/月 | ¥7,500/月（50CU） | ¥630/月（8核32G 包年） |
| **存储** | ¥0.12/GB/月 ≈ ¥120/TB/月 | ¥120/TB/月 | OSS ¥0.12/GB/月 ≈ ¥120/TB/月 |
| **SQL 复杂度附加** | 复杂度系数 × 输入量 × ¥0.3/GB | 无 | 无 |
| **网络下载** | 外网下载收费 | 外网下载收费 | OSS 内网免费 |
| **月总成本** | **¥9,120+** | **¥7,620** | **¥750** |

### 4.3 性能对比

| 场景 | MaxCompute | DuckDB + OSS |
|------|-----------|--------------|
| **1TB 全表扫描聚合** | 10-30min（取决于 CU） | 5-15min（8核32G，分区裁剪后） |
| **100GB 分区查询** | 1-5min | 10-60s（Parquet 分区裁剪） |
| **即席查询响应** | 秒级（有 Query Acceleration） | 秒级（本地）/ 10s+（远程 OSS） |
| **并发能力** | 数百并发 | 单连接，适合 1-5 并发 |
| **超大数据集** | PB 级原生支持 | TB 级优秀，PB 级受限于单机 |

### 4.4 适用场景

| 场景 | 推荐方案 | 原因 |
|------|---------|------|
| **T+1 离线批处理（ETL）** | MaxCompute | 成熟调度、容错、DataWorks 集成 |
| **Ad-hoc 即席分析** | DuckDB + OSS | 成本低 10x，响应快，无需预购 CU |
| **实时看板** | MaxCompute + Hologres | DuckDB 单机并发不够 |
| **数据量 < 5TB，团队 < 10人** | DuckDB + OSS | 性价比碾压 |
| **数据量 > 100TB，多团队协作** | MaxCompute | 分布式、权限、审计、调度 |
| **临时分析/数据探索** | DuckDB + OSS | 零成本启动，会 SQL 即可 |
| **生产级数仓（需 SLA）** | MaxCompute | 99.9% 可用性，自动备份容灾 |

### 4.5 核心优劣势

**MaxCompute 优势**
- PB 级数据原生支持，自动扩缩
- DataWorks 调度 + 血缘 + 权限管理
- 多租户、企业级安全合规
- Spot 闲时算力价格低（¥0.1/GB）

**MaxCompute 劣势**
- 贵：1TB/天查询月费 ¥9,000+
- 锁定阿里云，迁移成本高
- 配置复杂，CU 规划需经验
- Ad-hoc 查询不够灵活

**DuckDB + OSS 优势**
- 便宜：同等场景成本约为 MaxCompute 的 **1/10**
- 开放：Parquet 格式，随时迁移到 Spark/Trino
- 简单：会 SQL 就能用，半天上线
- 灵活：本地 + 云端无缝切换

**DuckDB + OSS 劣势**
- 单节点，无法水平扩展
- 无原生调度和任务管理
- 并发有限，不适合多用户同时查询
- 需自建监控、备份、高可用

### 4.6 选型决策树

```
数据分析场景选型决策树：

数据量 > 50TB？
  ├─ 是 → MaxCompute（包月预留 CU）
  └─ 否 ↓

需要多团队并发查询？
  ├─ 是 → MaxCompute（按量）
  └─ 否 ↓

有现成 ECS/服务器？
  ├─ 是 → DuckDB + OSS（首选）
  └─ 否 → 轻量应用服务器 38元/年起

预算 > ¥5,000/月？
  ├─ 是 → MaxCompute + Hologres
  └─ 否 → DuckDB + OSS
```

**一句话总结**：中小规模 TB 级分析选 DuckDB+OSS（成本低 10x），大规模 PB 级生产数仓选 MaxCompute（运维省心）。两者不互斥——OSS 上的 Parquet 数据可以同时被两者读取。

---

## 5. DuckDB 扩展处理 PB 级数据

### 5.1 方案全景

```
PB 级数据
    │
    ├── 1. Smallpond（DeepSeek 开源）← 最成熟方案
    ├── 2. DuckLake（DuckDB 官方湖仓格式）
    ├── 3. Quack（DuckDB 网络协议）
    ├── 4. OpenDuck（开源 MotherDuck 替代）
    └── 5. 自建分片 + OSS 直查
```

### 5.2 Smallpond（推荐首选）

DeepSeek 开源，5000+ star，PB 级验证过的方案。

**架构**：DuckDB（每节点计算引擎）+ 3FS（分布式文件系统）+ 调度器

```
┌──────────┐  ┌──────────┐  ┌──────────┐
│ Node 1   │  │ Node 2   │  │ Node N   │
│ DuckDB   │  │ DuckDB   │  │ DuckDB   │
│ 16C/64G  │  │ 16C/64G  │  │ 16C/64G  │
└────┬─────┘  └────┬─────┘  └────┬─────┘
     │              │              │
     └──────────────┼──────────────┘
                    │
            ┌───────▼───────┐
            │  3FS / OSS    │
            │  共享存储层    │
            └───────────────┘
```

**核心特点**：
- 无需 Shuffle，数据本地化调度
- 单节点 DuckDB 性能比 Spark 快 5-10x
- 纯 SQL，学习成本极低
- 实测：**110 TiB 数据，50 节点，30 分钟处理完**（吞吐 3.66 TiB/min）

**集群规格**：

| 组件 | 最低 | 推荐 |
|------|------|------|
| 计算节点 | 4C/8G | 16C/64G |
| 存储节点 | 4C/8G + 4TB NVMe | 16C/64G + 20TB NVMe |
| 网络 | 10GbE | 25GbE / InfiniBand |
| 节点数 | 3 | 10-50 |

**适用规模**：1TB ~ 100TB（甜点区间），100TB+ 也可用

### 5.3 DuckLake（DuckDB 官方湖仓格式）

v1.0 已发布，生产就绪。

**架构**：SQL 目录（PostgreSQL/SQLite）+ Parquet 数据文件（S3/OSS）

```sql
-- 一行接入
ATTACH 'ducklake:postgres://host/db' AS lake;
CREATE TABLE lake.events (id INT, ts TIMESTAMP, ...);
-- 标准 SQL 读写，数据自动存为 Parquet
SELECT * FROM lake.events WHERE dt = '2026-08-20';
```

**核心特点**：
- 元数据存在 PostgreSQL，数据存 Parquet
- 多个 DuckDB 实例可同时读写同一个 DuckLake
- ACID 事务、时间旅行、快照隔离
- 兼容 Iceberg/Delta 生态

**适用场景**：多团队共享数据湖，替代专有数仓格式

### 5.4 Quack（DuckDB 网络协议）

让 DuckDB 进程之间互相查询。

```sql
-- Node A 上执行
ATTACH 'quack://node-b:8523/mydb' AS remote_db;
SELECT * FROM remote_db.big_table;  -- 透明远程查询
```

**核心特点**：
- 同一份 SQL、类型系统、格式
- 可水平扩展为读副本集群
- 适合构建 DuckDB 微服务网格

**适用场景**：多节点读分离，DuckDB 之间联邦查询

### 5.5 OpenDuck（开源 MotherDuck 替代）

```sql
ATTACH 'openduck:mydb' AS db;
-- 混合执行：查询自动拆分到本地 + 远程
SELECT * FROM db.huge_table JOIN local_table ON ...;
```

**核心特点**：
- 差分存储（追加层 + 快照隔离）
- 混合执行（单查询跨本地和远程）
- 开源，可自托管

### 5.6 自建分片（最简单方案）

不依赖任何框架，手动分片 + OSS 直查。

```sql
-- 数据按 hash 分片存 OSS
-- shard_0/data.parquet, shard_1/data.parquet, ...

CREATE VIEW events AS
SELECT * FROM read_parquet('s3://bucket/shard_*/events/*.parquet',
    hive_partitioning = true);

SELECT region, count(*) FROM events
WHERE dt = '2026-08-01'
GROUP BY region;
```

### 5.7 方案对比

| 维度 | Smallpond | DuckLake | Quack | 自建分片 |
|------|-----------|----------|-------|---------|
| **成熟度** | 生产验证 | v1.0 GA | 早期 | 成熟 |
| **学习成本** | 低（纯 SQL） | 低 | 中 | 低 |
| **运维复杂度** | 中（需 3FS） | 低 | 中 | 低 |
| **最大规模** | PB 级 | TB-PB 级 | TB 级 | 取决于分片数 |
| **写入支持** | 有限 | 完整 ACID | 单写者 | 只读为主 |
| **并发查询** | 多节点并行 | 多实例共享 | 多连接 | 单节点 |
| **依赖** | 3FS + Ray | PostgreSQL | 无 | 无 |

### 5.8 选型建议

```
数据规模决策：

< 1TB        → 单机 DuckDB（最简）
1-100TB      → Smallpond（首选）或 DuckLake
100TB-1PB    → Smallpond + 3FS 集群
> 1PB        → Smallpond 或考虑 Spark/Flink

团队能力决策：

会 SQL，不会运维   → DuckLake + PostgreSQL 目录
会 SQL，会运维     → Smallpond
有 Spark 经验      → Smallpond（迁移成本最低）
纯 Python 团队     → Smallpond（Python API）
```

**Smallpond 是目前 PB 级 DuckDB 最成熟的方案**——DeepSeek 用它处理 110TiB 训练数据，50 节点 30 分钟完成，比 Spark 快 1.5-2x。如果不想依赖 3FS，DuckLake + PostgreSQL 是更轻量的多节点共享方案。

---

## 6. 分布式计算：Shuffle 原理与扩展方案

### 6.1 Shuffle 是什么

Shuffle 是分布式计算中 **按 key 重新分布数据** 的过程。发生在 `GROUP BY`、`JOIN`、`DISTINCT`、`ORDER BY` 等需要跨分区重分布数据的操作。

```
Map 端 (分区并行处理)              Reduce 端 (按 key 合并)
┌──────────────┐                  ┌──────────────┐
│ Partition 0  │ ──shuffle──→    │ Reduce 0     │
│ (Alice:100)  │   (网络传输)     │ (Alice:500)  │
├──────────────┤                  ├──────────────┤
│ Partition 1  │ ──shuffle──→    │ Reduce 1     │
│ (Bob:200)    │                  │ (Bob:250)    │
├──────────────┤                  ├──────────────┤
│ Partition 2  │ ──shuffle──→    │ Reduce 2     │
│ (Charlie:300)│                  │ (Charlie:300)│
└──────────────┘                  └──────────────┘
```

### 6.2 Shuffle 完整流程（以 Spark 为例）

```
阶段1: Map 端（每个分区独立处理）
┌─────────────────────────────────────────┐
│  Task 0 (分区0的数据)                    │
│  ┌─────────────┐                        │
│  │ 读取数据     │                        │
│  │    ↓        │                        │
│  │ 本地聚合     │  ← 部分聚合（combiner）│
│  │    ↓        │                        │
│  │ 按 key hash  │  ← hash(key) % N      │
│  │    ↓        │                        │
│  │ 分成 N 个桶  │  ──写入本地磁盘──→     │
│  └─────────────┘                        │
├─────────────────────────────────────────┤
│  Task 1 (分区1的数据)                    │
│  ┌─────────────┐                        │
│  │ 读取数据     │                        │
│  │    ↓        │                        │
│  │ 本地聚合     │                        │
│  │    ↓        │                        │
│  │ 按 key hash  │                        │
│  │    ↓        │                        │
│  │ 分成 N 个桶  │  ──写入本地磁盘──→     │
│  └─────────────┘                        │
└─────────────────────────────────────────┘
              │
              ▼
阶段2: Shuffle 写（Map 端 → 磁盘）
┌─────────────────────────────────────────┐
│  Map Task 0 磁盘文件:                    │
│  ├── bucket_0.data (给 Reduce Task 0)   │
│  ├── bucket_1.data (给 Reduce Task 1)   │
│  └── bucket_2.data (给 Reduce Task 2)   │
│                                          │
│  Map Task 1 磁盘文件:                    │
│  ├── bucket_0.data (给 Reduce Task 0)   │
│  ├── bucket_1.data (给 Reduce Task 1)   │
│  └── bucket_2.data (给 Reduce Task 2)   │
└─────────────────────────────────────────┘
              │
              ▼
阶段3: Shuffle 读（磁盘 → 网络 → Reduce 端）
┌─────────────────────────────────────────┐
│  Reduce Task 0:                         │
│  ├── 从 Map Task 0 拉取 bucket_0        │
│  ├── 从 Map Task 1 拉取 bucket_0        │
│  ├── 合并所有 bucket_0                   │
│  └── 输出结果                            │
│                                          │
│  Reduce Task 1:                         │
│  ├── 从 Map Task 0 拉取 bucket_1        │
│  ├── 从 Map Task 1 拉取 bucket_1        │
│  ├── 合并所有 bucket_1                   │
│  └── 输出结果                            │
└─────────────────────────────────────────┘
```

#### Step 1: Map 端分区

```python
# Spark 内部逻辑
def map_side_shuffle(partition_id, data):
    # 1. 读取当前分区的数据
    records = read_partition(partition_id)  # [(key1, val1), (key2, val2), ...]
    
    # 2. 按 key 的 hash 值分成 N 个桶
    buckets = defaultdict(list)
    for key, value in records:
        bucket_id = hash(key) % num_reducers  # 决定去哪个 Reduce
        buckets[bucket_id].append((key, value))
    
    # 3. 每个桶写到本地磁盘
    for bucket_id, records in buckets.items():
        write_to_disk(f"shuffle_{partition_id}_{bucket_id}.data", records)
```

#### Step 2: Shuffle 写（序列化 + 溢写）

```
内存中的数据结构:
┌─────────────────────────────┐
│ Partition 0 的数据           │
│ ┌─────────────────────────┐ │
│ │ Hash Table (内存)       │ │
│ │ key1 → [val1, val2]    │ │
│ │ key2 → [val3]          │ │
│ │ key3 → [val4, val5]    │ │
│ └─────────────────────────┘ │
│            │                 │
│     内存不足时溢写到磁盘      │
│            ↓                 │
│ ┌─────────────────────────┐ │
│ │ 磁盘文件 (Spill File)   │ │
│ │ - 序列化为二进制         │ │
│ │ - 压缩 (Snappy/LZ4)    │ │
│ │ - 写入本地 SSD          │ │
│ └─────────────────────────┘ │
└─────────────────────────────┘
```

#### Step 3: Shuffle 读（网络传输）

```
Reduce Task 0 的读取过程:
┌─────────────────────────────────────────┐
│  1. 获取 Map 输出位置                      │
│     - 从 Driver 获取所有 Map Task 的      │
│       shuffle 文件位置                     │
│                                          │
│  2. 建立网络连接                           │
│     - 连接到 Map Task 0, 1, 2, ...       │
│                                          │
│  3. 拉取数据 (HTTP GET)                   │
│     - GET /shuffle/map_0/bucket_0        │
│     - GET /shuffle/map_1/bucket_0        │
│     - GET /shuffle/map_2/bucket_0        │
│                                          │
│  4. 合并 + 排序                           │
│     - 合并所有 Map 的 bucket_0            │
│     - 按 key 排序（如果需要）              │
│                                          │
│  5. 写入 Reduce 端内存/磁盘               │
└─────────────────────────────────────────┘
```

#### Step 4: Reduce 端处理

```python
def reduce_side_shuffle(bucket_id):
    # 1. 从所有 Map Task 拉取同一个 bucket 的数据
    all_records = []
    for map_id in range(num_mappers):
        records = fetch_from_map(map_id, bucket_id)
        all_records.extend(records)
    
    # 2. 合并相同 key 的值
    merged = defaultdict(list)
    for key, value in all_records:
        merged[key].append(value)
    
    # 3. 执行最终聚合
    for key, values in merged.items():
        result = aggregate(values)  # SUM, COUNT, AVG 等
        yield (key, result)
```

#### SQL 例子

```sql
-- 输入：3个分区的数据
-- Partition 0: [(Alice, 100), (Bob, 200)]
-- Partition 1: [(Alice, 150), (Charlie, 300)]
-- Partition 2: [(Bob, 50), (Alice, 250)]

SELECT name, SUM(amount)
FROM sales
GROUP BY name;
```

```
Map 端 (3个 Task 并行):
┌──────────────────┐  ┌──────────────────┐  ┌──────────────────┐
│ Partition 0      │  │ Partition 1      │  │ Partition 2      │
│ Alice → 100      │  │ Alice → 150      │  │ Bob → 50         │
│ Bob → 200        │  │ Charlie → 300    │  │ Alice → 250      │
│                  │  │                  │  │                  │
│ hash(Alice) % 2  │  │ hash(Alice) % 2  │  │ hash(Bob) % 2    │
│ = 0 → bucket 0   │  │ = 0 → bucket 0   │  │ = 1 → bucket 1   │
│                  │  │                  │  │                  │
│ hash(Bob) % 2    │  │ hash(Charlie)%2  │  │ hash(Alice) % 2  │
│ = 1 → bucket 1   │  │ = 1 → bucket 1   │  │ = 0 → bucket 0   │
└──────────────────┘  └──────────────────┘  └──────────────────┘
         │                    │                    │
         └────────────────────┼────────────────────┘
                              ▼
Shuffle 写到磁盘:
Map 0: bucket_0=[(Alice,100)], bucket_1=[(Bob,200)]
Map 1: bucket_0=[(Alice,150)], bucket_1=[(Charlie,300)]
Map 2: bucket_0=[(Alice,250)], bucket_1=[(Bob,50)]
                              │
                              ▼
网络传输 (Reduce Task 拉取):
┌─────────────────────┐     ┌─────────────────────┐
│ Reduce Task 0       │     │ Reduce Task 1       │
│ (处理 bucket 0)     │     │ (处理 bucket 1)     │
│                     │     │                     │
│ 从 Map 0 拉取:      │     │ 从 Map 0 拉取:      │
│   (Alice, 100)      │     │   (Bob, 200)        │
│ 从 Map 1 拉取:      │     │ 从 Map 1 拉取:      │
│   (Alice, 150)      │     │   (Charlie, 300)    │
│ 从 Map 2 拉取:      │     │ 从 Map 2 拉取:      │
│   (Alice, 250)      │     │   (Bob, 50)         │
│                     │     │                     │
│ 合并:               │     │ 合并:               │
│ Alice: [100,150,250]│     │ Bob: [200,50]       │
│                     │     │ Charlie: [300]      │
│ 聚合:               │     │                     │
│ Alice: 500          │     │ Bob: 250            │
│                     │     │ Charlie: 300        │
└─────────────────────┘     └─────────────────────┘
```

### 6.3 Shuffle 性能瓶颈

```
瓶颈分析:

1. 磁盘 I/O (Map 端写 + Reduce 端读)
   ├── 写: 每个 Map Task 写 N 个文件
   └── 读: 每个 Reduce Task 读 M 个文件
   总 I/O = M × N × 单文件大小

2. 网络传输 (Map → Reduce)
   ├── 数据量: 所有 Map 输出的总和
   ├── 带宽: 受限于集群网络
   └── 连接数: M × N 个 TCP 连接

3. 序列化/反序列化
   ├── Map 端: 对象 → 字节 (序列化)
   └── Reduce 端: 字节 → 对象 (反序列化)

4. 内存压力
   ├── Map 端: 缓冲区管理
   └── Reduce 端: 合并时需要内存
```

### 6.4 DuckDB 的"反 Shuffle"策略

```sql
-- DuckDB: Spill to Disk（单节点内）
-- 传统 shuffle: 跨节点网络传输

-- Spark:
SELECT name, SUM(amount) 
FROM huge_table          -- 10亿行
GROUP BY name;
-- 过程: Map → Shuffle 写 → 网络 → Shuffle 读 → Reduce

-- DuckDB:
SELECT name, SUM(amount) 
FROM huge_table          -- 10亿行
GROUP BY name;
-- 过程: 读数据 → 内存聚合 → 内存不足时 Spill 到本地 SSD → 继续聚合
```

| 维度 | Spark Shuffle | DuckDB Spill |
|------|--------------|-------------|
| 数据位置 | 跨节点（网络） | 单节点（本地磁盘） |
| 传输协议 | HTTP/GFS | 本地文件系统 |
| 延迟 | 网络 RTT + 序列化 | 磁盘 I/O（μs 级） |
| 带宽 | 1-10 Gbps | 3-7 GB/s (NVMe) |

**结论**：现代 NVMe SSD 的带宽（3-7 GB/s）远超万兆网络（1.25 GB/s），所以 DuckDB 的 Spill to Disk 在单节点场景下比 Spark 的网络 Shuffle 更快。

### 6.5 MotherDuck：不 Shuffle 的云方案

MotherDuck 的核心理念：**"Big Data is Dead"**——95% 的数据库 < 1TB，95% 的查询涉及 < 10GB 数据。

**Dual Execution 架构**：

```
┌─────────────────────────────────────────────────┐
│                  查询计划                        │
│                                                  │
│  ┌──────────┐          ┌──────────┐             │
│  │ 本地扫描  │          │ 远程扫描  │             │
│  │ (笔记本)  │          │ (云端)    │             │
│  └────┬─────┘          └────┬─────┘             │
│       │                     │                    │
│       └──────┬──────────────┘                    │
│              │                                   │
│       ┌──────▼──────┐                            │
│       │ Bridge 算子  │  ← 数据传输（Arrow IPC）   │
│       └──────┬──────┘                            │
│              │                                   │
│       ┌──────▼──────┐                            │
│       │   JOIN/聚合  │  ← 在最优位置执行          │
│       └─────────────┘                            │
└─────────────────────────────────────────────────┘
```

**工作流程**：
1. **优化器分析**：哪些表在本地，哪些在云端
2. **拆分查询计划**：标记每个算子为 `L`(Local) 或 `R`(Remote)
3. **插入 Bridge 算子**：在本地/远程边界传输数据（Arrow IPC 格式，零拷贝）
4. **智能路由**：尽量在数据所在位置执行，只传输最小结果

**与 Spark Shuffle 对比**：

| 维度 | Spark Shuffle | MotherDuck Dual Execution |
|------|--------------|--------------------------|
| 本质 | 数据按 key 重分布 | 查询计划拆分到两台机器 |
| 传输什么 | 所有数据按 hash 分发 | 过滤后的中间结果 |
| 网络开销 | `数据量 × 节点数` | `过滤后数据量`（通常极小） |
| 适用规模 | PB 级 | TB 级（单节点够用） |
| 复杂度 | 极高（分布式调度） | 低（两节点，客户端-服务器） |

**MotherDuck 扩展策略**：

| 数据量 | 方案 |
|--------|------|
| < 10GB | 纯本地 DuckDB（免费） |
| 10GB-1TB | Dual Execution（本地+云端） |
| 1-100TB | MotherDuck 云端 Duckling（scale up） |
| > 100TB | 不适合 MotherDuck，用 Spark/Smallpond |

### 6.6 DuckLake 分布式查询计划

**DuckLake 不做分布式查询**。DuckLake 是存储格式（类似 Iceberg），不是计算引擎。

```
DuckLake 架构：
├── 存储层：Parquet 文件（S3/OSS）     ← 可以无限扩展
├── 元数据层：PostgreSQL/DuckDB        ← 可以无限扩展
└── 计算层：DuckDB 实例                ← 每个实例仍是单节点
```

**DuckLake 路线图**：

| 版本 | 时间 | 内容 |
|------|------|------|
| v1.1 | 2026.09 | Variant 内联、多删除向量、查询计划序列化 |
| v2.0 | 远期 | 增量物化视图、删除向量改进、Branching/merge |
| 分布式计算 | **无计划** | 无 shuffle/分布式执行相关内容 |

DuckLake 通过**外部系统**解决大规模计算需求：
- 多用户并发：多个 DuckDB 实例连接同一个 PostgreSQL 目录
- 超大数据集：用 **Smallpond** 在 DuckLake 上做分布式处理
- 跨引擎查询：DuckLake 已有 Spark、Trino、DataFusion 的客户端实现

### 6.7 Spark vs Ray vs Beam vs Dask 对比

```
一句话定位：
"Spark 管数据，Ray 管模型，Beam 管流，Dask 管分析"
```

| 维度 | Spark | Ray | Beam | Dask |
|------|-------|-----|------|------|
| **定位** | 统一数据分析 | 分布式 AI 计算 | 流批统一框架 | 科学计算/分析 |
| **核心模型** | DataFrame + RDD | Actor + Task | Pipeline + PTransform | DataFrame + Delayed |
| ** Shuffle** | 有（磁盘+网络） | 无（GCS 对象存储） | 有（Runner 相关） | 有（本地/网络） |
| **内存管理** | 堆外 Tungsten | 共享对象存储 | Runner 决定 | 堆内+Dask 外部 |
| **生态** | 最成熟（10年+） | 快速增长（AI 领域） | 稳定但小众 | Python 原生 |
| **学习曲线** | 陡峭 | 中等 | 陡峭 | 平缓 |
| **语言** | Scala/Python/SQL | Python/Java | Java/Python/Go | Python |
| **适合场景** | 大规模 ETL、数仓 | LLM 训练、强化学习 | 复杂流处理、多 Runner | Pandas 加速、科研 |
| **不适合** | 低延迟、小数据 | 纯 SQL 分析 | 简单批处理 | 超大规模（>1PB） |

**选型决策树**：

```
你的场景是什么？
│
├─ 大规模 ETL / 数仓建设 → Spark
│   └─ 已有 Hadoop/K8s 生态，需要成熟调度和监控
│
├─ LLM 训练 / 强化学习 → Ray
│   └─ 需要 Actor 模型、动态任务图、GCS 共享对象
│
├─ 复杂流处理 / 多 Runner → Beam
│   └─ 需要写一次代码跑在 Spark/Flink/Dataflow 上
│
├─ 科学计算 / Pandas 加速 → Dask
│   └─ Python 原生，学习成本低，适合科研和中小规模分析
│
└─ TB 级 SQL 分析 → DuckDB（单机）/ Smallpond（分布式）
    └─ 不需要以上框架的复杂度
```

### 6.8 选型决策树

```
分布式计算方案选型：

数据规模 < 1TB？
  ├─ 是 → DuckDB 单机（最简，零运维）
  └─ 否 ↓

数据规模 1-100TB？
  ├─ 是 → Smallpond（DuckDB 分布式，首选）
  │       或 MotherDuck（云端托管，会 SQL 即可）
  └─ 否 ↓

数据规模 > 100TB？
  ├─ 是 → Spark（成熟，生态最好）
  │       或 Ray（AI/ML 场景）
  └─ 否 ↓

需要复杂流处理？
  ├─ 是 → Beam（Flink/Spark Runner）
  └─ 否 → Dask（Python 原生，轻量）

需要多团队共享元数据？
  ├─ 是 → DuckLake（PostgreSQL 目录 + Parquet）
  └─ 否 → 自建分片 + OSS 直查
```

---

## 参考资料

- [DuckDB 官方性能指南](https://duckdb.org/docs/current/guides/performance/overview)
- [DuckDB 大数据库工作指南](https://duckdb.org/docs/current/guides/performance/working_with_huge_databases.html)
- [DuckDB 工作负载调优](https://duckdb.org/docs/current/guides/performance/how_to_tune_workloads.html)
- [DuckDB 分区裁剪实践](https://duckdblab.org/en/post/duckdb-partition-pruning-production-guide/)
- [DuckDB 性能调优：150x 提速](https://duckdblab.org/en/post/duckdb-clickstream-performance-tuning/)
- [Smallpond：PB 级分布式 DuckDB](https://duckdblab.org/en/post/deepseek-smallpond-duckdb-distributed/)
- [DuckLake v1.0 发布](https://ducklake.select/2026/04/13/ducklake-10/)
- [DuckLake 分布式讨论](https://github.com/duckdb/ducklake/discussions/39)
- [OpenDuck：开源 MotherDuck 替代](https://github.com/citguru/openduck)
- [Quack：DuckDB 网络协议](https://petascalelabs.com/blog/duckdb-quack-plural)
- [MotherDuck 架构与能力](https://motherduck.com/docs/concepts/architecture-and-capabilities/)
- [MotherDuck Dual Execution](https://motherduck.com/docs/key-tasks/running-hybrid-queries/)
- [MotherDuck 论文 (CIDR 2024)](https://www.cidrdb.org/cidr2024/papers/p46-atwal.pdf)
- [Spark Shuffle 原理](https://spark.apache.org/docs/latest/rdd-programming-guide.html#shuffle-operations)
- [Ray 分布式计算](https://docs.ray.io/en/latest/ray-overview/index.html)
- [Apache Beam](https://beam.apache.org/documentation/)
- [Dask 文档](https://docs.dask.org/en/stable/)
- [阿里云 MaxCompute 计费说明](https://help.aliyun.com/zh/maxcompute/product-overview/overview-1)
- [阿里云 ECS 价格](https://www.aliyun.com/price/detail/ecs)
