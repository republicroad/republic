# Parquet 分区与 Z-Order 兼容性总结

> 适用场景：流式采集 → 落盘 → 查询 的数据管线架构选型参考

## 1. Parquet 分区基础

### 1.1 分区键选择原则

按查询模式分区，而非数据量：

- **时间维度优先**：`year=/month=/day=` — 覆盖绝大多数 OLAP 引擎
- **高基数维度慎用**：如 `user_id` 分区会导致小文件过多，改用 Z-Order / Bucket

### 1.2 Hive 风格目录结构

```
data/
  year=2026/
    month=08/
      day=19/
        part-0.parquet
```

所有主流引擎均原生识别：Spark、Trino、Presto、DuckDB、Pandas、Polars、BigQuery、Athena。

### 1.3 谓词下推兼容性

| 引擎 | 分区过滤 | 文件内过滤（page stats） |
|------|----------|------------------------|
| Spark | 自动 | 自动 |
| Trino | 自动 | 自动 |
| DuckDB | `hive_partitioning=true` | 自动 |
| Pandas | `filters` 参数 | 自动（需 PyArrow） |
| Polars | 自动 | 自动 |
| BigQuery | 自动（外部表） | 自动 |
| Athena | 自动 | 自动 |

### 1.4 写入最佳实践

```python
import pyarrow.parquet as pq
import pyarrow as pa

table = pa.table({...})
pq.write_to_dataset(
    table,
    root_path="output/",
    partition_cols=["year", "month", "day"],
    use_legacy_dataset=False,
    existing_data_behavior="overwrite_or_ignore",
    compression="snappy",
)
```

注意事项：

- 分区列用 **string 类型**（非 int），避免引擎解析错误
- 分区列值保持 ISO 格式（`2026-08-19`）
- 压缩选择 Snappy / Zstd（兼容性最好）
- 分区列不放进 Parquet 文件内部，输出到目录名

## 2. Z-Order 概念

Z-Order（Morton Code）是一种空间填充曲线，将多维数据映射到一维，使多列值在物理存储上邻近分布，从而收紧 row-group 级 min/max 统计范围，提升多列过滤的 data-skipping 效果。

适用场景：查询条件不固定在分区列上，需要对 2+ 列做等频过滤。

## 3. 各系统 Z-Order 支持现状

### 3.1 DuckDB

**不支持 Z-Order，Roadmap 中无计划。**

官方 Roadmap（2026-08 更新）相关项：

- **Sorting-aware optimizations** — 排序感知优化（Future Work）
- **Partition-aware optimizations** — 分区感知优化（Future Work）
- Lakehouse 格式改进（Iceberg、Delta、DuckLake）

间接路径：通过 Delta Lake 扩展读取已由 delta-rs 做过 Z-Order 的 Parquet 文件。

### 3.2 Vector.dev S3 Sink

**不支持 Z-Order。**

Vector v0.55.0（2026-04）新增 S3 sink Parquet batch encoding，仅支持：

- 按 batch 写入 Parquet 文件
- Snappy / Zstd / Gzip 压缩
- 自动 schema 推断

不支持排序/聚类的原因：

1. 流式写入按到达顺序追加，无法全局排序
2. 无全局视图，Z-Order 需要全量数据空间填充曲线计算
3. 底层 Arrow Parquet writer 不提供 Z-Order 接口

### 3.3 DuckLake

**不支持 Z-Order，但有 Sorted Tables 替代方案。**

```sql
-- 单列排序
ALTER TABLE events SET SORTED BY (event_time ASC);

-- 多列排序
ALTER TABLE events SET SORTED BY (event_time ASC, event_type DESC);

-- 表达式排序
ALTER TABLE events SET SORTED BY (date_trunc('hour', event_time) ASC);
```

Sorted Tables 工作原理：

- INSERT / compaction / flush 时自动按指定列排序写入 Parquet
- 查询时利用 page-level min/max statistics 做 data-skipping
- 支持关闭插入排序（`sort_on_insert = false`）提升写入速度

### 3.4 支持 Z-Order 的系统

| 系统 | Z-Order 支持 | 备注 |
|------|-------------|------|
| Delta Lake (Spark) | ✅ `OPTIMIZE ... ZORDER BY` | 最成熟 |
| Delta Lake (delta-rs) | ✅ | Rust 实现，DuckDB 可读取其输出 |
| Apache Iceberg | ✅ `rewriteDataFiles` + sort order | Spark 引擎支持 |
| Apache Hudi | ✅ `HoodieClusteringJob` | |
| Databricks | ✅ + Liquid Clustering | 推荐替代 Z-Order |
| Snowflake | ✅ `CLUSTER BY` | 自动管理 |
| BigQuery | ✅ `CLUSTER BY` | 建表时定义 |

## 4. 流式架构的典型折中方案

```
Vector (S3 sink)          →  原始 Parquet（无序，按时间分区）
  ↓ 定时任务（Spark / delta-rs / DuckDB）
批量重排层                  →  Z-Order / OPTIMIZE / Sorted Tables
  ↓
查询引擎（DuckDB / Trino）  →  利用 statistics 做 predicate pushdown
```

核心思路：流式负责采集落盘，批量层负责优化数据布局。这是业界标准做法。

## 5. Sorted Tables vs Z-Order 对比

| 维度 | DuckLake Sorted Tables | Z-Order |
|------|----------------------|---------|
| 排序方式 | 列级线性排序 | 多维空间填充曲线 |
| 多列效果 | 第二列仅在第一列值相同时有序 | 所有列均匀分布 |
| 增量写入 | 支持（compaction 时重排） | 需全量重写 |
| 跨引擎兼容 | DuckDB 原生 | Delta Lake / Iceberg |
| 适用场景 | 1-2 个高频过滤列 | 3+ 列等频查询 |

## 6. 选型建议

| 场景 | 推荐方案 |
|------|---------|
| 单列/时间序列过滤 | Hive 分区 + DuckLake Sorted Tables |
| 多列等频查询 | Delta Lake Z-Order (delta-rs) + DuckDB 查询 |
| 流式采集 + 离线优化 | Vector S3 sink → delta-rs Z-Order → DuckDB |
| 已有 DuckDB 技术栈 | DuckLake Sorted Tables（够用） |
| 需要最强 data-skipping | Delta Lake / Iceberg + Z-Order |
