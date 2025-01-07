# duckdb install

[duckdb安装](https://duckdb.org/docs/installation/index?version=stable&environment=cli&platform=linux&download_method=direct&architecture=x86_64)
### windows 系统安装

![](attach/Pasted%20image%2020241223141426.png)
下载地址：[https://github.com/duckdb/duckdb/releases/download/v1.1.3/duckdb_cli-windows-amd64.zip](https://github.com/duckdb/duckdb/releases/download/v1.1.3/duckdb_cli-windows-amd64.zip)

下载后进行解压：
![](attach/Pasted%20image%2020241223141909.png)
点击进入解压目录，双击运行duckdbe.exe 程序：
![](attach/Pasted%20image%2020241223142038.png)
![](attach/Pasted%20image%2020241223142059.png)
运行成功后：
![](attach/Pasted%20image%2020241223142141.png)


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

# 测试数据

```json
{ "store": {
    "book": [ 
      { "category": "reference",
        "author": "Nigel Rees",
        "title": "Sayings of the Century",
        "price": 8.95
      },
      { "category": "fiction",
        "author": "Evelyn Waugh",
        "title": "Sword of Honour",
        "price": 12.99
      },
      { "category": "fiction",
        "author": "Herman Melville",
        "title": "Moby Dick",
        "isbn": "0-553-21311-3",
        "price": 8.99
      },
      { "category": "fiction",
        "author": "J. R. R. Tolkien",
        "title": "The Lord of the Rings",
        "isbn": "0-395-19395-8",
        "price": 22.99
      }
    ],
    "bicycle": {
      "color": "red",
      "price": 19.95
    }
  }
}
```

```sql
CREATE TABLE tbl3 (j JSON);
insert into tbl3 values ('{"store":{"book":[{"category":"reference","author":"Nigel Rees","title":"Sayings of the Century","price":8.95},{"category":"fiction","author":"Evelyn Waugh","title":"Sword of Honour","price":12.99},{"category":"fiction","author":"Herman Melville","title":"Moby Dick","isbn":"0-553-21311-3","price":8.99},{"category":"fiction","author":"J. R. R. Tolkien","title":"The Lord of the Rings","isbn":"0-395-19395-8","price":22.99}],"bicycle":{"color":"red","price":19.95}}}');
```

```sql
select j->'$.store.book[*].author' from tbl3;

select j->'$..author' from tbl3;

select j->'$.store.*' from tbl3;

select j->'$.store..price' from tbl3;

select j->'$..book[2]' from tbl3;

select j->'$..book[#-1]' from tbl3;
```

# [jsonpath](https://goessner.net/articles/JsonPath/  )

不是所有的 jsonpath 路径都支持。支持的部分如下:

1. $.store.book[*].author
2. $..author
3. $.store.*
4. $.store..price
5. $..book[2]
6. $..book[#-1]

[prestodb json](https://prestodb.io/docs/current/functions/json.html)  


## 滑动窗口计算

#### 测试数据
```json
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "129.555575ms", "result": {"ip_user_ucnt": {"v": "13309671584", "gpv": 1, "gidle": 0, "uv": 1, "pv": 1, "vidle": 0, "group": "119.147.71.133", "timestamp": "1734425282"}, "register_result": ["\u6ce8\u518cip\u53d8\u66f4"], "ipcnt": {"timestamp": "1734425282", "v": "119.147.71.133", "counter": 11, "idle": 3043995}, "ip": "119.147.71.133", "user": "13309671584", "send_email": {"message": "success"}, "user_ip_ucnt": {"group": "13309671584", "gpv": 1, "pv": 1, "v": "119.147.71.133", "vidle": 0, "gidle": 0, "uv": 1, "timestamp": "1734425282"}, "white_list": {"result": false}, "black_list": {"result": false}}, "trace_id": "1efbc53a00536fa5b7211ecc6351715f"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "137.862074ms", "result": {"user_ip_ucnt": {"uv": 1, "gpv": 1, "group": "13625643658", "timestamp": "1734425291", "pv": 1, "gidle": 0, "vidle": 0, "v": "119.147.71.133"}, "ip_user_ucnt": {"group": "119.147.71.133", "timestamp": "1734425291", "uv": 2, "gidle": 9330, "gpv": 2, "pv": 1, "vidle": 0, "v": "13625643658"}, "send_email": {"message": "success"}, "black_list": {"result": false}, "ip": "119.147.71.133", "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c"], "ipcnt": {"idle": 9330, "counter": 12, "timestamp": "1734425291", "v": "119.147.71.133"}, "white_list": {"result": false}, "user": "13625643658"}, "trace_id": "1efbc53a59636184873ec5ce0b6c9f89"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "121.025392ms", "result": {"black_list": {"result": false}, "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c"], "user": "13309671584", "ip_user_ucnt": {"timestamp": "1734425299", "pv": 2, "v": "13309671584", "vidle": 17640, "gpv": 3, "group": "119.147.71.133", "uv": 2, "gidle": 8310}, "ipcnt": {"counter": 13, "timestamp": "1734425299", "idle": 8310, "v": "119.147.71.133"}, "ip": "119.147.71.133", "send_email": {"message": "success"}, "user_ip_ucnt": {"uv": 1, "pv": 2, "timestamp": "1734425299", "group": "13309671584", "vidle": 17640, "v": "119.147.71.133", "gidle": 17640, "gpv": 2}, "white_list": {"result": false}}, "trace_id": "1efbc53aa87b69efaa39e3d5badee095"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "5.245953ms", "result": {"white_list": {"result": false}, "ip_user_ucnt": {"gidle": 0, "v": "13625643658", "pv": 1, "timestamp": "1734425308", "vidle": 0, "uv": 1, "group": "111.85.210.139", "gpv": 1}, "user_ip_ucnt": {"pv": 1, "v": "111.85.210.139", "gpv": 2, "vidle": 0, "uv": 2, "group": "13625643658", "gidle": 16606, "timestamp": "1734425308"}, "ipcnt": {"idle": 3056539, "v": "111.85.210.139", "counter": 5, "timestamp": "1734425308"}, "user": "13625643658", "ip": "111.85.210.139", "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00\u7528\u6237\u4e0b\u591a\u4e2aip\u6ce8\u518c"], "black_list": {"result": false}}, "trace_id": "1efbc53af67d6cd8acd0ebf106fe91ba"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "5.340705ms", "result": {"ip_user_ucnt": {"timestamp": "1734425311", "pv": 1, "vidle": 0, "group": "111.85.210.139", "v": "17707115955", "uv": 2, "gpv": 2, "gidle": 3165}, "ip": "111.85.210.139", "user": "17707115955", "user_ip_ucnt": {"vidle": 0, "pv": 1, "v": "111.85.210.139", "gpv": 1, "group": "17707115955", "timestamp": "1734425311", "uv": 1, "gidle": 0}, "white_list": {"result": false}, "black_list": {"result": false}, "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c"], "ipcnt": {"idle": 3165, "v": "111.85.210.139", "counter": 6, "timestamp": "1734425311"}}, "trace_id": "1efbc53b14aa66bb9504d2afe03db53a"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "122.779011ms", "result": {"register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c"], "black_list": {"result": false}, "ip": "119.147.71.133", "ip_user_ucnt": {"pv": 3, "group": "119.147.71.133", "gidle": 17669, "gpv": 4, "vidle": 17669, "uv": 2, "timestamp": "1734425317", "v": "13309671584"}, "send_email": {"message": "success"}, "user": "13309671584", "user_ip_ucnt": {"vidle": 17669, "uv": 1, "gidle": 17669, "timestamp": "1734425317", "v": "119.147.71.133", "group": "13309671584", "pv": 3, "gpv": 3}, "white_list": {"result": false}, "ipcnt": {"timestamp": "1734425317", "idle": 17669, "counter": 14, "v": "119.147.71.133"}}, "trace_id": "1efbc53b50fd64c2bcc603910b1151c5"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "5.497546ms", "result": {"user_ip_ucnt": {"pv": 1, "gpv": 4, "gidle": 8276, "timestamp": "1734425325", "uv": 2, "vidle": 0, "v": "111.85.210.139", "group": "13309671584"}, "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c", "\u5355\u4e00\u7528\u6237\u4e0b\u591a\u4e2aip\u6ce8\u518c"], "white_list": {"result": false}, "black_list": {"result": false}, "ip_user_ucnt": {"group": "111.85.210.139", "pv": 1, "gpv": 3, "uv": 3, "gidle": 14484, "timestamp": "1734425325", "v": "13309671584", "vidle": 0}, "ip": "111.85.210.139", "ipcnt": {"v": "111.85.210.139", "timestamp": "1734425325", "idle": 14484, "counter": 7}, "user": "13309671584"}, "trace_id": "1efbc53b9ecd683f878b56a2a86c6e44"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "7.012836ms", "result": {"register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c", "\u5355\u4e00\u7528\u6237\u4e0b\u591a\u4e2aip\u6ce8\u518c"], "white_list": {"result": false}, "black_list": {"result": false}, "user": "13309671584", "ip": "111.85.210.139", "ip_user_ucnt": {"pv": 2, "timestamp": "1734425332", "uv": 3, "gidle": 6192, "gpv": 4, "vidle": 6192, "group": "111.85.210.139", "v": "13309671584"}, "user_ip_ucnt": {"pv": 2, "timestamp": "1734425332", "uv": 2, "v": "111.85.210.139", "gidle": 6192, "gpv": 5, "vidle": 6192, "group": "13309671584"}, "ipcnt": {"v": "111.85.210.139", "idle": 6192, "counter": 8, "timestamp": "1734425332"}}, "trace_id": "1efbc53bd9da6af9ba9e9f4fe8223f1e"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "6.075249ms", "result": {"black_list": {"result": false}, "white_list": {"result": false}, "ip": "111.85.210.139", "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c", "\u5355\u4e00\u7528\u6237\u4e0b\u591a\u4e2aip\u6ce8\u518c"], "user_ip_ucnt": {"timestamp": "1734425337", "group": "13625643658", "v": "111.85.210.139", "pv": 2, "vidle": 29004, "gidle": 29004, "gpv": 3, "uv": 2}, "ip_user_ucnt": {"group": "111.85.210.139", "uv": 3, "v": "13625643658", "gpv": 5, "vidle": 29004, "pv": 2, "gidle": 5163, "timestamp": "1734425337"}, "user": "13625643658", "ipcnt": {"timestamp": "1734425337", "counter": 9, "idle": 5163, "v": "111.85.210.139"}}, "trace_id": "1efbc53c0b18641c9442b643aa01f46c"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "5.739437ms", "result": {"ipcnt": {"v": "111.85.210.139", "counter": 10, "timestamp": "1734425343", "idle": 6185}, "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c", "\u5355\u4e00\u7528\u6237\u4e0b\u591a\u4e2aip\u6ce8\u518c"], "white_list": {"result": false}, "user": "13625643658", "ip_user_ucnt": {"uv": 3, "pv": 3, "group": "111.85.210.139", "timestamp": "1734425343", "gpv": 6, "gidle": 6185, "v": "13625643658", "vidle": 6185}, "black_list": {"result": false}, "ip": "111.85.210.139", "user_ip_ucnt": {"v": "111.85.210.139", "pv": 3, "gidle": 6185, "uv": 2, "gpv": 4, "group": "13625643658", "vidle": 6185, "timestamp": "1734425343"}}, "trace_id": "1efbc53c46136638b0cdb695d9c9b9c3"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "123.696608ms", "result": {"user_ip_ucnt": {"v": "111.85.210.139", "gpv": 2, "uv": 1, "pv": 2, "gidle": 37203, "timestamp": "1734425348", "vidle": 37203, "group": "17707115955"}, "black_list": {"result": false}, "user": "17707115955", "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c"], "ip": "111.85.210.139", "white_list": {"result": false}, "ip_user_ucnt": {"gidle": 5179, "timestamp": "1734425348", "uv": 3, "v": "17707115955", "pv": 2, "vidle": 37203, "gpv": 7, "group": "111.85.210.139"}, "ipcnt": {"counter": 11, "timestamp": "1734425348", "v": "111.85.210.139", "idle": 5179}, "send_email": {"message": "success"}}, "trace_id": "1efbc53c78996cc39a7f349ac2cc693a"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "125.573966ms", "result": {"white_list": {"result": false}, "black_list": {"result": false}, "user_ip_ucnt": {"gidle": 18837, "vidle": 33305, "gpv": 5, "pv": 3, "group": "13309671584", "uv": 1, "v": "119.147.71.133", "timestamp": "1734425350"}, "register_result": ["\u6ce8\u518cip\u53d8\u66f4"], "ip_user_ucnt": {"vidle": 33305, "gidle": 33305, "group": "119.147.71.133", "timestamp": "1734425350", "gpv": 4, "uv": 1, "pv": 3, "v": "13309671584"}, "ipcnt": {"v": "119.147.71.133", "timestamp": "1734425350", "idle": 33305, "counter": 14}, "ip": "119.147.71.133", "send_email": {"message": "success"}, "user": "13309671584"}, "trace_id": "1efbc53c8ea06dadab4150f27f340ce0"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "123.467784ms", "result": {"send_email": {"message": "success"}, "user_ip_ucnt": {"v": "119.147.71.133", "vidle": 0, "gpv": 3, "group": "17707115955", "uv": 2, "gidle": 4608, "timestamp": "1734425353", "pv": 1}, "black_list": {"result": false}, "ip": "119.147.71.133", "ipcnt": {"idle": 2298, "counter": 15, "timestamp": "1734425353", "v": "119.147.71.133"}, "user": "17707115955", "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c", "\u5355\u4e00\u7528\u6237\u4e0b\u591a\u4e2aip\u6ce8\u518c"], "white_list": {"result": false}, "ip_user_ucnt": {"gpv": 5, "timestamp": "1734425353", "pv": 1, "gidle": 2298, "group": "119.147.71.133", "v": "17707115955", "vidle": 0, "uv": 2}}, "trace_id": "1efbc53ca48c68e79a200170050c0efd"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "122.802121ms", "result": {"ip_user_ucnt": {"gpv": 8, "group": "111.85.210.139", "v": "13625643658", "vidle": 11088, "gidle": 5909, "uv": 3, "timestamp": "1734425354", "pv": 4}, "user_ip_ucnt": {"pv": 4, "v": "111.85.210.139", "uv": 1, "gpv": 4, "vidle": 11088, "timestamp": "1734425354", "gidle": 11088, "group": "13625643658"}, "ip": "111.85.210.139", "user": "13625643658", "ipcnt": {"counter": 12, "timestamp": "1734425354", "idle": 5909, "v": "111.85.210.139"}, "black_list": {"result": false}, "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c"], "white_list": {"result": false}, "send_email": {"message": "success"}}, "trace_id": "1efbc53cb0ef673dae62bd6461e2e69f"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "126.948236ms", "result": {"user_ip_ucnt": {"gpv": 5, "group": "13309671584", "timestamp": "1734425355", "vidle": 23735, "pv": 3, "uv": 1, "v": "111.85.210.139", "gidle": 4898}, "white_list": {"result": false}, "ipcnt": {"v": "111.85.210.139", "timestamp": "1734425355", "idle": 1299, "counter": 13}, "black_list": {"result": false}, "user": "13309671584", "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c"], "send_email": {"message": "success"}, "ip_user_ucnt": {"pv": 3, "uv": 3, "group": "111.85.210.139", "gpv": 9, "gidle": 1299, "timestamp": "1734425355", "vidle": 23735, "v": "13309671584"}, "ip": "111.85.210.139"}, "trace_id": "1efbc53cbd5e6262956460b2f6af8615"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "115.331404ms", "result": {"register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c"], "send_email": {"message": "success"}, "black_list": {"result": false}, "ip": "111.85.210.139", "ipcnt": {"timestamp": "1734425357", "idle": 1285, "counter": 14, "v": "111.85.210.139"}, "user_ip_ucnt": {"uv": 1, "gpv": 6, "gidle": 1285, "group": "13309671584", "timestamp": "1734425357", "pv": 4, "v": "111.85.210.139", "vidle": 1285}, "white_list": {"result": false}, "ip_user_ucnt": {"group": "111.85.210.139", "uv": 3, "gidle": 1285, "timestamp": "1734425357", "pv": 4, "vidle": 1285, "gpv": 10, "v": "13309671584"}, "user": "13309671584"}, "trace_id": "1efbc53cc98367f6b6916f049c614ba7"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "118.008882ms", "result": {"ip_user_ucnt": {"gpv": 11, "timestamp": "1734425361", "gidle": 4269, "pv": 3, "v": "17707115955", "vidle": 12762, "group": "111.85.210.139", "uv": 3}, "user": "17707115955", "white_list": {"result": false}, "ipcnt": {"counter": 15, "idle": 4269, "timestamp": "1734425361", "v": "111.85.210.139"}, "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c", "\u5355\u4e00\u7528\u6237\u4e0b\u591a\u4e2aip\u6ce8\u518c"], "black_list": {"result": false}, "send_email": {"message": "success"}, "ip": "111.85.210.139", "user_ip_ucnt": {"gpv": 4, "uv": 2, "gidle": 8154, "vidle": 12762, "v": "111.85.210.139", "timestamp": "1734425361", "pv": 3, "group": "17707115955"}}, "trace_id": "1efbc53cf241608aaadd22b694831787"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "130.230356ms", "result": {"register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c", "\u5355\u4e00\u7528\u6237\u4e0b\u591a\u4e2aip\u6ce8\u518c"], "user_ip_ucnt": {"timestamp": "1734425364", "uv": 2, "gpv": 5, "v": "119.147.71.133", "group": "13625643658", "pv": 1, "vidle": 0, "gidle": 10143}, "ip": "119.147.71.133", "send_email": {"message": "success"}, "ip_user_ucnt": {"v": "13625643658", "gpv": 5, "group": "119.147.71.133", "pv": 1, "timestamp": "1734425364", "gidle": 11444, "uv": 3, "vidle": 0}, "user": "13625643658", "ipcnt": {"counter": 16, "timestamp": "1734425364", "v": "119.147.71.133", "idle": 11444}, "black_list": {"result": false}, "white_list": {"result": false}}, "trace_id": "1efbc53d11bf6f1d8b92b3a3fcb0e6e9"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "117.003039ms", "result": {"ip": "119.147.71.133", "ip_user_ucnt": {"v": "17707115955", "vidle": 18732, "gidle": 7288, "uv": 3, "gpv": 6, "group": "119.147.71.133", "pv": 2, "timestamp": "1734425371"}, "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c"], "black_list": {"result": false}, "send_email": {"message": "success"}, "user_ip_ucnt": {"uv": 1, "v": "119.147.71.133", "pv": 2, "gidle": 10578, "timestamp": "1734425371", "vidle": 18732, "gpv": 4, "group": "17707115955"}, "white_list": {"result": false}, "user": "17707115955", "ipcnt": {"timestamp": "1734425371", "counter": 17, "idle": 7288, "v": "119.147.71.133"}}, "trace_id": "1efbc53d571f68cabb0aefdf22b39e4a"}, "message": "version 1.0"}
{"status": 0, "data": {"rule_id": "rule_ce53aadda8d241cc", "rule_name": "\u6ce8\u518c\u793a\u4f8b", "performance": "122.992814ms", "result": {"ip": "119.147.71.133", "ipcnt": {"idle": 4312, "counter": 18, "timestamp": "1734425376", "v": "119.147.71.133"}, "user_ip_ucnt": {"group": "13309671584", "vidle": 25342, "uv": 0, "gpv": 6, "v": "119.147.71.133", "timestamp": "1734425376", "gidle": 19159, "pv": 3}, "user": "13309671584", "black_list": {"result": false}, "white_list": {"result": false}, "register_result": ["\u6ce8\u518cip\u53d8\u66f4", "\u5355\u4e00ip\u4e0b\u591a\u4e2a\u7528\u6237\u6ce8\u518c"], "ip_user_ucnt": {"group": "119.147.71.133", "gidle": 4312, "uv": 2, "timestamp": "1734425376", "v": "13309671584", "pv": 3, "vidle": 25342, "gpv": 6}, "send_email": {"message": "success"}}, "trace_id": "1efbc53d804d69a9a04575adbf8afdae"}, "message": "version 1.0"}
```
#### 数据清洗

执行下列sql 清洗数据，提取分析所需字段
```sql
CREATE TABLE regs as SELECT 
    to_timestamp((data ->>'$.result.ipcnt.timestamp')::int) as date,
    CAST(data ->>'$.result.ipcnt.timestamp' as INTEGER) as op_timestamp, 
    data ->>'$.result.ip'as ip,data ->>'$.result.user'as user 
from read_json("./register_rule_data.json");

```
数据清洗结果
```sql
select * from regs;
```
![](attach/Pasted%20image%2020241223143308.png)
#### 指标计算
执行下列sql 进行滑动窗口计算，计算对应指标数据
```sql
SELECT 
    date,
    op_timestamp,
    ip,
    user,
    COUNT(*) OVER gpv_rate AS gpv_l1m,COUNT(*) OVER pv_rate AS pv_l1m,
    COUNT(distinct (user)) OVER uv_rate AS uv_l1m,
    op_timestamp -lag(op_timestamp, 1) OVER gidle_res AS gidle,
    op_timestamp -lag(op_timestamp, 1) OVER vidle_res AS vidle 
FROM regs 
    WINDOW 
        gpv_rate AS (PARTITION BY ip ORDER BY date RANGE BETWEEN INTERVAL '1 minute' PRECEDING AND CURRENT ROW),
        pv_rate AS (PARTITION BY (ip, user) ORDER BY date RANGE BETWEEN INTERVAL '1 minute' PRECEDING AND CURRENT ROW),
        uv_rate AS (PARTITION BY ip ORDER BY date RANGE BETWEEN INTERVAL '1 minute' PRECEDING AND CURRENT ROW),
        gidle_res AS (PARTITION BY ip ORDER BY date RANGE BETWEEN INTERVAL '1 minute' PRECEDING AND CURRENT ROW),
        vidle_res AS (PARTITION BY (ip, user) ORDER BY date RANGE BETWEEN INTERVAL '1 minute' PRECEDING AND CURRENT ROW) 
ORDER BY date;
```
计算结果

![](attach/Pasted%20image%2020241223143211.png)






## Excel文件导入/导出

#### Excel导出

```sql
# 安装并导入扩展
INSTALL spatial;
LOAD spatial;

# 导出数据
COPY (SELECT * FROM regs) TO 'output.xlsx' WITH (FORMAT GDAL, DRIVER 'xlsx');
```

#### Excel导入
```sql
# 直接读取数据
SELECT * FROM st_read('output.xlsx');

# 读取数据并创建表格导入数据
CREATE TABLE new_tbl AS SELECT * FROM st_read('output.xlsx');
select * from new_tbl;
```