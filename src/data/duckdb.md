# duckdb install

[duckdb安装](https://duckdb.org/docs/installation/index?version=stable&environment=cli&platform=linux&download_method=direct&architecture=x86_64)

# load data

数据明细结构如下所示:

```json
     // 这是其中一条log日志 主要对shadow_output:[{},{},{}]进行展开 
     {
     "_timestamp":1732518749379000,
     "agent":"{"ephemeral_id":"17fee4a3-9865-4542-8779-ddd5bb492dd7",
     "id":"38db1be3-001e-4193-b433-f6405275e68d",
     "name":"brde-59f77f89b-7bmft",
     "type":"filebeat",
     "version":"8.14.1,
     "ecs":"{"version":"8.0.0"}",
     "event_day":"2024-11-25",
     "event_hour":"15:00",
     "host":"{"name":"brde-59f77f89b-7bmft"}",
     "id":"1efaafca1bd56d1ca9b9059487b1df63",
     "input": {}
     "log":"{"file":{"path":"/log/brde_datalog/DATA_LOG_6.log"},"offset":2693589}",
     "proj_id":"proj_e8033f581ead4852",
     "shadow_output":"[{"performance":"6.851766ms","result":{"reason":"账号黑单","result":"reject"},"rule_id":"rule_5d87861d10b84380","rule_name":"多彩规则1"}]",
     "user_id":"82b3c08950464a23a72d3c5c6403730d"
     }
```


```shell
# 解压dchk.zip后查看
tree -L 1 dchk

# 有三天的数据
dchk
├── 22
├── 23
└── 24

# 进入duckdb shell
./duckdb
```
进入 duckdb shell 后，
```sql
-- 查看所有数据
SELECT * FROM 'dchk/**/*.parquet';
```

# duckdb 数据分析

## 创建表

```sql
-- 导入数据并创建数据表tbl（dchk下所有.parquet)
CREATE TABLE tbl as SELECT * FROM 'dchk/**/*.parquet';
```

##  查看表字段

```sql
DESCRIBE tbl;
┌───────────────┬─────────────┬─────────┬─────────┬─────────┬─────────┐
│  column_name  │ column_type │  null   │   key   │ default │  extra  │
│    varchar    │   varchar   │ varchar │ varchar │ varchar │ varchar │
├───────────────┼─────────────┼─────────┼─────────┼─────────┼─────────┤
│ _timestamp    │ BIGINT      │ YES     │         │         │         │
│ agent         │ VARCHAR     │ YES     │         │         │         │
│ ecs           │ VARCHAR     │ YES     │         │         │         │
│ event_day     │ VARCHAR     │ YES     │         │         │         │
│ event_hour    │ VARCHAR     │ YES     │         │         │         │
│ host          │ VARCHAR     │ YES     │         │         │         │
│ id            │ VARCHAR     │ YES     │         │         │         │
│ input         │ VARCHAR     │ YES     │         │         │         │
│ log           │ VARCHAR     │ YES     │         │         │         │
│ proj_id       │ VARCHAR     │ YES     │         │         │         │
│ shadow_output │ VARCHAR     │ YES     │         │         │         │
│ user_id       │ VARCHAR     │ YES     │         │         │         │
├───────────────┴─────────────┴─────────┴─────────┴─────────┴─────────┤
│ 12 rows                                                   6 columns │
└─────────────────────────────────────────────────────────────────────┘
```

## 查询表数据

此例子主要展示 duckdb 分析 [json 对象和数组嵌套json对象](https://duckdb.org/docs/data/json/json_functions)。

```sql
-- 查询所有字段
select * from tbl limit 3;
```

对 shadow_output 这种复杂的 json 字段进行分析

```sql
select shadow_output from tbl limit 3;
┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                                     shadow_output                                                     │
│                                                        varchar                                                        │
├───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│ [{"performance":"16.665342ms","result":{"result":"pass"},"rule_id":"rule_5d87861d10b84380","rule_name":"多彩规则1"}]  │
│ [{"performance":"16.322001ms","result":{"result":"pass"},"rule_id":"rule_5d87861d10b84380","rule_name":"多彩规则1"}]  │
│ [{"performance":"6.653507ms","result":{"result":"reject"},"rule_id":"rule_5d87861d10b84380","rule_name":"多彩规则1"}] │
└───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

-- 2 取result结果
select shadow_output->'$[0].result.result',id from tbl limit 3;
┌─────────────────────────────────────────┬──────────────────────────────────┐
│ (shadow_output -> '$[0].result.result') │                id                │
│                  json                   │             varchar              │
├─────────────────────────────────────────┼──────────────────────────────────┤
│ "pass"                                  │ 1efa91d27e49686aaccd6e27b025db76 │
│ "pass"                                  │ 1efa91d2f1736e82b76809244a0d3667 │
│ "reject"                                │ 1efa91d383736c6bab3dfd3ebb80ba8a │
└─────────────────────────────────────────┴──────────────────────────────────┘

```

## 分组聚合

```sql
-- 3 对结果进行group
select shadow_output->'$[0].result.result' as _res,count(*) from tbl group by 1;
┌──────────┬──────────────┐
│   _res   │ count_star() │
│   json   │    int64     │
├──────────┼──────────────┤
│ "pass"   │       274197 │
│ "reject" │       146436 │
└──────────┴──────────────┘
```


## 宽格式转长格式

一般是把嵌套的数据变成多行数据，方便聚合统计分析.

```sql
D select unnest(shadow_output->'$[*].result.result') as res,id from tbl limit 10;
┌──────────┬──────────────────────────────────┐
│   res    │                id                │
│   json   │             varchar              │
├──────────┼──────────────────────────────────┤
│ "pass"   │ 1efa91d27e49686aaccd6e27b025db76 │
│ "pass"   │ 1efa91d2f1736e82b76809244a0d3667 │
│ "reject" │ 1efa91d383736c6bab3dfd3ebb80ba8a │
│ "pass"   │ 1efa91d38f286b5b9a72d4b34ff8c86d │
│ "reject" │ 1efa91d5e2ae6b24ae4d59028badd59b │
│ "reject" │ 1efa91d6f9c267beae69c1ec0b504936 │
│ "pass"   │ 1efa91d7f5bd6c0ba5e6e9310d870420 │
│ "pass"   │ 1efa91d7fd156dd4a5ddcd73f1ed49f9 │
│ "reject" │ 1efa91d805e36491b5e7e708e57cbe62 │
│ "reject" │ 1efa91d80fed6e0f863660f457778c84 │
├──────────┴──────────────────────────────────┤
│ 10 rows                           2 columns │
└─────────────────────────────────────────────┘

```



```sql
-- _timestamp 字段是微秒(us), 所以需要除以 1000000 转化为秒
D select date_trunc('hour',to_timestamp((_timestamp/1000000))) as hour,shadow_output->'$[*].result.result' as res from tbl limit 3;
┌──────────────────────────┬────────────┐
│           hour           │    res     │
│ timestamp with time zone │   json[]   │
├──────────────────────────┼────────────┤
│ 2024-11-23 06:00:00+08   │ ["pass"]   │
│ 2024-11-23 06:00:00+08   │ ["pass"]   │
│ 2024-11-23 06:00:00+08   │ ["reject"] │
└──────────────────────────┴────────────┘

-- unnest 将嵌套的数组变成多行(宽格式转成长格式)
D select date_trunc('hour',to_timestamp((_timestamp/1000000))) as hour,id,unnest(shadow_output->'$[*].result.result') as res from tbl limit 2;
┌──────────────────────────┬──────────────────────────────────┬────────┐
│           hour           │                id                │  res   │
│ timestamp with time zone │             varchar              │  json  │
├──────────────────────────┼──────────────────────────────────┼────────┤
│ 2024-11-23 06:00:00+08   │ 1efa91d27e49686aaccd6e27b025db76 │ "pass" │
│ 2024-11-23 06:00:00+08   │ 1efa91d2f1736e82b76809244a0d3667 │ "pass" │
└──────────────────────────┴──────────────────────────────────┴────────┘

-- 对长格式进行分组绝活

D with unnest_tbl as (select date_trunc('hour',to_timestamp((_timestamp/1000000))) as hour,id,unnest(shadow_output->'$[*].result.result') as res from tbl) 
  select hour,res,count(*) from unnest_tbl group by 1, 2 limit 5;
┌──────────────────────────┬──────────┬──────────────┐
│           hour           │   res    │ count_star() │
│ timestamp with time zone │   json   │    int64     │
├──────────────────────────┼──────────┼──────────────┤
│ 2024-11-24 14:00:00+08   │ "reject" │        28096 │
│ 2024-11-24 15:00:00+08   │ "pass"   │         4103 │
│ 2024-11-24 17:00:00+08   │ "pass"   │         1256 │
│ 2024-11-24 21:00:00+08   │ "pass"   │         1836 │
│ 2024-11-24 22:00:00+08   │ "reject" │          868 │
└──────────────────────────┴──────────┴──────────────┘

```



# duckdb 练习

[Querying Parquet with Precision Using DuckDB](https://duckdb.org/2021/06/25/querying-parquet)

```sql
-- 创建
CREATE TABLE tbl2 (id varchar,j JSON);

-- 插入
INSERT INTO tbl2 VALUES ('1efa91d27e49686aaccd6e27b025db76','[{"performance":"16.665342ms","result":{"res":"pass"},"rule_id":"rule_5d87861d10b84381","rule_name":"规则1"},{"performance":"16.322001ms","result":{"res":"pass"},"rule_id":"rule_5d87861d10b84382","rule_name":"规则2"},{"performance":"6.653507ms","result":{"res":"reject"},"rule_id":"rule_5d87861d10b84383","rule_name":"规则3"}]');

-- 查询
select * from tbl2;
──────────────────────┬─────────────────────────────────────────────────────────────────────────────────────────────────────────
│          id          │                                        j                                                                                         
│       varchar        │                                       json                                                                                        
├──────────────────────┼────────────────────────────────────────────────────────────────────────────────────────────────────────
│ 1efa91d27e49686aac…  │ [{"performance":"16.665342ms","result":{"res":"pass"},"rule_id":"rule_5d87861d10b84381","rule_name":"规则1"},
                         {"performance":"16.322001ms","result":{"res":"pass"},"rule_id":"rule_5d87861d10b84382","rule_name":"规则2"},
                          {"performance":"6.653507ms","result":{"res":"reject"},"rule_id":"rule_5d87861d10b84383","rule_name":" 规则3"}]                         
└──────────────────────┴────────────────────────────────────────────────────────────────────────────────────────────────────────


-- 查询2
select j->'$[*].result.res' from tbl2;
┌─────────────────────────────┐
│ (j -> '$[*].result.res') │
│           json[]            │
├─────────────────────────────┤
│ ["pass", "pass", "reject"]  │
└─────────────────────────────┘


-- 利用unnest 展开
select id, unnest(j->'$[*].result.res')from tbl2;
┌──────────────────────────────────┬──────────────────────────────────┐
│                id                │ unnest((j -> '$[*].result.res')) │
│             varchar              │               json               │
├──────────────────────────────────┼──────────────────────────────────┤
│ 1efa91d27e49686aaccd6e27b025db76 │ "pass"                           │
│ 1efa91d27e49686aaccd6e27b025db76 │ "pass"                           │
│ 1efa91d27e49686aaccd6e27b025db76 │ "reject"                         │
└──────────────────────────────────┴──────────────────────────────────┘
```