## 1 课程概述

- 数字芯片设计流程

![](./assets/202209012126539.png)

- FPGA设计流程

![](./assets/image-20220901213024798.png)

- Setup Time - The length of time that data must stabilize before the clock transition.

![](assets/image-20220901213412666.png)

- Hold Time - The length of time that data must remain stable at the input pin after the active clock transition.

![](assets/image-20220901213436880.png)

> 静态时序分析主要针对同步时序电路，异步时序电路需要单独分析

![](assets/image-20220901214021580.png)

![](assets/image-20220901214042183.png)

## 2 Part-1: TCL语言

### 概述

![](assets/image-20220901215232039.png)

- TCL语言的启动

  - Linux：`#!/user/bin/tclsh`
  - Win：安装active tcl，双击wish.exe

- 置换 ——> TCL解释器运用规则将命令分为一个个独立的单词，并进行必要的置换。

  - 变量置换`$`

    ```tcl
    set a "snow"
    puts $a
    p
    ```

  - 命令置换`[]`

  - 反斜杆置换`\`

### 变量、数组、列表

### 控制流

### 过程函数

### 正则匹配

### 文本处理

