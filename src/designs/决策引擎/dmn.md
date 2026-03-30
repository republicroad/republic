
# DMN  && BPMN

BPMN 是流程编排引擎。 DMN 是符号决策引擎。
DMN 是 BPMN 的流程编排用于决策的补充。

## DMN

### DMN DSL

DMN 中默认的是关于决策表，评分卡等传统的决策概念。为了表达更灵活的概念，需要在决策表和评分卡中引入 `feel` 表达式来进行求值和判断，这样可以扩展决策的边界。

## dmn udf

可以在dmn中支持宿主语言的自定义函数.


## [together 规则引擎](https://www.rongtek.com/col.jsp?id=115)

together是使用 dmn 来实现的。 zen-engine与dmn在决策表节点上非常相似，主要区别在于 zen-engine 提供了分支节点，进而可以实现非线性逻辑的决策。

together是使用 dmn 来实现的。 zen-engine与dmn在决策表节点上非常相似，主要区别在于 zen-engine 提供了分支节点，进而可以实现非线性逻辑的决策。

DMN三大标准:

- 决策需求图    
- Feel lang
- 盒装表达式

### 经典案例

together 规则引擎blog包含大量的行业案例，可以尝试使用 jdm 来实现.
[动态预算费控决策模型](https://www.rongtek.com/nd.jsp?fromColId=109&id=116#_np=109_6062)  
[Together决策智能DI模型示例](https://www.rongtek.com/nd.jsp?fromColId=109&id=115#_np=109_6062)  
[休假申请天数计算模型](https://www.rongtek.com/nd.jsp?fromColId=109&id=114#_np=109_6062)  
[电信运营商返佣计算模型](https://www.rongtek.com/nd.jsp?fromColId=109&id=113#_np=109_6062)  
[生产成本计算模型](https://www.rongtek.com/nd.jsp?fromColId=109&id=105#_np=109_6062)  
[早孕诊断算法设计](https://www.rongtek.com/nd.jsp?fromColId=109&id=103#_np=109_6062)  
[贷款资格预审决策模型](https://www.rongtek.com/nd.jsp?fromColId=109&id=102#_np=109_6062)  
[金融贷款产品比较算法设计](https://www.rongtek.com/nd.jsp?fromColId=109&id=101#_np=109_6062)  
[乘客换乘服务算法](https://www.rongtek.com/nd.jsp?fromColId=109&id=100#_np=109_6062)  
[Together规则引擎中的会员积分规则模型设计](https://www.rongtek.com/nd.jsp?fromColId=109&id=96#_np=109_6062)  


### 行业

[金融解决方案](https://www.rongtek.com/col.jsp?id=116)  
[保险行业解决方案](https://www.rongtek.com/col.jsp?id=121)  
[供应链解决方案](https://www.rongtek.com/col.jsp?id=123)  
[物联网解决方案](https://www.rongtek.com/col.jsp?id=122)  
[医疗解决方案](https://www.rongtek.com/col.jsp?id=120)  
[制造业解决方案](https://www.rongtek.com/col.jsp?id=125)  
[企业风控解决方案](https://www.rongtek.com/col.jsp?id=127)  
[企业内控解决方案](https://www.rongtek.com/col.jsp?id=126)  
[电商解决方案](https://www.rongtek.com/col.jsp?id=128)  

## 资料


[Together规则引擎（决策引擎）DMN培训教程基础篇](https://space.bilibili.com/560662443/upload/video)
[DMN 决策要求图(DRD)组件](https://docs.redhat.com/zh-cn/documentation/red_hat_decision_manager/7.13/html/developing_decision_services_in_red_hat_decision_manager/dmn-drd-components-ref_dmn-models)
[DMN 模型示例](https://docs.redhat.com/zh-cn/documentation/red_hat_decision_manager/7.13/html/developing_decision_services_in_red_hat_decision_manager/dmn-model-example-ref_dmn-models)  