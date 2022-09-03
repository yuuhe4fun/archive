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
  set cell_1(ref_name) "bufx2"
  set cell_1(full_name) "top/cell_1"
  set cell_1(pins) "A B C"
  
  puts $cell_1(ref_name)
  # bufx2
  
  array size cell_1
  # 3
  array names cell_1
  # ref_name pins full_name
  # 这里的输出顺序？//TOFIX
  ```

- 列表：标量的有序集合

  定义：`set 列表名 {元素1 元素2 元素3 ......}`

  取值：`$列表名`

  ```tcl
  set ivt_list {ivtx2 ivtx3 ivtx8}
  puts ivt_list
  # ivtx2 ivtx3 ivtx8
  ```

  常用列表操作命令：

  ![](assets/image-20220903084202572.png)

  ```tcl
  set list1 {bufx1 bufx2 bufx4}
  set list2 {ivtx1 ivtx2 ivtx4}
  concat $list1 $list2
  # bufx1 bufx2 bufx4 ivtx1 ivtx2 ivtx4
  
  llength $list1
  # 3
  llength [concat $list1 $list1]
  # 6
  
  # lindex返回列表中的第n个元素（从0开始计数）
  lindex $list1 1
  # bufx2
  
  # lappend
  set a {1 2 3}
  # 注意读取需要加$，此处不是读取
  lappend a 4
  puts $a
  1 2 3 4
  ```

  ```tcl
  # 如何得到列表list1{a b c d e f}中的最后一个元素
  set list1 {a b c d e f}
  llength $list1
  # 6
  expr [llength $list1] - 1
  # 5
  lindex $list1 [expr [llength $list1] - 1]
  # f
  ```

  ```tcl
  set a {1 2 3}
  set b {4 5}
  lappend a $b
  # 1 2 3 {4 5}
  
  # 如果这里我们想得到4
  lindex [lappend a $b] 3
  # 4 5
  lindex [lindex [lappend a $b] 3] 0
  # 4
  ```

  - `lsort`列表指令将列表按照一定的规则排序
    - 缺省时默认按照ASCII码进行排序
    - `-real` ——> 按照浮点数值大小排序
    - `-unique` ——> 唯一化，删除重复元素

  ```tcl
  set list1 {c d a f b}
  # c d a f b
  lsort $list1
  # a b c d f
  
  set list2 {-2 3.1 5 0}
  # -2 3.1 5 0
  lsort -real $list2
  # -2 0 3.1 5
  
  set list3 {a c c b a d}
  # a c c b a d
  lsort -unique $list3
  # a b c d
  
  # 获取列表list1中最小值
  set list1 {0 1.2 -4 3 5}
  # 0 1.2 -4 3 5
  lsort -real $list1
  # -4 0 1.2 3 5
  lindex [lsort -real $list1] 0
  # -4
  ```

- 运算

  - 数学运算

    ```tcl
    a+b
    a-b
    a*b
    a/b
    ```

  - 逻辑运算

    ```tcl
    a<=b
    a>=b
    a==b
    a!=b
    ```

  - `expr`数学运算表达式，将运算表达式求值

    ```tcl
    expr 6 + 4
    # 10
    expr 6 - 4
    # 2
    
    expr 5/2
    # 2
    expr 5/2.0
    # 2.5
    expr 5.0/2
    # 2.5
    ```


### 控制流

#### if

- 语法格式：

  ```tcl
  if {$a > $b} {
  puts $a
  } else {
  puts $b
  }
  ```

- 注意，上例中脚本语句的**'{'**一定要写在上一行，因为如果不这样，TCL 解释器会认为if命令在换行符处已结束，下一行会被当成新的命令，从而导致错误。

### 过程函数

### 正则匹配

### 文本处理

