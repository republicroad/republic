### 构建Postgresql CDC(Change Data Capture)同步流程


#### 1常用方法有：

```text
1 pg的wal(预写日志）wal不是人直观的数据，需要一层层解析里面的二进制page，中间夹着着各种header头文件和原控制信息。市面上如，
WalMiner,wal2sql 是waL(write ahead logs)逻辑复制日志中解析出执行的SQL语句的工具。解析性能慢，并且需要修改pg的日志level配置（walminer除外）和slot的配置，
会造成pg存储压力。WalMiner最新版本还收费了。此类型的工具最多只能解析出执行过的sql语句，无法监控特定的表的数据变更，并且还需要基于sql语句，
继续二次开发获得最终的CDC。

参考文档：
https://gitee.com/movead/XLogMiner
http://www.postgres.cn/v2/news/viewone/1/385
http://www.postgres.cn/v2/news/viewone/1/386
http://www.postgres.cn/v2/news/viewone/1/306
http://www.postgres.cn/v2/news/viewone/1/419

```

#### 2  新思路

```
https://bryteflow.com/postgres-cdc-6-easy-methods-capture-data-changes/

文章提到过pg的触发器，可以监控特定的几个表的CDC，然后把新，旧值插入预先构建好的表中。
跟MongoDB的oplog一样，直接读取方便后续解析。
```

#### 3 potgresql 触发器概念

````
PostgreSQL 触发器（trigger）是一种特殊的函数，当某个数据变更事件（INSERT、UPDATE、DELETE 或者 TRUNCATE）DML
或者数据库事件（DDL 语句）发生时自动执行，而不是由用户或者应用程序进行调用

基于某个表或者视图数据变更的触发器被称为数据变更触发器（DML 触发器），
基于数据库事件的触发器被称为事件触发器（DDL 触发器）。一般我们更多使用的是数据变更触发器。
````

#### 4 创建触发器步骤

PostgreSQL 触发器的创建分为两步：

- 1 使用 CREATE FUNCTION 语句创建一个触发器函数。
- 2 使用 CREATE TRIGGER 语句将该函数与表进行关联。


```text
触发器的详细介绍和例子，快速了解，强推
https://zhuanlan.zhihu.com/p/459132312  
```


#### 4 创建的详细步骤

1 创建测试demo(**本地测试用**)

```sql
# 1 在db psotgres 创建employees表
create table employees(employee_id int primary key,name varchar(20),phone varchar(20),job varchar(10));

# 2 切换到数据库 cdc_db 创建 employees_history表  
create table employees_history(id serial primary key,employee_id int null,name varchar(20) null,phone varchar(20) null,job varchar(10) null,action_type varchar(10) not null,change_dt timestamp not null);

# 3 定义一个触发器函数
create or replace function track_employees_change() returns trigger as $$
begin
    if tg_op = 'INSERT' then
      insert into employees_history(employee_id,name,phone,job,action_type,change_dt)
      values(new.employee_id,new.name,new.phone,new.job,'insert',current_timestamp);
    elsif tg_op = 'UPDATE' then
      insert into employees_history(employee_id,name,phone,job,action_type,change_dt)
      values(old.employee_id,old.name,old.phone,old.job,'update',current_timestamp);
    end if;
    return new;
end; $$
language plpgsql;

# 4 关联触发器函数
create trigger trg_employees_change 
  after insert or update
  on employees
  for each row
  execute function track_employees_change();
  
# 5 插入一些测试数据
insert into employees(employee_id,name,phone,job) values(1,'tt1','123221','销售1');
insert into employees(employee_id,name,phone,job) values(2,'tt3','12323321','总监');
update employees set name='小五' where employee_id = 1;



# 管理触发器，
select * from pg_trigger where tgname = 'trg_employees_change';  # 查看

drop trigger trg_employees_change on employees;        # 删除对应的触发器

ALTER TABLE table_name {ENABLE | DISABLE} TRIGGER {trigger_name | ALL | USER}; # 禁用启用
   
```


2 新建一个表，用于记录业务三个表的变更(**涉及到业务，只举个例子**)

````sql
create table cfp_audit_change(
       table_name text not null,
       action_tstamp timestamp with time zone not null default current_timestamp,
       action TEXT NOT NULL check (action in ('i','d','u')),
       original_data text null,
       new_data text,
       query text
);
````


3 创建触发器函数

```sql
create or replace function track_cfp_change() returns trigger as $$
DECLARE
v_old_data TEXT;
v_new_data TEXT;
begin 
  if tg_op = 'INSERT' then
    v_new_data := ROW(NEW.*);
    insert into cfp_audit_change(table_name,action,new_data,query)                         values(TG_TABLE_NAME::TEXT,'i',v_new_data::TEXT,current_query());
  elsif tg_op = 'UPDATE' then
    v_old_data := ROW(OLD.*);
    v_new_data := ROW(NEW.*);
    insert into cfp_audit_change(table_name,action,original_data,new_data,query) 
    values(TG_TABLE_NAME::TEXT,'u',v_old_data::TEXT,v_new_data::TEXT,current_query());
  end if;
    return new;
end; $$
language plpgsql;


# 要监控那几个表。就创建关联触发器函数
create trigger trg_cfp_change1 
  after insert or update
  on xxxx1
  for each row
  execute function track_cfp_change();
 
create trigger trg_cfp_change2 
  after insert or update
  on xxxx2
  for each row
  execute function track_cfp_change();

create trigger trg_cfp_change3 
  after insert or update
  on xxxx3
  for each row
  execute function track_cfp_change();
  
```

