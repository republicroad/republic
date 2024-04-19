mitmproxy的使用
## 一.是啥？

官网：<https://www.mitmproxy.org/>

一句话：

  测试来看：基于python的更加细粒度、更加灵活、可编程、跨平台的代理工具。

  开发来看：基于python的可编程的调试工具（mock参数，检查服务端逻辑代码是否正常等等）。

  运维来看：是一个细粒度的负载均衡，可以指定特定的URL或者服务到指定的集群，方便收集信息。

## 二.有啥用？

排查线上问题：

线上出现问题需要排查是否和更新有关，那么我们可以用它来指向不同的集群（不用更改DNS解析），用它来劫持使用本地资源，快速调试。解决跨域和网络拓扑比较复杂的问题。

自定义脚本断言参数：

当然我们还可以编程，将其改造成符合自己需要的工具。例如：对答案进行解密，获取gee\_token。获取设备验的检查信息，反向对服务和前端规则的检查测试等等。

## 三.怎么用？

该工具分mitmproxy、mitmdump、mitmweb三种使用方式。

mitmproxy 为命令行启动,可进行命令编程

mitmdump 为输出式启动，命令行工具

mitmweb 浏览器界面的形式

我们平时使用，如果没有对自定义脚本的要求，一般使用mitmweb即可，那么接下来就以mitmweb为例来介绍如何使用

将我们的代理切换到mitmproxy的默认端口（127.0.0.1:8080）
*   下载证书

在代理情况下 输入mitm.it,下载安装证书
证书安装步骤如下（这里windows为例）：

1.挂上代理后输入mitm.it  
2.下载windows的证书  
3.点击下载的证书文件进行安装，选择“本地计算机”，下一步。  
4.默认下一步  
5.不用输入密码，下一步  
6.选择“将所有的证书都放入下列存储”，存储路径为“受信任的根证书颁发机构”，下一步  
7.点击完成即可  
如果没有挂上mitm的代理或者有错，这时候检查代理是否正确，mitm是否报错进行排查。
## 四.常见的使用场景

### 抓包（web、移动端）

在上一章已经介绍怎么抓web，那么这里就在介绍怎么抓移动端的包，以ios为例

1.移动端和PC端在同一个网络环境，将代理改为mitmproxy的服务地址（例如：192.168.0.73:8078）  
2.浏览器打开mitm.it，下载证书  
3.安装证书，将该证书信任  
4.此时就可以进行移动端抓包了，可以抓https和http的请求。  
### 替换资源

  在测试过程中需要进行线上的客户场景的覆盖测试，在排查客户问题时需要调试js,css等静态文件，故产生了替换资源的需求。在替换的过程中又分远程和本地，远程：用于线上资源替换成测试环境的资源（js经过打包编译）。本地：用于测试、调试、源码。

*   远程资源

  map\_remote

  替换远程资源的步骤如下：

  进入抓包界面（例如：localhost:8081）

  点击Options--Edit Options--选择map\_remote，填入替换资源。（这里的语法规则为：|http/https\://过滤内容|需要替换的URL|远程资源地址）当然我们在这里可以直接将过滤和需要替换的URL写成一个例如（|http/https\://过滤内容|远程资源地址）

  如：|<https://static.geetest.com/v4/static/v1.7.1/js/gcaptcha4.js>|<http://devstatic.geetest.com/v4/static/v1.7.2/js/gcaptcha4.js>

*   本地资源

  map\_local

  我们常用的应该是本地资源

  点击Options--Edit Options--选择map\_local,填入替换资源。语法与map\_remote类似，只是不用加协议。|/v4/static/v1.6.9/js/gcaptcha4.js|/v4/static/v1.6.9/js/gcaptcha4.js|/home/ryefccd/mitproxy/replace/gcaptcha4.js
### 指定服务集群

有两种方式：一种是在代码中进行修改。一种是在mitmweb中的option中修改，鉴于便利故这里介绍mitmweb方式。

Option--Edit Options --modify\_headers

例如配置：|static.geetest.com|Host|123.56.131.148，将static静态资源指向148服务器。同理我们可以将验证服务指向对应的服务，例如|api.geetest.com|Host|192.168.144.32
再例如在平时的测试过程中会遇到服务更新，特别是有新的规则时，由于我们服务不可能做到完全同步更新，故分布式集群上就会同时存在“新旧服务”的情况。也就是会出现：

  “新服务问---旧服务答”

  “旧服务问---新服务答”

  “旧服务问---旧服务答”

  “新服务问---新服务答”

### 自定义脚本
* 更改请求参数
```python
def request(flow: http.HTTPFlow) -> None:
    """
    将顺丰的ID替换成我们自己的ID 避免受规则影响
    """
    if "https://example.com/exc" in flow.request.url and re.search("7aff034c32dd2092ef63d12b86a95aeb",
                                                                              flow.request.url):
        new_id = "c3d2cbcabdf0e3b7eb6906e8cdbaace4"
        flow.request.query["ex_id"] = new_id
```

 * 自定义option
 由于我们在测试或者在日常工作中需要指向某个特殊的集群，实现细粒度的分布式。之前的“指定服务集群”一种是更改脚本，一种是劫持修改host   
 这里我们只讲使用，原理可参考官网options-configure.py 这个例子（<https://docs.mitmproxy.org/archive/v9/addons-examples/#options-configure>），然后更改flow的host即可。
```python 
"""React to configuration changes."""
from typing import Optional

from mitmproxy import ctx
from mitmproxy import exceptions


class AddHeader:
    def load(self, loader):
        loader.add_option(
            name="addheader",
            typespec=Optional[int],
            default=None,
            help="Add a header to responses",
        )

    def configure(self, updates):
        if "addheader" in updates:
            if ctx.options.addheader is not None and ctx.options.addheader > 100:
                raise exceptions.OptionsError("addheader must be <= 100")

    def response(self, flow):
        if ctx.options.addheader is not None:
            flow.response.headers["addheader"] = str(ctx.options.addheader)


addons = [AddHeader()]

```