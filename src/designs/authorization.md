
# authorization

authorization 授权，用于在认证用户身份后，使用者对系统的访问控制(access control).  
授权系统可以赋予和验证用户以什么动作去访问系统资源。

## RBAC

Role-Based Access Control (RBAC) 基于角色的访问控制系统, 是一种根据授权用户在组织内的角色来限制其系统访问权限的方法。RBAC 并非直接将权限分配给单个用户，而是将用户分组到角色中，并为这些角色分配特定的权限。

在设计上，一般会设计 用户--角色--权限(user--role--permission)的映射系统.

- **Users:** 系统使用者，用于分配一个或者多个角色.

- **Roles:** 代表系统内的工作职能或职责（例如“Manager”、“Accountant”、“Developer”）

- **Permissions:** 定义具有特定角色可以执行哪些操作 (e.g., "Read," "Write," "Delete"). 


本质上，角色既相当于用户组，又代表了用户组功能的集合.

## ABAC

[What Is Attribute-Based Access Control (ABAC)?](https://www.permit.io/blog/what-is-abac)  

基于属性的访问控制 (ABAC) 是一种授权模型，它根据环境条件以及用户和资源属性授予访问权限。ABAC 的使用与现代应用程序日益增长的复杂性和多样性相一致，其中传统的访问控制方法无法满足复杂且不断变化的安全要求。

基于属性的访问控制 (ABAC) 根据应用于属性（或特征）而非角色的条件来确定访问权限，从而允许定义细粒度的复杂访问控制规则。

ABAC 能够适应复杂的分布式环境，非常适合需要高度定制化和上下文敏感的访问决策应用。它在用户角色不足以涵盖所有访问需求的场景中表现出色。




一般来说, ABAC权限系统最后都会实现或者引入一套用于判定的表达式系统. 在表达式中去判断是否满足对应的属性.

## ReBAC

基于关系的访问控制系统, relationship-based access control. 本质上是多个向量实例的访问控制. 比如 (alice, doc1) 表示 alice 拥有对 doc1 的权限, (alice, read, doc1) 表示  alice 拥有对 doc1 的读权限, 这种权限本质上是一种多个维度(subject, action ,resource)的任意笛卡尔积的子集权限。

基于 ReBAC 权限系统的特点, 往往在实现时需要将这种关系以行级别数据实例的方式存储在数据库中, 这个权限的校验就变成了查找数据库中匹配的行, 如果存在则说明有此关系权限，如果不存在说明无此权限。

## PBAC

以上三种权限系统的综合版本，policy-based access control，称为基于策略的访问控制系统实现.


## 基于 zen-engine 实现PBAC的中央权限校验系统



## 参考资料

[A Guide to Bearer Tokens: JWT vs. Opaque Tokens](https://www.permit.io/blog/a-guide-to-bearer-tokens-jwt-vs-opaque-tokens)  

[Authentication and Authorization Best Practices](https://blog.gitguardian.com/authentication-and-authorization/)  
[casbin 中文](https://casbin.org/zh/)  
https://casbin.org/zh/docs/how-it-works
[casbin 编辑器](https://casbin.org/zh/editor)  
https://casbin.org/zh/docs/understanding-casbin-detail
### permit.io

[Role-Based Access Control (RBAC)](http://permit.io/rbac?_gl=1*vlim4i*_gcl_au*MTg2MTk3NDM3OC4xNzU1MDQ4NDE1*_ga*ODM5MTI1NDkwLjE3NDM2NDQ4OTU.*_ga_SPFKH5NXDE*czE3NTUwNTYyODIkbzkkZzEkdDE3NTUwNTYzNDIkajYwJGwwJGgxOTI1Mzk1ODM0 "http://permit.io/rbac")
https://www.permit.io/rbac
https://www.permit.io/abac

### openfga

https://openfga.dev/docs/concepts  
https://openfga.dev/docs/configuration-language  
https://github.com/openfga/openfga  
[openfga: Relationship-based access control made fast, scalable, and easy to use.](https://openfga.dev/)  
[使用 RBAC、ReBAC 和 ABAC 构建授权系统的完整指南](https://levelup.gitconnected.com/complete-guide-to-building-authorization-systems-using-rbac-rebac-and-abac-0a2ce5311d25)  

### 实际例子

[飞书IAM系统: 3年一线大厂高级开发，应变能力很强，定级P6+！](https://www.bilibili.com/video/BV1wHoiY9EBt/?spm_id_from=333.1387.upload.video_card.click&vd_source=8f2e8d9afb969c72b313832ed92dc193)  
[MuYun｜复杂系统的权限设计](https://www.bilibili.com/video/BV1iSDUYhEuh/?spm_id_from=333.337.search-card.all.click&vd_source=8f2e8d9afb969c72b313832ed92dc193)  
[IT老齐412】Mybaits-plus动态数据权限解决方案](https://www.bilibili.com/video/BV1Dh4y1i7Nm/?spm_id_from=333.337.search-card.all.click&vd_source=8f2e8d9afb969c72b313832ed92dc193)  
[Authorization Explained: When to Use RBAC, ABAC, ACL & More](https://www.youtube.com/watch?app=desktop&v=DT6Zy1X3ytM&t=552s)  

### tanstack router practice

[Contexts and Authenticated Routes deeper in the route tree #2032](https://github.com/TanStack/router/discussions/2032)  
