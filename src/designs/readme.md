
这里记录各种设计的随想。

# DMN  && BPMN

dmn 是 bpmn 的流程编排用于决策的补充。
工作流和决策能否重新整体设计，zen engine 目前已经超过了 dmn 概念的外延。能否重新去考虑工作流和决策的实现。因为某种意义上，决策和工作流在某些场景会成为彼此的依赖条件或者依赖触发动作。

## DMN

### DMN DSL

DMN 中默认的是关于决策表，评分卡等传统的决策概念。为了表达更灵活的概念，需要在决策表和评分卡中引入表达式来进行求值和判断，这样可以扩展决策的边界。

- feel lang
- zen expression

如果这些表达式可以支持外部的**自定义函数**，可以进一步增加表达式的适用范围。

## dmn function

[presto Functions and Operators ](https://prestodb.io/docs/current/functions.html)    
[duckdb Functions](https://duckdb.org/docs/sql/functions/overview.html)

未来可以考虑在 zen-expresion 或者 feel lang 加入一些数据的处理或者聚合函数。


# libuv

https://docs.libuv.org/en/v1.x/guide/introduction.html  
https://docs.libuv.org/en/v1.x/guide.html  

https://nikhilm.github.io/uvbook/index.html

## nim uv

https://github.com/dadadani/uva



# nim

https://www.reddit.com/r/nim/comments/1e0ztyk/what_libraries_would_benefit_the_ecosystem_the/  
https://www.reddit.com/r/nim/comments/1g9h1fh/any_backend_framework_that_is_still_maintained/  
https://narimiran.github.io/nim-basics/  

https://github.com/tomnomnom/gron  
https://www.edgedb.com/  
## nim import namespace

https://narimiran.github.io/2019/07/01/nim-import.html  


## nir

https://news.ycombinator.com/item?id=37719971  
https://github.com/nim-lang/Nim/pull/22777  

# nimskull

https://github.com/nim-works/nimskull  

https://nim-works.github.io/nimskull/debug.html  
https://www.reddit.com/r/nim/comments/1g38euq/is_nim_developer_experience_good_i_dont_think_so/  
https://www.reddit.com/r/nim/comments/1fxduzh/need_clarification_from_nim_community/  
