
网关是一种能够在不同[网络](https://zh.wikipedia.org/wiki/%E7%BD%91%E7%BB%9C%E4%BC%A0%E8%BE%93%E5%8D%8F%E8%AE%AE "网络传输协议")或[协议](https://zh.wikipedia.org/wiki/%E5%8D%8F%E8%AE%AE%E6%A0%88 "协议栈")之间进行[数据交换](https://zh.wikipedia.org/wiki/%E6%95%B0%E6%8D%AE%E4%BA%A4%E6%8D%A2 "数据交换")的设备或服务器。网关可以实现不同网络之间的互联互通，也可以实现不同协议之间的转换和适配。在应用层面, 一般是指七层应用层协议的路由入口。

## 网关设计


一般用作承载请求的入口, 

- 流量网关
	- 南北流量网关
	- 东西流量网关
- 微服务网关
- api网关

[API网关 - 流量网关和业务网关](https://edgar615.github.io/api-gateway-flow.html)



## 参考资料

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

[多层网关已成过去，网关多合一成潮流，网关改造正当时｜Higress 1.0 正式发布](https://www.infoq.cn/article/Ez2pf3Zcv0fmeSwzeF3s?utm_source=related_read_bottom&utm_medium=article)
[开源云原生融合网关 Hango 的最新实践与思考](https://www.infoq.cn/article/49azpb4XyqxCBNI1Y4di?utm_source=related_read&utm_medium=article)
[国内航空巨头如何从 NGINX 迁移至 APISIX？](https://www.infoq.cn/article/HVLkNv8FAjSjG2ob53XH?utm_source=related_read&utm_medium=article)

[Higress nginx apisix: 云原生网关当道，三大主流厂商如何“竞技”？](https://www.infoq.cn/article/k3ssXQGz0pq4pHaLbdWd?utm_source=related_read_bottom&utm_medium=article)
[Edith网关——面向小红书亿级DAU的网关大规模实践-陈华昌](https://www.sgpjbg.com/baogao/153222.html)
[云原生时代的通用流量入口 高性能云原生网关实践-韩佳浩.pdf](https://www.sgpjbg.com/baogao/153200.html)
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
