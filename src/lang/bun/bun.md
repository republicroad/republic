
# bun

Bun 的核心是一个 JavaScript 运行时，但它的实用性远不止于此。以下是它在开发工作流程中可以扮演的各种角色的简要概述：

### 1. 打包工具与构建工具

Bun 可以充当打包工具，类似于 Webpack、Rollup 或 Vite 等流行工具。它可以高效地打包 JavaScript 代码及其依赖项，从而优化 Web 应用程序的性能和加载速度。
同时, bun 也支持本地构建(后端程序)，支持交叉编译。最新版本支持在二进制构建中捆绑静态资源。对于未来的原生app可以有更好的支持。

### 2. 测试运行器

测试是软件开发中不可或缺的一部分，而 Bun 在这方面也毫不逊色。它可以无缝地充当测试运行器，支持 Jest、Vitest 和 Mocha 等流行的测试框架。这简化了编写和执行测试的过程，确保了代码库的可靠性和稳定性。

### 3. 包管理器集成

Bun 可以与各种包管理器（包括 npm、Yarn 和 pnpm）完美兼容。这种兼容性让您可以轻松管理项目的依赖项，并选择最适合您工作流程的包管理器。

### 4. 兼容 Node.js API 

Bun 虽然提供了一系列自身的功能，但它仍然与现有的 Node.js API 兼容。这意味着您可以利用 Bun 的强大功能，同时仍然依赖 Node.js 提供的熟悉功能。

### 5. CommonJS与ES模块支持

Bun 旨在满足多样化的 JavaScript 模块生态系统的需求。它能够轻松处理 CommonJS 和 ES 模块，确保您的代码能够与各种库和框架无缝交互。

### 6. 内置 TypeScript 支持

TypeScript 凭借其静态类型和增强的工具功能，在 JavaScript 社区中越来越受欢迎。Bun 通过在开发和生产环境中提供对 TypeScript 和 TSX 的开箱即用支持，简化了 TypeScript 集成。这消除了对 Nodemon 和 Concurrently 等额外工具的需求，从而简化了您的开发工作流程。

### 7. 配置环境变量文件

在某些开发设置中，处理环境变量可能是一项繁琐的任务。然而，Bun 通过 使环境变量可全局访问，从而简化了此过程`Bun.env`。这意味着您可以轻松访问环境变量，从而增强应用程序的可配置性。

### 8. 设置 HTTP 服务器

在 Web 开发中，搭建 HTTP 服务器是一项基本任务。Bun 简化了这一过程，并提供了一种为您的应用程序建立 HTTP 服务器的直接方法。此外，还有一个名为 Elysia 的后端框架可以与 Bun 无缝配对，为前端和后端开发提供全面的解决方案。



## bun debugger

https://bun.sh/docs/runtime/debugger  
https://debug.bun.sh/  
https://bun.com/docs/runtime/bunfig  
https://www.codingtag.com/debugging-in-bunjs  

Bun 使用[WebKit 检查器协议](https://github.com/oven-sh/bun/blob/main/packages/bun-inspector-protocol/src/protocol/jsc/index.d.ts)，因此您可以使用交互式调试器调试代码。

在 `bun --inspect-brk index.ts` 之后，会出现一个用于调试的url:

```bash
--------------------- Bun Inspector ---------------------
Inspect in browser:
  https://debug.bun.sh/#localhost:6499/cv5lqky952s
Listening:
  ws://localhost:6499/cv5lqky952s
```

或者 `chrome://inspect` 使用 localhost:6499 连接调试服务器进行调试.

### vscode

要安装扩展，请访问VS Code 市场网站上的[Bun for Visual Studio Code页面，然后单击“安装”。](https://marketplace.visualstudio.com/items?itemName=oven.bun-vscode)


## bun repl

Start a REPL session with Bun

例子: 查看 sqlite 版本
```bash
 > bun repl
Welcome to Bun v1.1.7
Type ".help" for more information.
[!] Please note that the REPL implementation is still experimental!
    Don't consider it to be representative of the stability or behavior of Bun overall.
> import { Database } from "bun:sqlite";
> const db = new Database(":memory:");
undefined
> const query = db.query("SELECT sqlite_version()")
undefined
> query.get()
{ 'sqlite_version()': '3.45.0' }
>
```


## tools

### [Benchmarking tools](https://bun.sh/docs/project/benchmarking#benchmarking-tools)

#### [mitata](https://github.com/evanwashere/mitata)  

适合单元测试，对单元测试进行压测，方便对比同一个实现不同库的性能对比(pytest-benchmark).
For microbenchmarks, a great general-purpose tool is [`mitata`](https://github.com/evanwashere/mitata).
try with  https://bolt.new/~/mitata  

```js
//test.js
import { run, bench } from 'mitata';
bench('noop', () => {});
await run();
```

```js
bun test.js
clk: ~0.21 GHz
cpu: null
runtime: webcontainer cb7c0bca (js + wasm)

benchmark                   avg (min … max) p75 / p99    (min … top 1%)
------------------------------------------- -------------------------------
noop                         748.06 ps/iter   1.22 ns █         ▅           !
                        (0.00 ps … 1.14 µs)   2.44 ns █         █          
                    (  0.00  b …   0.00  b)   0.00  b █▁▁▁▁▁▁▁▁▁█▁▁▁▁▁▁▁▁▁▂

               benchmark was likely optimized out (dead code elimination) = !
               https://github.com/evanwashere/mitata#writing-good-benchmarks
```


#### load testing

For load testing, you _must use_ an HTTP benchmarking tool that is at least as fast as `Bun.serve()`, or your results will be skewed. Some popular Node.js-based benchmarking tools like [`autocannon`](https://github.com/mcollina/autocannon) are not fast enough. We recommend one of the following:

- [`bombardier`](https://github.com/codesenberg/bombardier)
- [`oha`](https://github.com/hatoo/oha)
- [`http_load_test`](https://github.com/uNetworking/uSockets/blob/master/examples/http_load_test.c)

#### [`hyperfine`](https://github.com/sharkdp/hyperfine)

A command-line benchmarking tool.

- Statistical analysis across multiple runs.
- Support for arbitrary shell commands.
- Constant feedback about the benchmark progress and current estimates.
- Warmup runs can be executed before the actual benchmark.
- Cache-clearing commands can be set up before each timing run.
- Statistical outlier detection to detect interference from other programs and caching effects.
- Export results to various formats: CSV, JSON, Markdown, AsciiDoc.
- Parameterized benchmarks (e.g. vary the number of threads).
- Cross-platform