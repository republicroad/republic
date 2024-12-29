# postgresql应用技巧


## 幂等


实际际业务中, 为了更好的实现幂等的业务逻辑, 最好在数据存储支持这些功能. 第一可以简化系统设计, 第二可以提高性能.
一旦数据库没有实现修改幂等的逻辑, 那么在应用中需要进行类似于mvcc多版本管理的特性. 比如乐观锁(cas), 悲观锁(数据库的行锁和单独的分布式锁). 一旦数据库的事务和分布式锁之间相互嵌套, 极容易发生死锁. 一般这种情况的最佳实践是让分布式锁包裹(想想这事为什么, 快设备保护慢设备).

**如果数据库层面能支持一些操作的原子性, 那么就可以减少这种复杂的锁的设计.**

- on conflict 相关语句
- [Updating multiple values at a time](https://dba.stackexchange.com/questions/246753/updating-multiple-values-at-a-time)



## sharding && partition


- [notion:the-great-re-shard](https://www.notion.so/blog/the-great-re-shard)
- [notion:sharding-postgres-at-notion](https://www.notion.so/blog/sharding-postgres-at-notion)
- [adding Postgres capacity (again) with zero downtime](https://www.notion.so/blog/the-great-re-shard)



## pgbouncer

[how-achieve-postgresql-high-availability-pgbouncer](https://severalnines.com/blog/how-achieve-postgresql-high-availability-pgbouncer/)
[pgBouncer Setup, Extended Architecture, Use-cases and leveraging SO_REUSEPORT](https://www.enterprisedb.com/postgres-tutorials/pgbouncer-setup-extended-architecture-use-cases-and-leveraging-soreuseport)

