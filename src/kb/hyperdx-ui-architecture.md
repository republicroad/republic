# HyperDX UI 重建架构方案

## 1. 项目概述

基于开源可观测性平台 [HyperDX](https://github.com/hyperdxio/hyperdx) 的前端功能，使用现代技术栈（TanStack Start + shadcn/ui + ReUI + ECharts）进行前端重建。暂不实现后端，通过 MSW 提供模拟数据，预留 API 接口层以便未来对接真实后端。

### 技术选型对照

| 层级   | HyperDX (原)        | 重建方案                                         |
| ---- | ------------------ | -------------------------------------------- |
| 框架   | Next.js 16         | **TanStack Start** (Vite + Nitro)            |
| 路由   | App Router (文件路由)  | **TanStack Router** (类型安全文件路由)               |
| UI 库 | Mantine            | **shadcn/ui + ReUI**                         |
| 图表   | Recharts + uPlot   | **ECharts 5.x**（主力）+ **uPlot**（高性能时序）+ **Recharts**（KPI 卡片） |
| 地图   | 无                  | **Leaflet/MapLibre**（底图）+ **deck.gl**（大数据聚合） |
| 状态   | Jotai              | **TanStack Query** (服务端) + **Zustand** (客户端) |
| 表单   | React Hook Form    | **TanStack Form + Zod**                      |
| 表格   | TanStack Table     | **TanStack Table + ReUI data-grid**          |
| 样式   | CSS Modules + Sass | **Tailwind CSS v4**                          |
| DnD  | @dnd-kit           | **@dnd-kit**                                 |
| 流程图  | @xyflow/react      | **@xyflow/react**                            |
| Mock | 无                  | **MSW v2**                                   |
| 测试   | Jest + Playwright  | **Vitest + Testing Library + Playwright**    |

---

## 2. RSC vs Client Components 分析

### 核心判断原则

> **交互概率 x 用户量 > 60% -> Client Component，否则 -> RSC 或静态**

### 可观测性仪表盘中的组件分类

| 组件 | 特征 | RSC 适合？ | 理由 |
|------|------|-----------|------|
| 侧边栏导航 | 静态结构，无状态 | 适合 | 零交互，RSC 零 JS 开销 |
| 页面布局骨架 | Shell 布局 | 适合 | 即时渲染视觉壳 |
| KPI 摘要卡片 | 只读，加载一次 | 适合 | 低交互，可服务端预取 |
| 历史数据图表 | 加载时查询，只读展示 | 可选 | 初始数据可预取，但 ECharts 本身是客户端 |
| 实时时间序列图 | WebSocket/SSE 持续更新 | 必须 Client | 需要 hooks + 持续状态更新 |
| 过滤器/搜索框 | onChange 事件处理 | 必须 Client | 纯交互组件 |
| 数据表格 | 排序/筛选/虚拟滚动 | 必须 Client | TanStack Table 纯客户端 |
| 仪表盘拖拽网格 | resize/drag/reorder | 必须 Client | 浏览器 API 依赖 |
| 告警配置表单 | 复杂状态 + 验证 | 必须 Client | 表单状态管理 |
| Session Replay 播放器 | rrweb Canvas 渲染 | 必须 Client | DOM 回放引擎 |
| SQL 编辑器 | CodeMirror 交互 | 必须 Client | 编辑器实例 |
| 设置页面（表单类） | 表单状态 | 必须 Client | 交互式表单 |

### 诚实的结论

对于可观测性 SaaS 仪表盘：

- **约 15-20% 组件适合 RSC**：导航、布局、KPI 摘要、只读展示
- **约 80-85% 必须是 Client Component**：图表、表格、搜索、表单、播放器、拖拽

**RSC 对本项目的真实价值**：
1. 首屏即时渲染布局壳（用户看到东西更快）
2. 初始数据快照预取（KPI 卡片服务端获取）
3. 服务端密钥安全（DB 凭据不出 bundle）
4. 不会改善图表渲染性能（ECharts 是 Canvas）
5. 不会改善实时数据流（WebSocket 纯客户端）
6. 不会显著减少 bundle（85% 组件需要客户端 JS）

### TanStack Start 的选择理由

TanStack Start 虽然默认是 Client-first，但通过 Route Loaders 可以在 SSR 阶段预取数据，实现类似 RSC 的首屏数据加载效果，同时避免 RSC 的心智负担：

| 需求 | Next.js RSC 方式 | TanStack Start 方式 |
|------|-----------------|-------------------|
| 首屏数据预取 | Server Component async/await | Route Loader + useSuspenseQuery |
| 安全服务端逻辑 | Server Component 内直接写 | createServerFn |
| 类型安全 | 有限（RSC 序列化限制） | 全栈端到端推断 |
| 心智模型 | Server/Client 边界划分 | 传统 SPA + 可选服务端 |

---

## 3. TanStack Start vs Next.js 对比

### 核心哲学差异

| | Next.js | TanStack Start |
|---|---------|---------------|
| 心智模型 | Server-first - 默认服务端渲染，用 "use client" 开启客户端 | Client-first - 默认 SPA，用 createServerFn 开启服务端 |
| 构建工具 | Turbopack（自研） | Vite（生态通用） |
| 路由 | 文件系统路由（App Router） | TanStack Router（类型安全的文件路由） |
| 数据获取 | RSC + Server Actions + 多层缓存 | Route Loaders + Server Functions（RPC 风格） |
| 部署 | 深度绑定 Vercel（其他平台体验较差） | 完全平台无关（Node/Bun/Deno/Cloudflare/Netlify） |

### 逐项对比

| 维度 | Next.js | TanStack Start | 胜出 |
|------|---------|---------------|------|
| 类型安全 | 路由类型靠 IDE 插件，非编译时 | 路由参数、Loader 数据、Server Function 输入输出全部编译时推断 | Start |
| RSC 支持 | 原生，核心卖点 | 实验性支持 | Next |
| SEO / SSG | ISR / SSG / Draft Mode 成熟 | 全文档 SSR + Streaming，无 ISR | Next |
| 缓存模型 | 多层缓存（多次重写，复杂） | 显式缓存，客户端 TanStack Query 管理 | Start（更简单） |
| 生态规模 | 8+ 年，海量第三方库 | 2 年，生态较小但增长快 | Next |
| 学习曲线 | App Router + RSC 概念多 | 接近传统 SPA 开发体验 | Start |
| Bundle 大小 | 较大 | 比 Next 小 30-35% | Start |
| Server Functions | Server Actions ("use server") | createServerFn（类型安全 RPC） | Start（类型更好） |
| Dev 体验 | Turbopack 快，但复杂度高 | Vite HMR 极快，心智负担低 | Start |
| 部署灵活性 | Vercel 最优，其他平台需额外配置 | 任意平台，无需改代码 | Start |
| 企业采用 | OpenAI, TikTok, Nike 等 | Lovable, 部分 SaaS | Next（目前） |

---

## 4. ECharts 在 OpenObserve 中的功能参考

OpenObserve 使用 **ECharts 6.x**（树摇导入），提供了 19 种面板类型 + 25 种自定义图表模板，是可观测性场景下 ECharts 使用的最佳实践参考。

### 4.1 内置面板类型

| 面板类型 | ECharts 系列 | 用途 |
|---------|-------------|------|
| `line` | LineChart | 时间序列折线图 |
| `area` | LineChart + areaStyle | 面积图 |
| `area-stacked` | LineChart + stack | 堆叠面积图 |
| `bar` | BarChart | 柱状图 |
| `h-bar` | BarChart (x/y 交换) | 水平柱状图 |
| `stacked` | BarChart + stack | 堆叠柱状图 |
| `h-stacked` | BarChart + 水平 + stack | 水平堆叠柱状图 |
| `scatter` | ScatterChart | 散点图/气泡图 |
| `pie` | PieChart | 饼图 |
| `donut` | PieChart + radius | 环形图 |
| `heatmap` | HeatmapChart + VisualMap | 热力图（延迟分布等） |
| `gauge` | GaugeChart | 仪表盘/量表 |
| `metric` | GraphicComponent | KPI 单值卡片（可带 sparkline） |
| `sankey` | SankeyChart | 桑基图（流量分析） |
| `geomap` | ECharts + Leaflet | 地理散点/柱状图（OpenStreetMap 底图） |
| `maps` | ECharts registerMap | 世界地图/区域热力图 |
| `table` | TanStack Table | 数据表格（非 ECharts） |
| `html` / `markdown` | 非 ECharts | 自定义 HTML/Markdown |
| `custom_chart` | 完整 ECharts option | 用户手写 ECharts 配置（支持任意图表） |

### 4.2 ECharts 组件注册（OpenObserve 实践）

```typescript
// OpenObserve ChartRenderer.vue 中注册的组件
import {
  TitleComponent, TooltipComponent, GridComponent, ToolboxComponent,
  DatasetComponent, LegendComponent, PolarComponent, VisualMapComponent,
  DataZoomComponent, MarkLineComponent, MarkAreaComponent, GraphicComponent
} from "echarts/components"

import { CanvasRenderer, SVGRenderer } from "echarts/renderers"
```

### 4.3 核心交互特性

- **跨图表联动**：hover 一个图表时，其他图表同步高亮相同时间点
- **DataZoom 框选缩放**：支持 brush 选择区域放大
- **右键菜单**：从图表数据点直接创建告警
- **MarkLine 告警线**：阈值线、参考线
- **主题系统**：CSS 变量 -> ECharts 颜色的运行时桥接（5 套预设主题）
- **自定义图表模板**：13 类 25 个模板（折线/柱状/饼图/散点/雷达/箱线/力导向/矩形树图/漏斗/3D 等）

### 4.4 主题桥接模式（OpenObserve 实践）

OpenObserve 不使用 ECharts 内置主题，而是运行时解析 CSS 设计令牌：

```typescript
// chartColor() 读取 CSS 变量并缓存
// 支持 12 色系列调色板 + 24 色表格调色板 + 35 色 Trace Span 调色板
// 主题切换时 invalidateChartTheme() 清除缓存触发重绘
```

---

## 5. React 图表库替代方案对比

### 5.1 总览对比

| 库 | Bundle (gzip) | 图表类型 | 大数据性能 | shadcn 兼容 | SSR | 维护状态 |
|---|---|---|---|---|---|---|
| **ECharts** | 100-186 KB | **30+** | **100K+ 点** | @devstool/shadcn-echarts | Canvas | 活跃 (Apache) |
| **Recharts** | 45-150 KB | ~15 | ~5K 点 | shadcn 官方图表 | SVG | 活跃 (48.9M/周) |
| **Nivo** | 70-285 KB | 25-30 | ~5-10K 点 | 不兼容 | RSC 问题 | 活跃 |
| **Visx** | 25-60 KB | 无限（原语） | 取决于实现 | 手动集成 | SVG | 活跃 (Airbnb) |
| **Tremor** | ~60 KB | 6-8 | ~5K 点 | Tailwind 原生 | SVG | 活跃 (Vercel) |
| **Chart.js** | 25-92 KB | 10-15 | 10-100K 点 | 不兼容 | Canvas | 活跃 |
| **Victory** | 68-100 KB | ~12 | ~1K 点 | 不兼容 | SVG | 稳定但慢 |
| **uPlot** | **10 KB** | 3-4 | **166K 点 25ms** | 不兼容 | Canvas | 稳定（单维护者） |
| **D3.js** | 10-90 KB | 无限 | 取决于实现 | 手动 | 部分 | 活跃 (113K 星) |
| **Plotly.js** | 430-870 KB | 40+ | WebGL 100K+ | 不兼容 | 不兼容 | React 包装器过时 |

### 5.2 各库详细评价

#### ECharts（推荐主力）
- **优势**：图表类型最全（heatmap/gauge/sankey/treemap/geo），Canvas 性能强，内置 dataZoom 缩放，shadcn 有 @devstool/shadcn-echarts 适配
- **劣势**：配置对象深层嵌套，TypeScript 类型自动生成，部分文档中文

#### Recharts（shadcn 官方）
- **优势**：JSX 声明式组件最易上手，shadcn/ui 官方图表基于此构建，SVG 可 SSR
- **劣势**：SVG 5K 点瓶颈，无 heatmap/gauge/sankey/geo，无内置缩放

#### uPlot（性能之王）
- **优势**：10KB gzip，166K 点 25ms 渲染，60fps 实时流，CPU 占用最低
- **劣势**：仅支持时序/线/面积/柱/OHLC，无 pie/heatmap/sankey，单维护者

#### Visx（自定义之王）
- **优势**：最底层原语，bundle 最小（5KB 起），完全可控，含 a11y 包
- **劣势**：需要自己构建所有图表，学习曲线最陡

#### Nivo
- **优势**：30+ 图表类型，动画精美，含 sankey/chord/calendar/treemap
- **劣势**：RSC 不兼容（issue #2626 未解决），累计 bundle 大，无原生缩放

#### 不推荐用于本项目的库

| 库 | 原因 |
|---|---|
| **Victory** | SVG 性能最差（~1K 点即卡），仅 web 项目无需 React Native 兼容 |
| **Plotly.js** | React 包装器过时（2025.1 后未更新），bundle 巨大（870KB），多图表页 WebGL 上下文限制 |
| **Nivo** | RSC 不兼容，累计 bundle 大，无原生缩放 |

### 5.3 高性能时序图表库（补充）

| 库 | Bundle | 性能 | 许可证 | 特点 |
|---|---|---|---|---|
| **LightningChart JS** | ~1 MB | **10M 点 0.29s，24M 点 60fps** | 商业许可 | GPU 加速，性能最强 |
| **SciChart.js** | ~500 KB | **百万点级实时流** | 商业（个人免费） | WebGL + WASM，ECG/金融专用 |
| **ZoomCharts TimeChart** | 未知 | 1M+ 点 60fps | 商业 | 智能数据采样，自动降级细节 |
| **ApexCharts** | ~125 KB | 5K-10K 点 | MIT | 默认样式最美，实时更新简单 |
| **Highcharts** | ~100 KB | 10K-50K 点 | 商业 | 金融图表专业（K线/OHLC） |
| **amCharts 5** | ~200 KB | 中等 | 商业 | 动画精美，主题丰富 |
| **AnyStock** | 轻量 | 中等 | 免费开源 | 专注金融时序 |

### 5.4 地图框架（补充）

#### 底图引擎

| 库 | 渲染 | 许可证 | React 封装 | 特点 |
|---|---|---|---|---|
| **Leaflet** | Canvas | BSD-2 | react-leaflet | 最简单，插件生态最大 |
| **Mapbox GL JS** | WebGL | 商业（免费额度） | react-map-gl | 矢量瓦片，样式最灵活 |
| **MapLibre GL JS** | WebGL | BSD-3 | react-maplibre | Mapbox 开源 fork，零瓦片费 |
| **OpenLayers** | Canvas | BSD-2 | 社区封装 | GIS 专业（WMS/WFS/KML） |
| **Google Maps JS API** | WebGL | 按量付费 | @vis.gl/react-google-maps | Google 生态集成 |
| **CesiumJS** | WebGL | Apache 2 | 有限 | **3D 地球/地形**唯一选择 |

#### 数据可视化层

| 库 | 用途 | 特点 |
|---|---|---|
| **deck.gl** (Uber) | 大规模地理数据 | 100K+ 点渲染，六边形/弧线/3D 图层 |
| **ECharts + Leaflet** | echarts-leaflet 扩展 | OpenObserve 采用的方案 |
| **react-visgl-maplibre** | Three.js + Deck.gl + MapLibre | 3D 地图可视化模板 |

#### 地图推荐组合

```
┌─────────────────────────────────────────────────────┐
│              地图架构推荐                              │
├──────────────────┬──────────────────────────────────┤
│  底图             │  Leaflet（简单）或 MapLibre（矢量） │
│                  │  零成本，OpenStreetMap 瓦片          │
├──────────────────┼──────────────────────────────────┤
│  数据叠加         │  ECharts geo series（少量点）       │
│                  │  deck.gl HexagonLayer（大量点聚合）  │
├──────────────────┼──────────────────────────────────┤
│  3D 地球          │  CesiumJS（仅在需要 3D 时）         │
└──────────────────┴──────────────────────────────────┘
```

### 5.5 可观测性仪表盘推荐图表架构

```
┌─────────────────────────────────────────────────────────┐
│              可观测性仪表盘图表架构                         │
├──────────────────┬──────────────────────────────────────┤
│  ECharts (主力)   │ Heatmap / Gauge / Sankey / Geo       │
│  100-186 KB      │ 多系列时间序列 + dataZoom              │
│                  │ 自定义图表模板                         │
├──────────────────┼──────────────────────────────────────┤
│  uPlot (高性能)   │ 实时流式时间序列面板                   │
│  10 KB           │ 100K+ 点，60fps，最低 CPU              │
├──────────────────┼──────────────────────────────────────┤
│  Recharts (快速)  │ KPI 摘要卡片 / 简单指标面板            │
│  45 KB           │ shadcn/ui 官方图表组件                  │
├──────────────────┼──────────────────────────────────────┤
│  Visx (自定义)    │ Trace 瀑布图 / 火焰图 / 拓扑图        │
│  25 KB           │ 任何 ECharts 不覆盖的自定义可视化       │
├──────────────────┼──────────────────────────────────────┤
│  Leaflet/MapLibre │ GeoMap 面板底图                       │
│  ~40 KB          │ OpenStreetMap 矢量瓦片                 │
├──────────────────┼──────────────────────────────────────┤
│  deck.gl         │ 大规模地理数据聚合                     │
│  ~100 KB         │ 100K+ 点六边形/弧线/3D 图层            │
└──────────────────┴──────────────────────────────────────┘
```

### 5.6 图表类型与推荐库映射

| 图表类型 | 推荐库 | 理由 |
|---------|--------|------|
| 时间序列（历史） | ECharts | dataZoom 缩放，多系列叠加 |
| 时间序列（实时流） | uPlot 或 SciChart.js | uPlot 100K+ 点 60fps，SciChart 百万点级 |
| 热力图 (Heatmap) | ECharts | 原生支持 + VisualMap 颜色映射 |
| 仪表盘 (Gauge) | ECharts | 原生支持 |
| 桑基图 (Sankey) | ECharts | 原生支持 |
| 地理地图 (GeoMap) | ECharts + Leaflet 或 MapLibre + deck.gl | 少量点用 ECharts，大量点聚合用 deck.gl |
| 饼图/环形图 | ECharts 或 Recharts | ECharts 功能更强，Recharts 更简单 |
| 柱状图/条形图 | ECharts 或 Recharts | 同上 |
| KPI 单值卡片 | Recharts (sparkline) 或 ApexCharts | shadcn 官方支持，ApexCharts 默认样式更美 |
| 散点图/气泡图 | ECharts | Canvas 性能好 |
| 矩形树图 (Treemap) | ECharts | 原生支持 |
| 漏斗图 (Funnel) | ECharts | 原生支持 |
| 雷达图 (Radar) | ECharts 或 Nivo | ECharts 更可控 |
| Trace 瀑布图 | Visx 或 自定义 | 需要定制化 Gantt 渲染 |
| 火焰图 (Flamegraph) | Visx 或 D3 | 需要定制化渲染 |
| 服务拓扑图 | @xyflow/react | 非图表库，专用流程图 |

### 5.7 图表实现模式

#### ECharts 树摇注册（完整版）

```typescript
// src/lib/echarts-setup.ts
import * as echarts from 'echarts/core'
import {
  BarChart, LineChart, PieChart,
  HeatmapChart, GaugeChart, ScatterChart,
  SankeyChart, TreemapChart, FunnelChart,
  RadarChart, CandlestickChart
} from 'echarts/charts'
import {
  TitleComponent, TooltipComponent, GridComponent,
  LegendComponent, DatasetComponent, TransformComponent,
  VisualMapComponent, DataZoomComponent, ToolboxComponent,
  PolarComponent, MarkLineComponent, MarkAreaComponent,
  GraphicComponent
} from 'echarts/components'
import { CanvasRenderer, SVGRenderer } from 'echarts/renderers'

echarts.use([
  BarChart, LineChart, PieChart, HeatmapChart, GaugeChart, ScatterChart,
  SankeyChart, TreemapChart, FunnelChart, RadarChart, CandlestickChart,
  TitleComponent, TooltipComponent, GridComponent, LegendComponent,
  DatasetComponent, TransformComponent, VisualMapComponent,
  DataZoomComponent, ToolboxComponent, PolarComponent,
  MarkLineComponent, MarkAreaComponent, GraphicComponent,
  CanvasRenderer, SVGRenderer,
])

export default echarts
```

#### ECharts 主题桥接（shadcn CSS 变量 -> ECharts）

```typescript
// src/lib/echarts-theme.ts
function tok(name: string): string {
  if (typeof window === 'undefined') return '#888'
  return getComputedStyle(document.documentElement)
    .getPropertyValue(name).trim()
}

export function registerTheme(name: string) {
  echarts.registerTheme(name, {
    color: [
      tok('--chart-1'), tok('--chart-2'),
      tok('--chart-3'), tok('--chart-4'), tok('--chart-5')
    ],
    backgroundColor: 'transparent',
    textStyle: { color: tok('--foreground') },
    tooltip: {
      backgroundColor: tok('--card'),
      borderColor: tok('--border'),
      textStyle: { color: tok('--card-foreground') }
    },
    xAxis: {
      axisLine: { lineStyle: { color: tok('--border') } },
      axisLabel: { color: tok('--muted-foreground') }
    },
    yAxis: {
      splitLine: { lineStyle: { color: tok('--border') } },
      axisLabel: { color: tok('--muted-foreground') }
    },
  })
}
```

#### uPlot 集成模式

```typescript
// src/components/charts/RealtimeChart.tsx
'use client'
import { useEffect, useRef } from 'react'
import uPlot from 'uplot'
import 'uplot/dist/uPlot.min.css'

interface RealtimeChartProps {
  data: [number[], ...number[][]]  // [x, ...y系列]
  options?: Partial<uPlot.Options>
}

export function RealtimeChart({ data, options }: RealtimeChartProps) {
  const containerRef = useRef<HTMLDivElement>(null)
  const chartRef = useRef<uPlot | null>(null)

  useEffect(() => {
    if (!containerRef.current) return

    const defaultOptions: uPlot.Options = {
      width: containerRef.current.clientWidth,
      height: 300,
      series: [
        {},  // x轴
        { stroke: 'color-mix(in srgb, var(--chart-1) 100%, transparent)', fill: 'color-mix(in srgb, var(--chart-1) 20%, transparent)' },
        { stroke: 'color-mix(in srgb, var(--chart-2) 100%, transparent)' },
      ],
      axes: [
        { stroke: 'var(--border)', grid: { stroke: 'var(--border)' } },
        { stroke: 'var(--border)', grid: { stroke: 'var(--border)' } },
      ],
      ...options,
    }

    chartRef.current = new uPlot(defaultOptions, data, containerRef.current)

    const resizeObserver = new ResizeObserver(() => {
      chartRef.current?.setSize({
        width: containerRef.current!.clientWidth,
        height: 300,
      })
    })
    resizeObserver.observe(containerRef.current)

    return () => {
      resizeObserver.disconnect()
      chartRef.current?.destroy()
    }
  }, [data, options])

  return <div ref={containerRef} className="w-full" />
}
```

#### Recharts KPI 卡片

```typescript
// src/components/charts/KpiCard.tsx
'use client'
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card'
import { Area, AreaChart, ResponsiveContainer } from 'recharts'

interface KpiCardProps {
  title: string
  value: string | number
  trend?: { value: number; isPositive: boolean }
  sparklineData?: { value: number }[]
}

export function KpiCard({ title, value, trend, sparklineData }: KpiCardProps) {
  return (
    <Card>
      <CardHeader className="flex flex-row items-center justify-between space-y-0 pb-2">
        <CardTitle className="text-sm font-medium">{title}</CardTitle>
      </CardHeader>
      <CardContent>
        <div className="text-2xl font-bold">{value}</div>
        {trend && (
          <p className={`text-xs ${trend.isPositive ? 'text-emerald-500' : 'text-red-500'}`}>
            {trend.isPositive ? '+' : ''}{trend.value}% from last period
          </p>
        )}
        {sparklineData && (
          <div className="h-[40px] mt-2">
            <ResponsiveContainer width="100%" height="100%">
              <AreaChart data={sparklineData}>
                <Area
                  type="monotone"
                  dataKey="value"
                  stroke="var(--chart-1)"
                  fill="var(--chart-1)"
                  fillOpacity={0.2}
                />
              </AreaChart>
            </ResponsiveContainer>
          </div>
        )}
      </CardContent>
    </Card>
  )
}
```

#### Leaflet 地图集成

```typescript
// src/components/charts/GeoMap.tsx
'use client'
import { useEffect, useRef } from 'react'
import L from 'leaflet'
import 'leaflet/dist/leaflet.css'

interface GeoMapProps {
  center: [number, number]
  zoom: number
  markers?: { lat: number; lng: number; popup?: string }[]
}

export function GeoMap({ center, zoom, markers = [] }: GeoMapProps) {
  const mapRef = useRef<HTMLDivElement>(null)
  const mapInstanceRef = useRef<L.Map | null>(null)

  useEffect(() => {
    if (!mapRef.current || mapInstanceRef.current) return

    const map = L.map(mapRef.current).setView(center, zoom)
    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
      attribution: '© OpenStreetMap contributors',
    }).addTo(map)

    markers.forEach(({ lat, lng, popup }) => {
      const marker = L.marker([lat, lng]).addTo(map)
      if (popup) marker.bindPopup(popup)
    })

    mapInstanceRef.current = map

    return () => {
      map.remove()
      mapInstanceRef.current = null
    }
  }, [center, zoom, markers])

  return <div ref={mapRef} className="h-[400px] w-full rounded-lg" />
}
```

---

## 6. 项目目录结构

```
hyperdx-ui/
├── app.config.ts                     # TanStack Start 配置
├── vite.config.ts                    # Vite + TanStack Router 插件
├── package.json
├── tsconfig.json
├── vitest.config.ts
├── playwright.config.ts
├── public/
│   └── mockServiceWorker.js          # MSW Service Worker
│
├── src/
│   ├── routes/                       # 文件路由（自动生成 routeTree.gen.ts）
│   │   ├── __root.tsx                # 根布局（ThemeProvider, QueryProvider）
│   │   ├── index.tsx                 # / -> 重定向到 /search
│   │   │
│   │   ├── \_auth/                    # 认证布局（pathless）
|   │   │   ├── route.tsx             # beforeLoad 认证守卫
│   │   │   │
│   │   │   ├── search.tsx            # /search - 日志搜索
│   │   │   │
│   │   │   ├── dashboards/
│   │   │   │   ├── route.tsx         # 仪表盘布局
│   │   │   │   ├── index.tsx         # /dashboards - 列表
│   │   │   │   └── .tsx  # /dashboards/:id - 详情
│   │   │   │
│   │   │   ├── charts/
│   │   │   │   └── .tsx      # /charts/:id - 单图表详情
│   │   │   │
│   │   │   ├── traces.tsx            # /traces - 链路追踪
│   │   │   ├── traces..tsx   # /traces/:traceId - Trace 详情
│   │   │   │
│   │   │   ├── services.tsx          # /services - 服务目录
│   │   │   ├── services..tsx
│   │   │   ├── service-map.tsx       # /service-map - 拓扑图
│   │   │   │
│   │   │   ├── metrics.tsx           # /metrics - 指标浏览
│   │   │   ├── metrics.editor.tsx    # /metrics/editor
│   │   │   │
│   │   │   ├── alerts/
│   │   │   │   ├── route.tsx
│   │   │   │   ├── index.tsx         # /alerts - 列表
│   │   │   │   ├── .tsx      # /alerts/:id - 详情
│   │   │   │   └── destinations.tsx  # /alerts/destinations
│   │   │   │
│   │   │   ├── sessions.tsx          # /sessions - Session Replay
│   │   │   ├── sessions..tsx
│   │   │   │
│   │   │   ├── kubernetes.tsx        # /kubernetes - K8s 概览
│   │   │   ├── settings.tsx          # /settings - 设置
│   │   │   │
│   │   │   └── clickhouse.tsx        # /clickhouse - SQL 编辑器
│   │   │
│   │   ├── \_public/                  # 公开页面布局（pathless）
│   │   │   ├── route.tsx
│   │   │   ├── login.tsx             # /login
│   │   │   └── join.\.tsx     # /join/:teamId
│   │   │
│   │   └── $.tsx                     # 404 Catch-all
│   │
│   ├── components/
│   │   ├── ui/                       # shadcn/ui 组件（自动生成）
│   │   ├── reui/                     # ReUI 组件（按需安装）
│   │   │
│   │   ├── charts/                   # ECharts 图表封装
│   │   │   ├── echarts-setup.ts      # 树摇注册
│   │   │   ├── echarts-theme.ts      # shadcn -> ECharts 主题桥接
│   │   │   ├── EChart.tsx            # 基础组件（"use client"）
│   │   │   ├── TimeSeriesChart.tsx
│   │   │   ├── BarChart.tsx
│   │   │   ├── PieChart.tsx
│   │   │   ├── HeatmapChart.tsx
│   │   │   ├── GaugeChart.tsx
│   │   │   ├── NumberCard.tsx
│   │   │   ├── HistogramChart.tsx
│   │   │   └── TableChart.tsx
│   │   │
│   │   ├── layout/                   # App Shell
│   │   │   ├── AppNav.tsx            # 侧边栏导航
│   │   │   ├── TopBar.tsx            # 顶栏
│   │   │   ├── Breadcrumb.tsx
│   │   │   └── TabBar.tsx
│   │   │
│   │   ├── search/                   # 日志搜索模块
│   │   │   ├── SearchInput.tsx       # 查询输入 + 自动补全
│   │   │   ├── SearchFilters.tsx     # 过滤器系统
│   │   │   ├── LogResultsTable.tsx   # 日志结果表格
│   │   │   ├── LogHistogram.tsx      # 时间直方图
│   │   │   └── LogDetail.tsx         # 日志详情展开
│   │   │
│   │   ├── dashboard/                # 仪表盘模块
│   │   │   ├── DashboardGrid.tsx     # 拖拽网格布局
│   │   │   ├── DashboardCard.tsx     # 图表卡片
│   │   │   ├── DashboardFilters.tsx  # 仪表盘级过滤
│   │   │   ├── ChartEditor.tsx       # 图表配置 Drawer
│   │   │   └── DashboardToolbar.tsx  # 工具栏
│   │   │
│   │   ├── traces/                   # 链路追踪
│   │   │   ├── TraceWaterfall.tsx
│   │   │   ├── SpanBlock.tsx
│   │   │   ├── SpanDetail.tsx
│   │   │   └── ServiceMap.tsx        # @xyflow/react
│   │   │
│   │   ├── alerts/                   # 告警
│   │   ├── sessions/                 # Session Replay
│   │   ├── settings/                 # 设置
│   │   └── shared/                   # 共享组件
│   │       ├── TimePicker.tsx
│   │       ├── JsonViewer.tsx
│   │       ├── VirtualTable.tsx
│   │       ├── EmptyState.tsx
│   │       └── ConfirmDialog.tsx
│   │
│   ├── hooks/                        # 自定义 hooks（纯 UI 逻辑）
│   │   ├── useTimeRange.ts
│   │   ├── useSearchQuery.ts
│   │   └── useChartConfig.ts
│   │
│   ├── lib/
│   │   ├── echarts-setup.ts          # ECharts 树摇注册
│   │   ├── echarts-theme.ts          # 主题桥接
│   │   ├── utils.ts                  # cn(), 格式化等
│   │   └── constants.ts
│   │
│   ├── types/                        # 共享 TypeScript 类型
│   │   ├── api.ts
│   │   ├── dashboard.ts
│   │   ├── trace.ts
│   │   ├── alert.ts
│   │   └── chart.ts
│   │
│   ├── server/                       # Server Functions
│   │   ├── search.functions.ts       # createServerFn 包装
│   │   ├── search.server.ts          # 服务端查询逻辑
│   │   ├── dashboards.functions.ts
│   │   ├── dashboards.server.ts
│   │   ├── traces.functions.ts
│   │   ├── metrics.functions.ts
│   │   ├── alerts.functions.ts
│   │   ├── sessions.functions.ts
│   │   ├── settings.functions.ts
│   │   └── schemas.ts               # 共享 Zod schemas
│   │
│   ├── services/                     # 客户端 API 调用层
│   │   ├── search.ts
│   │   ├── dashboards.ts
│   │   ├── traces.ts
│   │   ├── metrics.ts
│   │   ├── alerts.ts
│   │   ├── sessions.ts
│   │   └── settings.ts
│   │
│   ├── stores/                       # Zustand stores（客户端全局状态）
│   │   ├── auth-store.ts
│   │   ├── dashboard-store.ts
│   │   └── ui-store.ts
│   │
│   └── mocks/                        # MSW Mock
│       ├── handlers/
│       │   ├── search.ts
│       │   ├── dashboards.ts
│       │   ├── traces.ts
│       │   ├── metrics.ts
│       │   ├── alerts.ts
│       │   ├── sessions.ts
│       │   ├── settings.ts
│       │   └── index.ts
│       ├── data/                     # 静态 mock 数据
│       │   ├── logs.json
│       │   ├── traces.json
│       │   ├── dashboards.json
│       │   └── metrics.json
│       ├── browser.ts                # 浏览器端 worker
│       └── server.ts                 # Node 端 server
```

---

## 7. 关键集成模式

### 7.1 Route Loader 数据预取（替代 RSC 的首屏数据加载）

```tsx
// src/routes/search.tsx
import { createFileRoute, useSuspenseQuery } from '@tanstack/react-router'
import { searchQueryOptions } from '../services/search'

export const Route = createFileRoute('/search')({
  // SSR 阶段预取数据，流式发送到客户端
  loader: ({ context }) =>
    context.queryClient.ensureQueryData(
      searchQueryOptions({ query: '', timeRange: '1h' })
    ),
  component: SearchPage,
})

function SearchPage() {
  const { data } = useSuspenseQuery(
    searchQueryOptions({ query: '', timeRange: '1h' })
  )
  return <LogResultsTable data={data.results} />
}
```

### 7.2 Server Functions（类型安全 RPC）

```ts
// src/server/search.functions.ts
import { createServerFn } from '@tanstack/react-start'
import { z } from 'zod'

export const searchLogs = createServerFn({ method: 'POST' })
  .validator(z.object({
    query: z.string(),
    timeRange: z.enum(['1h', '6h', '24h', '7d']),
    limit: z.number().default(100),
  }))
  .handler(async ({ data }) => {
    // 服务端逻辑：查 ClickHouse / 数据库
    return { results: [...], total: 42 }
  })
```

### 7.3 MSW + Vite 集成

```ts
// vite.config.ts
import { defineConfig } from 'vite'
import { TanStackRouterVite } from '@tanstack/router-plugin/vite'
import { tanstackStart } from '@tanstack/react-start/plugin/vite'
import react from '@vitejs/plugin-react'
import msw from '@iodigital/vite-plugin-msw'
import { handlers } from './src/mocks/handlers'

export default defineConfig({
  plugins: [
    tanstackStart(),
    TanStackRouterVite(),
    react(),
    ...(process.env.VITE_ENABLE_MSW === 'true'
      ? [msw({ handlers, mode: 'browser' })]
      : []),
  ],
})
```

### 7.4 ECharts 树摇注册

```ts
// src/lib/echarts-setup.ts
import * as echarts from 'echarts/core'
import {
  BarChart, LineChart, PieChart,
  HeatmapChart, GaugeChart, ScatterChart
} from 'echarts/charts'
import {
  TitleComponent, TooltipComponent, GridComponent,
  LegendComponent, DatasetComponent, TransformComponent,
  VisualMapComponent
} from 'echarts/components'
import { CanvasRenderer } from 'echarts/renderers'

echarts.use([
  BarChart, LineChart, PieChart, HeatmapChart, GaugeChart, ScatterChart,
  TitleComponent, TooltipComponent, GridComponent, LegendComponent,
  DatasetComponent, TransformComponent, VisualMapComponent,
  CanvasRenderer,
])

export default echarts
```

### 7.5 ECharts 主题桥接（shadcn CSS 变量 -> ECharts）

```ts
// src/lib/echarts-theme.ts
function tok(name: string): string {
  if (typeof window === 'undefined') return '#888'
  return getComputedStyle(document.documentElement)
    .getPropertyValue(name).trim()
}

export function registerTheme(name: string) {
  echarts.registerTheme(name, {
    color: [
      tok('--chart-1'), tok('--chart-2'),
      tok('--chart-3'), tok('--chart-4'), tok('--chart-5')
    ],
    backgroundColor: 'transparent',
    textStyle: { color: tok('--foreground') },
    tooltip: {
      backgroundColor: tok('--card'),
      borderColor: tok('--border'),
      textStyle: { color: tok('--card-foreground') }
    },
    xAxis: {
      axisLine: { lineStyle: { color: tok('--border') } },
      axisLabel: { color: tok('--muted-foreground') }
    },
    yAxis: {
      splitLine: { lineStyle: { color: tok('--border') } },
      axisLabel: { color: tok('--muted-foreground') }
    },
  })
}
```

---

## 8. 模块化开发顺序

| 阶段 | 模块 | 预估 |
|------|------|------|
| P0-1 | 基础架构（TanStack Start + shadcn + ECharts + uPlot + Recharts + Leaflet + MSW + 路由 + App Shell） | 2 周 |
| P0-2 | 共享层（types + server functions + services + mocks + hooks + TimePicker） | 1.5 周 |
| P0-3 | 日志搜索（SearchInput + Filters + Results + Histogram） | 3 周 |
| P0-4 | 仪表盘（Grid + Cards + ChartEditor + 全部图表类型 + GeoMap） | 4 周 |
| P1-1 | 链路追踪（Waterfall + SpanDetail + ServiceMap） | 3 周 |
| P1-2 | 指标（MetricsExplorer + PromQL Editor） | 2 周 |
| P1-3 | 告警（List + Form + History + Destinations） | 2.5 周 |
| P1-4 | 设置（Team + API Keys + Connections） | 2 周 |
| P2-1 | Session Replay（List + Player + Timeline） | 2.5 周 |
| P2-2 | Kubernetes（Dashboard + Pod/Node Detail） | 1.5 周 |
| P2-3 | SQL 编辑器（CodeMirror 集成） | 1.5 周 |
| P3 | AI 摘要 + Onboarding + Storybook | 2 周 |
| | **总计** | **约 28 周** |

---

## 9. OpenObserve 功能移植清单

以下是 OpenObserve 有而 HyperDX 没有的功能，后续可按需移植：

| 功能 | 复杂度 | 优先级 |
|------|--------|--------|
| 管道编辑器（可视化 VRL/JS 流程） | 高 | P2 |
| 合成监控（浏览器测试 + 协议检查） | 高 | P3 |
| SLO 管理 | 中 | P2 |
| IAM（RBAC 权限矩阵 + 组 + 服务账户） | 高 | P2 |
| 报表（定时生成 PDF/邮件报表） | 中 | P3 |
| 多语言（i18n） | 中 | P3 |
| GeoMap 面板 | 低 | P2 |
| HTML/Markdown 面板 | 低 | P2 |

---

## 10. TanStack Start 关键包清单

| 包名                               | 用途                           |
| -------------------------------- | ---------------------------- |
| @tanstack/react-start            | 全栈框架（Vite + Nitro）           |
| @tanstack/react-router           | 类型安全路由核心                     |
| @tanstack/router-plugin/vite     | 自动生成路由树                      |
| @tanstack/react-query            | 服务端状态管理                      |
| @tanstack/react-router-ssr-query | SSR dehydration/hydration 集成 |
| @tanstack/form                   | 表单管理                         |
| @tanstack/react-table            | 数据表格                         |
| @tanstack/router-devtools        | 路由调试工具                       |
| shadcn/ui                        | UI 组件库                       |
| @reui/*                          | 扩展组件（data-grid, filters 等）   |
| echarts                          | 图表库（主力）                     |
| uplot                            | 高性能时序图表                     |
| recharts                         | KPI 卡片 / 简单图表                |
| leaflet / react-leaflet          | 地图底图（简单场景）                 |
| maplibre-gl / react-maplibre     | 地图底图（矢量瓦片）                 |
| deck.gl                          | 大规模地理数据可视化                 |
| @iodigital/vite-plugin-msw       | MSW Vite 集成                  |
| msw                              | API Mock                     |
| zod                              | Schema 验证                    |
| zustand                          | 客户端全局状态                      |
| @dnd-kit/*                       | 拖拽                           |
| @xyflow/react                    | 流程图/拓扑图                      |
| vitest                           | 单元测试                         |
| @playwright/test                 | E2E 测试                       |
