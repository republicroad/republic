
CEP(Complex Event Processing): 复杂事件处理。

在决策引擎的场景中，经常需要对输入的事件按照时间轴去刻画一些特征，比如按照不同的时间窗口大小和移动步长去描述其中的特征和规律。再结合任意的空间维度，就会组合成为CEP。 

比如常见的当日积分累加，七天内的金额累加，三十天内的转账是否连续递增等需求。

drools和 flinkcep 都为 CEP 设计了专有的语法:

TODO

**brde 现在只提供CEP的算子(函数), 目前先不设计语法。**


## 资料

[redpanda: 复杂事件处理——架构及其他实际考量](https://www.redpanda.com/guides/event-stream-processing-complex-event-processing#:~:text=Summary%20of%20key%20complex%20event%20processing%20concepts&text=A%20specialized%20technique%20for%20analyzing,to%20identify%20patterns%20and%20trends.&text=Origins%20of%20events%20in%20a,generate%20data%20streams%20for%20analysis.&text=Standard%20techniques%20or%20rules%20in,%2C%20aggregation%2C%20and%20relationship%20identification.&text=Architectural%20models%20in%20CEP%2C%20choosing,servers/nodes%20(distributed).)

### timeplus

[Complex Event Processing Made Easy with Streaming SQL + UDF](https://www.timeplus.com/post/cep-with-streaming-sql-udf)  
[timeplus: 技术分享｜使用流式SQL+UDF简化复杂事件处理](https://zhuanlan.zhihu.com/p/1968754849265980826)  
https://gist.github.com/gangtao/44fce0d019be441f94e19503c0923cf7  
### [flink cep](https://nightlies.apache.org/flink/flink-docs-release-2.0/docs/dev/table/sql/queries/match_recognize/#known-limitations)  

[# FlinkCEP - Flink的复杂事件处理](https://nightlies.apache.org/flink/flink-docs-stable/zh/docs/libs/cep/)  
[第十一章，Flink CEP简介](https://confucianzuoyuan.github.io/flink-tutorial/book/chapter11-00-00-%E7%AC%AC%E5%8D%81%E4%B8%80%E7%AB%A0%EF%BC%8CFlink-CEP%E7%AE%80%E4%BB%8B.html#%E7%AC%AC%E5%8D%81%E4%B8%80%E7%AB%A0flink-cep%E7%AE%80%E4%BB%8B)
[# 动态Flink CEP电商实时预警系统](https://help.aliyun.com/zh/flink/realtime-flink/use-cases/build-an-e-commerce-early-warning-system-with-flink-cep)
[基于 Flink 构建大规模实时风控系统在阿里巴巴的落地](https://xie.infoq.cn/article/37d249a7cd16de527b4a39051)  
[详解 Flink CEP（以直播平台监控用户弹幕为例）](https://xie.infoq.cn/article/b2ffe9c41ecb4647a1ed8a340)  


### drools cep

CEP用例和业务规则用例共享若干需求和目标。

从业务的角度出发，业务规则的定义通常基于事件触发的场景来定义。在下面的例子中，事件形成根据业务规则的基础：

- 在一个算法交易应用中，如果证券价格高于开盘价格百分之X以上，则规则执行操作。在股票交易应用中，价格上涨被标记为事件。
- 在一个监控应用中，如果服务器机房的温度在Y分钟内增长了X摄氏度，则规则执行操作。传感器的示数被标记为事件。

从技术角度来看，业务规则评估和CEP有下列相似之处：

- 业务规则评估和CEP都需要和企业基础设施和应用无缝集成，这一条对于生命周期管理，审计和安全尤其重要。
- 业务规则评估和CEP都有函数需求，例如模式匹配，还有无函数需求，例如响应时间限制和查询规则解释

CEP场景具有以下特征：

- 通常用来处理大量事件，但是只有少量事件是相关的。
- 时间通常是不变的，并且代表状态改变的记录。
- 规则和查询针对事件运行，并且必须对检测出的事件模式做出反应
- 先关事件通常拥有很强的时间关系
- 单条事件没有优先级。CEP系统会优先考虑相关事件的模式以及他们之间的关系
- 事件通常需要被组合与聚合

鉴于这些普通CEP场景特点，Drools的CEP系统支持下面的特性和功能，优化了事件处理：

- 用合适的语义处理事件
- 事件监测，关联，聚合和组合
- 事件流处理
- 时间约束建模事件之间的时间关系
- 重大事件的滑动窗口
- 会话范围的统一时钟
- CEP用例所需要的事件量
- 反应性规则
- 用于将事件输入到Drools的适配器（管道）

[# 19、Drools 规则引擎 - CEP 复杂事件处理](https://ddkk.com/zhuanlan/other/drools/1/19.html)  
[# 20、Drools 规则引擎 - CEP 的属性更改设置和监听器](https://ddkk.com/zhuanlan/other/drools/1/20.html)  
[# 21、Drools 规则引擎 - CEP 的时间操作](https://ddkk.com/zhuanlan/other/drools/1/21.html)  
[# 22、Drools 规则引擎 - CEP 会话时钟，事件流和切入点](https://ddkk.com/zhuanlan/other/drools/1/22.html)  
[# 23、Drools 规则引擎 - CEP 滑动窗口和内存管理](https://ddkk.com/zhuanlan/other/drools/1/23.html)  
[# 24、Drools 规则引擎 - CEP 查询、事件监听、调试日志和性能调优](https://ddkk.com/zhuanlan/other/drools/1/24.html)  

[drools: Fact 对象详解](https://juejin.cn/post/7519454363826110503)
[drools: 从规则引擎到复杂事件处理](https://holbrook.github.io/2013/12/22/from_rule_to_cep.html)  
[Drools Fusion (CEP) Example 和 关键概念](https://www.cnblogs.com/zhwbqd/p/4212766.html "发布于 2015-01-09 10:52")  
[Drools在复杂事件处理中的应用与实战详解](https://blog.csdn.net/weixin_42376614/article/details/154604018)  

这个是drools cep讲的最好的:
[Drools在复杂事件处理中的应用与实战详解](https://blog.csdn.net/weixin_42376614/article/details/154604018)