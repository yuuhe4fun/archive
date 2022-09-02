# 总线简介
- 总线是CPU、内存和I/O之间交换数据的共同通道，将一根信号线在多个模块间共享进行通信。
- 当两个模块通过总线交换数据时，发起访问的一侧称为总线主控master，接受访问的一侧属于总线从属slave。
- 总线的构成：
   - 数据总线：传输交换的数据
   - 地址总线：指定访问的地址
   - 控制总线：负责总线访问的控制
- 各个信号的时序、进行交换的规则等称为**总线协议**
- 通过总线进行交换数据的整个过程称为**总线传输**
   - [I]申请使用总线
      - 基于总线的共享机制，其上接有多个总线主控Master，因此需要对Master使用请求进行调停。
      - 访问总线的权力称为总线控制权，对多个访问进行调度称为总线仲裁。
      - Master在访问总线前需要先向总线控制器**申请总线控制权**。
   - [II]许可使用总线
      - 总线控制器依据仲裁规则对总线的使用进行许可授权。
   - [III]请求访问
      - 获得总线控制权的Master对Slave发送访问请求，即“地址”“操作（读or写）”“待写入的数据”等。
      - 一般总线结构会为每个Slave分配地址空间，总线控制器中的地址解码器根据待访问的地址产生片选信号（Chip select，芯片选择信号）控制信号对Slave的访问。
   - [IV]请求的应答
   - [V]释放总线控制权
      - 总线使用完毕后Mater通知总线控制器释放总线控制权。
# 总线的设计与实现

- 通过总线进行访问，需要预先设定Master与Slave间的通信规则，即总线协议。
# AXI总线

AMBA AXI协议支持高性能高频的系统设计，该协议拥有以下优点：

- 适合高带宽和低延迟的设计
- 不需要复杂的桥即可提供高频操作
- 可以满足多种组件的接口需求
- 适合高初始访问延迟的存储器控制器
- 提供互联架构实现的灵活性
- 向后兼容AHB与APB接口

同时，该协议还拥有以下特点：

- 分离的地址/控制和数据阶段
- 通过使用字节选通信号的方式支持非对齐数据传输
- 使用突发传输时只需要传输起始地址
- 允许地址信息提前于实际数据传输发送
- 分离的读写数据通道，这可以提供低损耗的DMA
- 支持发出多个未完成的地址
- 支持乱序传输完成
- 允许简单添加寄存器阶段以提供时序收敛

AXI协议同时包含了低功耗操作所需的信号扩展，也包含了AXI4-Lite协议，该协议是AXI4的子集，有着更简单的控制寄存器接口和组件。

## （一）VALID/READY 握手机制
reference: manual A3.2.1<br />AXI 总线共有 5 个独立的通道，分别为写地址、写数据、写回应、读地址、读数据通道。
> 读地址 (AR) read address
> 读数据 (R) read data
> 写地址 (AW) write address
> 写数据 (W) write data
> 写回复 (R) write response

5条通道相互独立，有一些细小的差别，但共同使用一套握手机制：VALID/READY 机制，来实现信息的传递。VALID/READY 机制这个赛高啊，这个好啊，ARM 的手册上这么夸：
> 作为一种双向流控机制，VALID/READY 机制可以使发送接收双方都有能力控制传输速率。

在握手机制中，通信双方分别扮演**发送方**(Source)和**接收方**（Destination），两者的操作（技能）并不相同。<br />**发送方**置高 **VALID** 信号表示发送方已经将数据，地址或者控制信息已经就绪，并保持于消息总线上。<br />**接收方**置高 **READY** 信号表示接收方已经做好接收的准备。<br />![](https://raw.githubusercontent.com/yuuhe4fun/cloudimg/main/img/1657352142228-0cd2b2a8-70bb-4f88-8b69-2e31fbb14f18.png)<br />当双方的 VALID/READY 信号同时为高，在时钟 ACLK 上升沿，完成一次数据传输。所有数据传输完毕后，双方同时置低自己的信号。<br />所谓的**双向流控**机制，指的是发送方通过置起 VALID 信号控制发送的时机与速度，接收方也可以通过 READY 信号的置起与否控制接收速度。<br />发送方拥有传输的主动权，但接收方在不具备接收能力时，也能够置低信号停止传输，反压发送方。
<a name="Gqrvy"></a>

### VALID/READY 的三种情况
VALID/READY 信号按照到达的先后顺序可以分为 3 种情况：
<a name="FBf3i"></a>

#### 1.VALID 信号先到达
![](https://raw.githubusercontent.com/yuuhe4fun/cloudimg/main/img/1657352142078-850e7fe5-8e22-4bf4-b1ef-0defd89e927f.png)<br />发送方 VALID 信号早早就到了，这时还不到 T2 时刻，并带来了新鲜的数据（数据通道），地址或者控制信息（地址通道），总之是信息（Information）。<br />但过了 T2 也没见到接收方的 READY 信号。原来是接收方还忙着，可能上一次的数据还没存完，还堵在数据通路上，忙过了 T2 才来。<br />好吧，那也行，T3 时刻传输完成。<br />在这种情况下，接收方通过 READY 信号控制了传输速度，反压了发送速度。<br />协议规定在这种情况下，VALID 信号一旦置起就不能置低，直到完成握手（handshake occurs),至少传输一周期数据。<br />在设计接收方逻辑时，检测到 VALID 信号置起，如果系统正忙，完全可以让发送方等待，发送方在完成传输之前都不会置低 VALID 信号，不需要考虑发送方撤销传输的可能。<br />协议另外规定：发送方不能通过等待接收方 READY 信号来确定置起 VALID 信号的时机<br />这句中文阅读理解有点难，原文为：
> A source is not permitted to wait until **READY** is asserted before asserting **VALID**.

笔者个人理解，READY 信号可能先到达，如下图的情况。但是设计发送方逻辑时，不能将 READY 信号作为置高 VALID 逻辑的条件，比如将 READY 信号通过组合逻辑生成 VALID 信号。<br />换句话说，发送方准备发送，置起 VALID 信号是完全**主动与独立**的过程。接收方 READY 信号按照协议可以依赖发送方 VALID 信号，但如果此时发送方也依赖接收方信号，就会造成死锁的情况，所以协议在这里强调了 VALID 信号的主动性。
```verilog
module axi_src
	...
	//assign VALID = SRC_CONDITION && READY;//NOT permitted and may cause deadlock
	assign VALID = SRC_CONDITION;//permitted 
	...
endmodule

module axi_dst
	...
	assign READY = CONDITION && VALID;//permitted
	...
endmodule
```

#### 2.READY 信号先到达
![](https://raw.githubusercontent.com/yuuhe4fun/cloudimg/main/img/1657352142183-86a6d85a-f184-4c59-87c2-1d8ca06f6c63.png)<br />READY 信号很自由，可以等待 VALID 信号到来再做响应，但也完全可以在 VALID 信号到来前就置高，表示接收端已经做好准备了。<br />而且，READY 信号与 VALID 不同，接收方可以置起 READY 之后发现：其实我好像还挺忙，然后置低 READY 信号。只要此时 VALID 信号没有置起，这种操作是完全可以。
<a name="qNRXw"></a>

#### 3.同时到达
![](https://raw.githubusercontent.com/yuuhe4fun/cloudimg/main/img/1657352142196-c15b2b5e-1e12-40c1-9355-e812b4a160f3.png)<br />同时到达就很简单，等到下一个时钟上升沿 T2，传输就这么轻松愉快地在一个时钟周期里完成了。
<a name="iObUN"></a>

### 实现细节
前文我们讨论过， READY 信号原则上由接收方自身的接收状况以及 VALID 信号控制。（或者仅由接收方自身的接收状况决定）协议中没有规定 READY 信号的默认状态，即未进行传输时的电平状态（default value）。<br />协议建议 AW/AR READY 信号（这里 AW/AR 指的是读写地址通道的 READY 信号，将在第二章中正式引入）的默认电平为高电平。若默认电平为低，则每次传输至少需要 2 个周期才能完成，第一个周期置高 VALID 信号，第二个周期从机才会置高 READY 信号。相当于每次传输增加 1 个周期时间开销，这在某些情况下会对传输效率有较大的影响。
<a name="JjMjh"></a>
## （二）架构
reference: AXI 总线的传输通道（对应手册中的章 A1.3）、各通道信号（A2）、通道间的关系（A3）以及 AXI 总线的拓扑连接（A1.3.2）
<a name="lOiY1"></a>
### 五个独立通道
AXI4 总线的一大特征是它有 5 个独立的传输通道，这些通道都只支持单向传输。<br />作为类比，SPI 总线有 2 条单向传输通道：MISO， MOSI。SPI 输入和输出的数据，大路朝天，各走一条。<br />而作为对比， IIC 协议则只有 SDA 一条双向通道，输入输出数据只能在这一条通道上分时双向传输。<br />单向传输的通道意味着两端的终端节点是有身份差距的，好比水只能从上游流到下流。在 AXI 总线传输中，通道两端分为 Master 主机与 Slave 从机，主机总是发起读写请求的一方。常见的主机有CPU、DMA，而存储介质控制器（比如 DDR 控制器）则是典型的从机。主机可能通过从机读取或者写入存储介质。而显然从机不可能主动向 CPU 写入数据。<br />通道的读/写定义都是根据主机来定义的，那么五个通道都有谁呢：
> 1. 读地址 （AR） read address
> 1. 读数据 （R） read data
> 1. 写地址 （AW） write address
> 1. 写数据 （W） write data
> 1. 写回复 （R） write response

5 个是不是很奇怪，你看读/写怎么也得是个 2 的倍数不是，那为什么不是 6 条，为什么没有读回复？其实，读回复借用了读数据通道。至于为什么有独立的写回复，而无读回复通道，我们之后再讲。
<a name="DSL71"></a>
### 读写传输操作中的通道们
首先是写传输操作（Write transcation）：<br />如下图所示，主机首先在写地址通道（AWC）上告知从机本次传输操作（transcaction，对应后文中的"传输操作"）的特性，包括地址和控制信息。<br />然后，在写数据通道（WDC）向从机写入数据，一次传输操作中可能包括多个数据传输（data transfer）。<br />最后，从机在接收到写数据后，通过写回复通道（RC）将本次传输操作的响应告知主机。主机以收到从机的响应信号，作为本次传输操作结束的标志。手册中强调，写回复是针对整个传输操作（transcaction）的，而不是针对每个写入数据（data transfer）。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/26861020/1657352890519-4db3feda-827e-437b-8018-52912105b7f5.png#clientId=uea8d41f8-f3bb-4&crop=0&crop=0&crop=1&crop=1&from=paste&id=u44007054&margin=%5Bobject%20Object%5D&name=image.png&originHeight=413&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&size=62349&status=done&style=none&taskId=u96f9864f-ad64-4053-841d-a8c77e46ba5&title=)<br />所有传输操作中，RC 与 WDC 的关系都如上图所示，写回复必然是在收到传输操作中最后一个写数据之后触发。<br />但是，AWC 与 WC 的关系并不局限于一种情况。一般来说，写数据都发生在写地址操作之后，但也不是绝对的，在有些情况下，可以先写数据，或者在同一周期写入数据与地址，都是允许的。这也表现了 AXI 通道之间的独立性。（例外：RC 和 WDC 间的独立性较弱）<br />接下来看读操作（Read transcation）：<br />读操作只涉及两个通道，首先主机在读地址通道（ARC）上写入本次传输操作（Transcation）待读取数据的地址以及控制信息。<br />从机在接收到地址后，将该地址上的数据通过读数据通道（RC）传输给主机。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/26861020/1657352890498-1a7f45e0-6afd-4821-9bf1-cb51586dc0fa.png#clientId=uea8d41f8-f3bb-4&crop=0&crop=0&crop=1&crop=1&from=paste&id=ud6408927&margin=%5Bobject%20Object%5D&name=image.png&originHeight=302&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&size=47711&status=done&style=none&taskId=ue71e7955-8366-478f-b0d0-0f29a6744c0&title=)<br />值得注意的是， AR 虽然名字为读地址通道，但实际上仍由主机写入地址，只不过是写入要读取数据的地址。读地址通道，这个名字确实有点歧义，主机读操作地址通道表达得更贴切一些。<br />读操作时通道间（ARC&RC）完全独立，当然从机发出读数据一定发生在主机写入读地址后，从机不能未卜先知，是吧。<br />无论是读写操作，AXI 总线支持，或者说基于突发传输（Burst Transaction）。简单来说，主机可以写入起始地址以及突发传输的长度等信息，从机将起始地址开始，依次接收主机传输的写数据，或者读取连续地址上的数据，作为读数据传输给主机。所以上面两张图中，一次传输操作中（Transcation） 中包括了一次地址与控制信息（Address & Control）、多个数据（data transfer）。<br />突发传输又称猝发传输，详情可见后续更新的文章：[深入 AXI4 总线（三）传输事务结构](https://zhuanlan.zhihu.com/p/46538028)
<a name="GxRbw"></a>
### 通道定义
五个通道有一些共同的定义：<br />每个通道都使用 VALID/READY 握手机制，详情可见：[（一）VALID/READY握手机制](#qm1ak)<br />在读写数据（WC&RC）两条数据通道中，传输突发传输（Burst Transaction）中的最后一个数据，必须要给出 LAST 信号，来标识这是此次突发传输中的最后一个数据（data transfer）。<br />在共同的定义之外，各个通道有自己的定义。<br />**读&写地址通道（ARC&AWC）：**<br />写入本次传输操作所需的地址和控制信息，读写操作都拥有各自的地址通道。<br />**读数据通道（RC）：**<br />读数据通道上包括从机发送给主机的读数据，以及从机对于本次读传输操作的回复，具体的读操作状态回复情况会在之后讨论。总线数据位宽可以是 8,16,64,128,256,512 或者是 1024 比特。<br />**写数据通道（WC）：**<br />写数据通道用于将主机的写数据传输至从机，位宽和RC的数据位宽相同。WC 有一点 RC 所不具有的特性是拥有 STROBE 信号，用于标识写数据中有效的传输字节。即有些无效的数据，出于减少主机工作量的目的，或者在读写宽度不对称时，被放到写数据通道上和有效数据一起发送。而 STROBE 的信号的作用就是标识出这些无用的数据，告知从机不需要接收无用数据。（Master：我太懒，以至于把所有信号都送过来了）<br />写数据通道设计有缓存，可超前于从机响应本次传输操作，发起下一次写传输操作。<br />**写回复通道（RC）：**<br />用于从机将写操作响应回复给主机。所有写传输操作都需要以写回复通道上接收写响应作为完成信号。再次强调，写回复是针对一次传输操作（transcation）的，而不是针对每一次写数据（data transfer）。<br />那么问题来了，为什么只有写回复通道而没有读回复通道呢？<br />这个问题可以从**数据流向**看出来，主机在读取数据时，数据在读通道上传输，流向为从机到主机。而读回复由从机向主机报告读操作的情况，信号的数据流向也是从机到主机，所以读回复可以合并在读数据通道中，搭个顺风车。<br />但写回复通道的数据流向就和写数据**相反**。写数据是从主机到从机，而写回复为从机报告写操作的完成情况，流向为从机到主机，无法合并到写数据通道中，另一方面，写回复又是不可或缺的，所以就有了一条独立的写回复通道。
<a name="y8SNc"></a>
### 通道上的信号们
<a name="BsX12"></a>
#### 全局信号
AXI 总线中有两个全局信号：ACLK，全局的时钟信号，所有的传输操作都发生在 ACLK 的上升沿。<br />ARESETn，全局复位信号，低电平有效。在复位问题上，AXI 规定了一些细节，会在后续的文章中讨论。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/26861020/1657352890346-cd1343d4-fb55-4396-ba01-5877b7d8ed03.png#clientId=uea8d41f8-f3bb-4&crop=0&crop=0&crop=1&crop=1&from=paste&id=u2911aff3&margin=%5Bobject%20Object%5D&name=image.png&originHeight=169&originWidth=653&originalType=url&ratio=1&rotation=0&showTitle=false&size=36306&status=done&style=none&taskId=ufa147b95-a5ba-413b-b3a2-244e6599125&title=)<br />**注意**：AResetn 一般是一个同步复位信号，A 代表 AXI，而不是 Async。
<a name="Uo85e"></a>
#### 写地址通道
![image.png](https://cdn.nlark.com/yuque/0/2022/png/26861020/1657616904431-f024691c-c93a-4221-b3a5-91724b92d349.png#clientId=u9cff70e2-c997-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=805&id=uad6532b9&margin=%5Bobject%20Object%5D&name=image.png&originHeight=758&originWidth=658&originalType=binary&ratio=1&rotation=0&showTitle=false&size=152490&status=done&style=none&taskId=u69cb487d-d5d1-447b-9eb1-035a157a345&title=&width=699)<br />写地址通道的信号可以分为 3 部分：经常用到的基础信号、突发传输的控制信号、内存访问相关以及其他的在基础阶段不是很常用的信号。<br />基础信号即 AWADDR：传输操作的起始地址，AWVALID 以及 AWREADY：所有通道都使用的握手信号。<br />突发传输指的是传输一次起始地址后，进行多次地址上连续的读写操作。突发传输有关的操作包括：AWLEN：突发传输的长度，即在一次突发传输中数据传输的个数。AWSIZE：每次突发传输中的数据传输的位宽。AWBURST：突发传输的类型。<br />其他信号包括和内存原子操作有关的 AWLOCK，AWCACHE，AWPROT 以及用于用户自定义的 AWUSER 信号，都将在以后的文章涉及。（等我自己先用到再说）<br />**读地址通道**<br />读地址通道和写地址通道的信号十分类似，就不再从 specification 中截图以及介绍了。<br />**写数据通道**<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/26861020/1657352890525-32d08c75-d27c-4d3c-b879-b7a58c5eabf2.png#clientId=uea8d41f8-f3bb-4&crop=0&crop=0&crop=1&crop=1&from=paste&id=u9733f984&margin=%5Bobject%20Object%5D&name=image.png&originHeight=404&originWidth=698&originalType=url&ratio=1&rotation=0&showTitle=false&size=82800&status=done&style=none&taskId=u0bcbda4b-6ddc-4f2c-bbda-0eaaf44cf8e&title=)<br />值得注意的是 AXI4 不再支持 WID 信号，这和 AXI4 的乱序机制有关，AXI4 规定所有数据通道的数据必须顺序发送。<br />WDATA 与常见的握手信号不再赘述，WDATA 的可使用位宽可以见上文。WSTRB 信号用于标记传输数据中有效的字节，每个 WSTRB 位对应一个字节的位宽，比如数据位宽为 64 位，那么 WSTRB 信号的位宽就是 1 个字节,共 8 位。(感谢评论区纠正)<br />WLAST 标识一次突发传输中最后一次数据传输，如果没有正确的 WLAST 的信号，就会造成写入地址的混乱，导致从机无法正确接收写数据，从而造成从机不再拉高 READY 信号的现象。<br />**写回复通道**<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/26861020/1657352891999-ae089ef4-6663-41e1-8e88-ac047a34280c.png#clientId=uea8d41f8-f3bb-4&crop=0&crop=0&crop=1&crop=1&from=paste&id=u4fdea15f&margin=%5Bobject%20Object%5D&name=image.png&originHeight=329&originWidth=718&originalType=url&ratio=1&rotation=0&showTitle=false&size=64393&status=done&style=none&taskId=u6ba1243b-01c0-4fd6-a5aa-29a6a5b2b4b&title=)<br />与写数据通道不同，写回复通道支持 BID，即支持乱序的写回复，关于乱序的问题，我们稍后再谈。BRESP 回复上一次的写状态。<br />**读数据通道**<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/26861020/1657352892090-bec4a84b-7bc1-4189-b6d6-c65c998009c8.png#clientId=uea8d41f8-f3bb-4&crop=0&crop=0&crop=1&crop=1&from=paste&id=ua065f1ad&margin=%5Bobject%20Object%5D&name=image.png&originHeight=416&originWidth=710&originalType=url&ratio=1&rotation=0&showTitle=false&size=81482&status=done&style=none&taskId=u99477ec3-3e06-4b05-a657-fe01c5e5297&title=)<br />读数据通道与写数据通道类似，区别有两点：<br />一，支持 RID 信号。<br />二，因为读回复信息在读数据通道上传递，所以集成了 RRESP 信号，用于返回读状态，值得注意的是读回复信号和读数据一样，发送方（source）为从机（slave）。
<a name="cgwvO"></a>
### 通道间的关系
首先，再次声明一般情况下 AXI 的五个通道之间是独立的。但凡事都免不了例外，是吧。AXI 的通道间需要保证以下三种联系（relationship）：

- 写回复必须在其所属传输的最后一个写数据完成后（write response must follow the last write transfer in the transaction）
- 读数据必须在接收到读地址信号后产生
- 通道间的握手需要满足通道间的握手依赖性(handshake dependencies)

除了这三种联系外，通道之间保持独立关系。前两种联系已在上文中讨论，接下来我们讨论第三种情况：通道间的握手信号依赖
<a name="Hdu2N"></a>
## **握手依赖关系**
协议规定握手依赖的目的是为了防止死锁（deadlock），手册定义于章节 A3.3 ，主要的原则还是第一章中说过的两条：

- 发送方 VALID **一定不能依赖**接收方 READY 信号
- 接收方 READY 信号可以检测到 VALID 置起后再置起有效，换句话说，**可以依赖** VALID 信号

下述内容就是对上述原则在不同情况下的具体表述，为选读内容，无需实现协议的读者可以略过。<br />我们首先来看**读传输**的情况，读地址通道中主机为发送方、从机为接收方；读数据通道中主机为接收方、从机为发送方。其中，<br />图中的单头箭头表示：其**指向的信号**可以在箭头**起始信号**置起之前或之后置起（**无依赖**）<br />图中的双头箭头表示：其**指向的信号**必须在箭头**起始信号**置起之后置起（指向信号**依赖**起始信号）<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/26861020/1657352892241-d4d4d8c0-8bcb-42f2-a99c-f1e1f169525b.png#clientId=uea8d41f8-f3bb-4&crop=0&crop=0&crop=1&crop=1&from=paste&id=u7757cae8&margin=%5Bobject%20Object%5D&name=image.png&originHeight=198&originWidth=566&originalType=url&ratio=1&rotation=0&showTitle=false&size=35638&status=done&style=none&taskId=ubd70a1c2-2b0d-4b30-858b-16b2198fc73&title=)<br />具体的依赖可以表述为：

- 主机的 **ARVALID** 信号不能等待从机的 **ARREADY** 信号置起后才置起，反之可以
- 从机的 **ARREADY** 信号可以先于主机的 **ARVALID** 信号置起（如单头箭头所示）
- 从机的 **RVALID** 信号必须等待 **ARVALID** 和 **ARREADY** 信号均置起后置起（如两个双头箭头所示）
- 从机的 **RVALID** 信号不能等待主机的 **RREADY** 信号置起后才置起，反之可以
- 主机的 **RREADY** 信号可以在 **RVALID** 置起前置起，换句话说：无依赖关系（如单头箭头所示）

接下来是**写传输**的情况，在写数据以及写地址通道中，主机作为发送方；写回复通道中从机作为发送方<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/26861020/1657352892381-ce61598e-852b-428b-ad95-965b94aa3aa6.png#clientId=uea8d41f8-f3bb-4&crop=0&crop=0&crop=1&crop=1&from=paste&id=uf88589dc&margin=%5Bobject%20Object%5D&name=image.png&originHeight=240&originWidth=709&originalType=url&ratio=1&rotation=0&showTitle=false&size=63827&status=done&style=none&taskId=ud339847e-0009-4557-ba29-12046dfd090&title=)<br />从图中看到，只有 2 组代表着依赖关系的双头箭头指向了 **BVALID**，这代表从机首先等待 **WVALID** 以及 **WREADY** 置高，此时写传输启动。再等待 **WLAST** 置起，此时传输完成。只有在传输完成后才能根据情况生成写回复信号中的 **BRESP** 信号。此时置高 **BVALID** 信号表示从机的写回复已经就绪。<br />此外另外三组单头箭头表示 **AWREADY** 、**WREADY** 信号可以根据 **AWVALID**、**WVALID** 信号生成，但不存在依赖。同样的关系还存在于 **BREADY** 与 **BVALID** 信号之间。本文此处不再展开具体依赖，可以参阅官方手册的 A3-43 页获得详细的表述。<br />(手册中图 A3-6 即上图中的注释部分似乎将 BVALID 误写为 WVALID，因为主机置起 WVALID 似乎与本次传输的 WLAST 无关，而且文本中也提到 BVALID 依赖于 WLAST)<br />在 AXI4 中，写传输相较此前**增加了从机置起 BVALID 信号所需的依赖**。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/26861020/1657352892982-9a799bbe-48e1-4999-a86c-61f3eb2c1df7.png#clientId=uea8d41f8-f3bb-4&crop=0&crop=0&crop=1&crop=1&from=paste&id=ubfd203f0&margin=%5Bobject%20Object%5D&name=image.png&originHeight=296&originWidth=690&originalType=url&ratio=1&rotation=0&showTitle=false&size=75222&status=done&style=none&taskId=u86e2a4cd-69e1-49c6-8ba0-ccdf92c1b95&title=)<br />从上图中增加的依赖可以看出，从机 **BVALID** 信号有效需要等待 **AWVALID** 以及 **AWREADY** 信号置起，完成地址传输作为前提条件之一。这在某种程度上是自然的，肯定需要等待从机获取本次操作的地址以及数据之后才能回复写入情况。一般而言地址会在数据之前传输完毕，不过协议并没有要求这点，所以从机需要等待地址写入后再产生写回复，作为本次传输完成的标志。
<a name="vjn8h"></a>
## 拓扑连接
从上述信号我们可以发现，原生的 AXI 总线为单对单的拓扑结构，主机与从机间对应信号直接相连，比如下图中的 VALID / READY 信号。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/26861020/1657352893067-7bc8ba16-ab99-450c-a434-ece712cbf50d.png#clientId=uea8d41f8-f3bb-4&crop=0&crop=0&crop=1&crop=1&from=paste&id=u53668462&margin=%5Bobject%20Object%5D&name=image.png&originHeight=346&originWidth=742&originalType=url&ratio=1&rotation=0&showTitle=false&size=93805&status=done&style=none&taskId=u08bb96ba-51ea-4764-9a0e-f99d145991e&title=)<br />在一个系统中，显然不可能只有单个主机和从机。AXI 总线需要借助总线互联IP（ Interconnect ）来实现多对的的拓扑，结构如下图。Interconnect 拥有多个 Master/Slave 接口，并在内部基于轮询或者优先级策略对多路数据进行路由转发，我们将在后续的文章中详细讨论。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/26861020/1657352893604-924f4c22-cdcd-457f-a0e0-d0afc4d53c13.png#clientId=uea8d41f8-f3bb-4&crop=0&crop=0&crop=1&crop=1&from=paste&id=u13859b99&margin=%5Bobject%20Object%5D&name=image.png&originHeight=132&originWidth=517&originalType=url&ratio=1&rotation=0&showTitle=false&size=24624&status=done&style=none&taskId=u355cf116-fe43-4453-97b9-64e6ab527fc&title=)
<a name="QD1RP"></a>
## **实现细节**
本文的最后讨论一个协议的实现细节。如果读者仅仅想了解 AXI 协议的使用，那么每篇文章的实现细节章节可以跳过。这个章节面向有实现 AXI 协议需求的读者。<br />AXI 协议并没有对写数据以及写地址通道中的握手信号先后顺序做出规定。因此，实现中数据可以先于地址完成握手。这适用于一些情况，手册中举了个例子，比如写地址通道为了满足时序，触发器的级数与写数据通道不同，这样一来两通道握手的先后顺序可能不同。<br />这反映了 AXI 通道间的独立性，利用该特性可调整路径中的触发器，满足时序约束。
