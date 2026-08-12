# Nexus 仓库类型（hosted / proxy / group）

> 在 Browse assets and components 中，type 字段表示仓库类型，共三种。

## hosted（宿主仓库）

- 构件由 Nexus 自己存储托管，通常是私服。
- 存放自研构件或第三方构件，`mvn deploy` 推送至此。
- 例：`maven-releases`、`maven-snapshots`。

## proxy（代理仓库）

- 指向远程仓库（如 Maven Central），客户端请求时先远程拉取并缓存一份。
- 之后本地直接命中缓存，不直接访问远程。

## group（组仓库）

- 聚合多个 hosted + proxy 的虚拟仓库，对外暴露一个统一 URL。
- 请求按组内仓库顺序逐个查找，第一个命中的返回。
- Maven 的 settings.xml 通常只配置 group（如 `maven-public`），无需关心构件来自本地还是远程。

## 相关文档

- [VS Code / Zed 使用公司内网 Maven 仓库](../editor/vscode-java-maven.md)
