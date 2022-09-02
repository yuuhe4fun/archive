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

  - 变量置换`$`：TCL解释器会将认为$后面为变量名，将变量置换成它的值。

    ```tcl
    set a "snow"
    puts $a #snow
    puts a  #a
    ```

  - 命令置换`[]`：[]内是一个独立的TCL语句。

    ```tcl
    set a "[expr 3 + 4]"
    puts $a #7
    ```

  - 反斜杆置换`\`：换行符、空格、[、$等被TCL解释器当作特殊符号对待的字符，加上反斜杠后变成普通字符。

    ```tcl
    puts "[expr $X + $Y]"     #2.5
    puts "\[expr $X + $Y\]"   #[expr 1.2 + 1.3]
    puts "\[expr \$X + \$Y\]" #[expr $X + $Y]
    ```

    `\t` -> TAB；
  
    `\n` -> 换行符；
  
    ```tcl
    puts "a\tb"
    # a		b
    puts "a\nb"
    # a
    # b
    ```
  
  - 其他符合：
  
    `""`：TCL解释器对双引号中`$`和`[]`符合会进行变量置换和命令置换。
  
    ```tcl
    puts "\t[expr $X + $Y]"
    # 2.5
    ```
  
    `{}`：花括号中，所有特殊字符都将成为普通字符，TCL解释器不会对其特殊处理。
  
    ```tcl
    puts {\t[expr $X + $Y]}
    # \t[expr $X + $Y]
    ```
  
    `#`：表示注释。

### 变量、数组、列表

- 变量

  定义：`set 变量名 变量值`

  取值：`$变量名`

  ```tcl
  set cell "bufx2"
  # bufx2
  puts $cell
  # bufx2
  set cell "ivtx2"
  # ivtx2
  puts $cell
  # ivtx2
  
  set a 2
  # 2
  puts $a_1
  # can't read "a_1": no such variable
  puts ${a}_1
  # 2_1
  ```

- 数组

  定义：set 数组名(元素名) 值

  取值：$数组名(元素名)

  ```tcl
  
  ```

  

### 控制流

### 过程函数

### 正则匹配

### 文本处理

