
网关是一种能够在不同[网络](https://zh.wikipedia.org/wiki/%E7%BD%91%E7%BB%9C%E4%BC%A0%E8%BE%93%E5%8D%8F%E8%AE%AE "网络传输协议")或[协议](https://zh.wikipedia.org/wiki/%E5%8D%8F%E8%AE%AE%E6%A0%88 "协议栈")之间进行[数据交换](https://zh.wikipedia.org/wiki/%E6%95%B0%E6%8D%AE%E4%BA%A4%E6%8D%A2 "数据交换")的设备或服务器。网关可以实现不同网络之间的互联互通，也可以实现不同协议之间的转换和适配。在应用层面, 一般是指七层应用层协议的路由入口。

## 网关发展

随着业务的发展, 规模和复杂性与日俱增. 业务上需要进行更多的组织拆分, 功能拆分, 进而形成了大量的服务, 为了对这些服务进行统一的管理, 就有了`网关`的概念. 

对于流量的入口, 我们一般称之为流量网关, 流量网关主要用于控制和管理网络流量，包括负载均衡、流量限制、数据过滤等功能。它通常位于网络架构中的最前端，作为应用程序和网络之间的入口。流量网关的主要作用是确保网络的可用性和可靠性。

各个业务域的入口一般称为API网关, API网关是一种集成了配置发布、环境管理、接入认证，用户鉴权、访问控制等功能的API管理和服务治理的工具。使用API网关托管API，即可高效，安全、低成本的管理服务，同时还提供API的转换、聚合、过滤器等服务。API网关作为请求的单一入口点，将请求分配给相应的服务，然后收集结果并将其传递给请求者。而不是让客户单独请求访问每项微服务。


| 网关                                            | 痛点                                                                                                                                 | 优势                                                                                                                                                 | 语言     |
| --------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------- | ------ |
| NGINX                                         | 1. 修改配置需要 Reload 才能生效，跟不上云原生的发展。                                                                                                   | 1. 老牌应用；  <br>2. 稳定可靠，久经考验；  <br>3. 高性能。                                                                                                           | c      |
| Apache APISIX                                 | 1. 文档不够丰富和清晰，需要待改进。                                                                                                                | 1. Apache 基金会顶级项目；  <br>2. 技术架构更贴合云原生； 3. 性能表现优秀；  <br>4. 生态丰富； 5. 除了支持 Lua 开发插件外，还支持 Java、Go、Python、Node 等语言插件。                                   | c,lua  |
| Kong                                          | 1. 默认使用 PostgreSQL 或 Cassandra 数据库，使得整个架构非常臃肿，并且会带来高可用的问题；  <br>2. 路由使用的是遍历查找，当网关内有超过上千个路由时，它的性能就会出现比较急剧的下降；  <br>3. 一些重要功能是需要付费的。 | 1. 开源 API 网关的鼻祖，用户数众多；  <br>2. 性能满足大部分用户的需求；  <br>3. 生态丰富；  <br>4. 支持 Lua 和 Go 开发插件。                                                               | c,lua  |
| Envoy                                         | 1. 使用 C++，二次开发难度大；  <br>2. 除了 C++ 开发 filter 外，还支持 WASM 和 Lua。                                                                      | 1. CNCF 毕业项目 更适合服务网格场景多语言架构部署。                                                                                                                     | c++    |
| Spring Cloud Gateway                          | 1. 虽然 Spring 社区成熟，但是 Gateway 资源缺乏。                                                                                                 | 1. 内置了非常多的开箱即用功能，并且都可以通过 SpringBoot 配置或者手工编码链式调用来使用；  <br>2. Spring 系列可扩展性强，易配置，可维护性好；  <br>3. Spring 社区成熟；  <br>4. 简单易用；  <br>5. 对于 Java 技术栈来说方便。 | java   |
| [traefik](https://github.com/traefik/traefik) | GC 抖动, p99时延抖动                                                                                                                     | 功能扩展方便, 性能高                                                                                                                                        | golang |
| 百度 BFE                                        | GC 抖动, p99时延抖动                                                                                                                     | 功能扩展方便, 性能高                                                                                                                                        | golang |
[api网关对比](https://apisix.apache.org/zh/blog/2023/03/08/why-do-microservices-need-an-api-gateway/#%E4%B8%BB%E6%B5%81%E7%BD%91%E5%85%B3%E9%80%89%E6%8B%A9)



### 网关功能总结

- 请求路由
- 负载均衡
- 动态上游
- 热更新(不断连接)
- 配置中心
- 熔断, 限流, 降级
- 自定义逻辑(灰度发布，服务发现，HTTP 基本认证，密钥认证，CORS，私有协议扩展)
- 可观测性
- 混合架构(虚拟机与云原生容器)


### 网关融合

目前的架构基本上南北用流量网关接入, 微服务间东西流量用api网关来负载. 随着安全, 合规审计的之类的垂直需求愈加频繁的背景下, 东西南北网关融合的趋势愈加明显. 既想要流量网关的性能, 又想要api网关的灵活和扩展诉求, 随之而来的是云原生的网关, 对业务网关和流量网关以及*安全网关*的进行了高度集成. 显著地降低了部署和运维成本.



### 网关异常流量识别(安全网关增强)

- 协议破解检测
- web环境检测
- 小程序环境检测
- app环境检测
- 客户端请求签名


瑞数          botgate
威胁猎人   api安全管控平台



## 参考资料

[什么是API网关？](https://info.support.huawei.com/info-finder/encyclopedia/zh/API%E7%BD%91%E5%85%B3.html)
[Deploying NGINX as an API Gateway, Part 1](https://www.nginx.com/blog/deploying-nginx-plus-as-an-api-gateway-part-1/)
[Cloudflare弃用NGINX，改用Rust编写的Pingora，你怎么看？](https://www.zhihu.com/question/554595029)

[亿级流量架构网关设计思路，常用网关对比](https://developer.aliyun.com/article/1036808?spm=a2c6h.14164896.0.0.49bd47c54bushS&scm=20140722.S_community@@%E6%96%87%E7%AB%A0@@1036808._.ID_1036808-RL_%E4%BA%BF%E7%BA%A7%E6%B5%81%E9%87%8F%E6%9E%B6%E6%9E%84%E7%BD%91%E5%85%B3%E8%AE%BE%E8%AE%A1%E6%80%9D%E8%B7%AF%EF%BC%8C%E5%B8%B8%E7%94%A8%E7%BD%91%E5%85%B3%E5%AF%B9%E6%AF%94-LOC_search~UND~community~UND~item-OR_ser-V_3-P0_0)
[亿级流量架构网关设计思路，常用网关对比2](https://developer.aliyun.com/article/1036809?spm=a2c6h.14164896.0.0.49bd47c54bushS)
https://edgar615.github.io/api-gateway-flow.html

#### 视频资料

人月聊IT(所在公司开发了一个云原生的网关项目, 现在无限期停止维护, 公司主要做项目交付)
[一张图讲解微服务-注册中心，微服务网关和API网关区别](https://www.bilibili.com/video/BV1MP411p7fA/?spm_id_from=333.337.search-card.all.click&vd_source=8f2e8d9afb969c72b313832ed92dc193)

IT老齐的咨询建议:
[上万租户SAAS系统大规模Nginx动态路由优化方案](https://www.bilibili.com/video/BV1Ro4y167An/?spm_id_from=333.337.search-card.all.click&vd_source=8f2e8d9afb969c72b313832ed92dc193)
[【IT老齐321】20分钟上手高性能动态网关APISIX](https://www.bilibili.com/video/BV1RV4y1S7CU/?spm_id_from=333.337.search-card.all.click&vd_source=8f2e8d9afb969c72b313832ed92dc193)

#### apisix conf 相关分享
[Apisix集中式流控与Sentinel分布式流控](https://www.bilibili.com/video/BV1vo4y1T7vY/?spm_id_from=333.337.search-card.all.click&vd_source=8f2e8d9afb969c72b313832ed92dc193)
[apisix 故障排查: Apache APISIX 在希沃网关的应用与实践](https://www.bilibili.com/video/BV1sm4y1F7Nh/?spm_id_from=333.337.search-card.all.click&vd_source=8f2e8d9afb969c72b313832ed92dc193)
[vivo 用数据库替换etcd: Apache APISIX 落地实践分享](https://www.bilibili.com/video/BV1Qe4y177sm/?spm_id_from=autoNext&vd_source=8f2e8d9afb969c72b313832ed92dc193)
[etcd initial cluster status 开始检查这个状态设置为空或者为new, 以后每次部署设置为 exists: 景顺长城基于 Apache APISIX 在金融云原生的生产实践](https://www.bilibili.com/video/BV1mc411L7TA?t=1118.1)
[基于 Apache APISIX 的蓝鲸 API 网关设计与应用](https://www.bilibili.com/video/BV1aT4y1q7eg/?spm_id_from=333.337.search-card.all.click&vd_source=8f2e8d9afb969c72b313832ed92dc193)
[爱奇艺基于 Apache APISIX 的 API 网关落地实践](https://www.bilibili.com/video/BV1Qq4y1M7bK/?spm_id_from=autoNext&vd_source=8f2e8d9afb969c72b313832ed92dc193)
[Apache APISIX在有赞的落地实践](https://www.bilibili.com/video/BV1Y64y1h7A7/?spm_id_from=333.337.search-card.all.click&vd_source=8f2e8d9afb969c72b313832ed92dc193)

### 概念对比

[NGINX 与 Kong 的痛点](https://apisix.apache.org/zh/blog/2022/07/30/why-we-need-apache-apisix/)
[API 网关 Kong 实战](https://xie.infoq.cn/article/10e4dab2de0bdb6f2c3c93da6)
- [视频: API 网关 Kong 实战](https://www.bilibili.com/video/BV1jY411p7qm/?p=4&vd_source=8f2e8d9afb969c72b313832ed92dc193)
[云原生 API 网关 APISIX 入门教程](https://mp.weixin.qq.com/s/cN5c4QgJgeXWzah9WKFrEg)
- [视频: 云原生 API 网关 APISIX 入门教程](https://www.bilibili.com/video/BV1cM411S7TH?p=8&vd_source=8f2e8d9afb969c72b313832ed92dc193)
- [云原生API网关Apache APISIX实战教程，Nginx和Kong做不到的我APISIX来干](https://www.bilibili.com/video/BV1c14y1N7iM/?p=3&spm_id_from=333.788.top_right_bar_window_history.content.click)

[多层网关已成过去，网关多合一成潮流，网关改造正当时｜Higress 1.0 正式发布](https://www.infoq.cn/article/Ez2pf3Zcv0fmeSwzeF3s?utm_source=related_read_bottom&utm_medium=article)
[开源云原生融合网关 Hango 的最新实践与思考](https://www.infoq.cn/article/49azpb4XyqxCBNI1Y4di?utm_source=related_read&utm_medium=article)
[国内航空巨头如何从 NGINX 迁移至 APISIX？](https://www.infoq.cn/article/HVLkNv8FAjSjG2ob53XH?utm_source=related_read&utm_medium=article)

[Higress nginx apisix: 云原生网关当道，三大主流厂商如何“竞技”？](https://www.infoq.cn/article/k3ssXQGz0pq4pHaLbdWd?utm_source=related_read_bottom&utm_medium=article)
[阿里巴巴开源下一代云原生网关 Higress：基于 Envoy，支持 Nginx Ingress 零成本快速迁移](https://www.infoq.cn/article/eDP9ttYCRgbWETL6dT75?utm_source=related_read_bottom&utm_medium=article)
[Edith网关——面向小红书亿级DAU的网关大规模实践-陈华昌](https://www.sgpjbg.com/baogao/153222.html)
[MSE: 云原生网关将流量网关、微服务网关、安全网关三合一，被誉为下一代网关](https://xie.infoq.cn/article/fa0171ca2d78bb2b337024f6b?utm_source=related_read_bottom&utm_medium=article)
[hango: 为什么我们需要全能力云原生网关](https://www.infoq.cn/article/ZltFTNz44gRHYxNIUw1B?utm_campaign=geek_search&utm_content=geek_search&utm_medium=geek_search&utm_source=geek_search&utm_term=geek_search)
[网易hango](https://github.com/hango-io/hango-gateway)
[Loggly: Benchmarking 5 Popular Load Balancers: Nginx, HAProxy, Envoy, Traefik, and ALB](https://www.loggly.com/blog/benchmarking-5-popular-load-balancers-nginx-haproxy-envoy-traefik-and-alb/)
[slack: Migrating Millions of Concurrent Websockets to Envoy](https://slack.engineering/migrating-millions-of-concurrent-websockets-to-envoy/)

[Envoy 架构](https://icloudnative.io/envoy-handbook/docs/gettingstarted/architecture/)

#### 限流应用

[kong golang: 服务限流在多项目下的方案设计与落地实践](https://www.bilibili.com/video/BV1se41117go/?buvid=XY09E1B018DB4714D9199E7BB35EC2E15E122&from_spmid=tm.recommend.0.0&is_story_h5=false&mid=6kqihrl4D1JHmaSUPkMkZg%3D%3D&p=1&plat_id=116&share_from=ugc&share_medium=android&share_plat=android&share_session_id=6fc5dd26-d0b7-46a8-81d3-4fb4bc53c92c&share_source=COPY&share_tag=s_i&spmid=united.player-video-detail.0.0&timestamp=1705716947&unique_k=Vhn1KP0&up_id=2094867263&vd_source=5801d88e2ad7cf00f270560298a1ff4f)


#### java网关

[业务网关的一些场景: 业务网关的落地实践](https://www.infoq.cn/article/cAcwMUNMJMQpIxGJYkcS?spm=a2c6h.12873639.article-detail.8.743412ea1fpfhp)
[得物自研API网关实践之路](https://www.51cto.com/article/763146.html)
[美团: 百亿规模API网关服务Shepherd的设计与实现](https://tech.meituan.com/2021/05/20/shepherd-api-gateway.html)
[美团网关服务编排_流程引擎:美团服务体验平台对接业务数据的最佳实践-海盗中间件](https://tech.meituan.com/2018/07/26/sep-service-arrange.html)
[放弃 Spring Cloud Gateway！Apache APISIX 在「还呗」业务中的技术实践](https://www.infoq.cn/article/9QV9xxWgTWS9UdQo7kwd?utm_source=related_read_bottom&utm_medium=article)


#### golang 网关

[小红书Edith](https://new.qq.com/rain/a/20231222A04SM500)