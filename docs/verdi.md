- 加载信号

g：加载信号 Get signals。在弹出的窗口选中需要添加的信号，点击滚轮选择需要添加信号的位置
代码中选中信号 Ctrl+W 将信号加载在波形中

- 放大缩小

Z：Zoom In
z：Zoom Out
f：Zoom All

- 波形保存.rc

调出波形，为方便下次使用，按键shift+S可以将波形保存为*.rc格式文件，使用波形文件时，按快捷键r打开界面，选择波形。可以避免下次打开重新添加信号

- 移动波形窗口信号位置

使用滚轮在波形窗口选中信号，然后拖到代码窗口即可查看选中波形的代码逻辑

- 对信号的操作

点击波形窗口，选中信号，使用Delete删除信号。
选中信号，使用Ctrl+p复制选中的信号，然后使用insert按键插入，粘贴的位置由黄线所在位置决定。

- 显示信号的操作

快捷键 h
按H显示波形信号全路径，再按H撤销

- 显示状态机及其名称

选中状态机状态的波形，在nWave窗口选中Tools>Bus Contention>打开状态机，（关闭），返回查看波形已经变为状态的名称。
补充：选择具有状态机的模块选中，然后在主工具栏选择Tools>Extract interative FSM
  ![](https://raw.githubusercontent.com/yuuhe4fun/cloudimg/main/img/20210524173820962.png)
点击OK，然后点击下面的状态机，即可看到状态的跳转。
  ![](https://raw.githubusercontent.com/yuuhe4fun/cloudimg/main/img/20210524173834463.png)

- 改变信号和波形颜色

按C或者T修改信号或者波形颜色，方便查看

- 参考信号数值

选中Source>Active Annotation即可看到每一个信号，参数的数据显示

- 新建组与对组重命名

将黄线放到最后一行，添加信号即可新建一个信号组。然后鼠标选中右键即可进行相关操作，也可以选中一个模块，直接Ctrl+4即可将接口信号加入波形窗口

- mark标记功能

为方便波形定位，按键shift+M使用mark功能标记一下，可以自定义名称和颜色，方便查找。
