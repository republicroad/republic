

# bun


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