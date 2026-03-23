

# topk

### Use Cases

- **Log Analysis**: Find the most frequent IP addresses, user agents, or error messages
- **Text Processing**: Identify the most common words in large documents
- **Network Monitoring**: Track heavy hitters in network traffic
- **Clickstream Analysis**: Find the most popular pages or user actions
- **Time Series Data**: Monitor frequently occurring events or anomalies
- 热点键检测: 在分布式缓存系统中用于检查访问频率高的热键, 构建本地缓存，避免热键对存储系统的高频访问，降低存储系统压力，提高系统稳定性。

## heavykeeper
[heavykeeper](https://pypi.org/project/heavykeeper/): a fast, memory-efficient sketch-based algorithm for finding the top-
K most frequent items in data streams.

## [Bubble Sketch](https://dl.acm.org/doi/10.1145/3627673.3679882)

A High-performance and Memory-efficient Sketch for Finding Top-𝑙 Items in Data Streams
