
# 1553B芯片使用流程

___

## 1 上电与复位

### 1.1 自测试

* 自测试包括协议自测试和RAM自测试。协议自测试在上电时自动进行。协议或RAM自测试的初始化由主处理器通过“开始复位寄存器“（Start/Reset Register，0x03H）命令完成。

* 协议测试：测试所有寄存器，曼彻斯特解码器，协议逻辑和存储器管理日志。20MHz的时钟需要1.6ms，10MHz的时钟需要3.2ms。

* RAM测试：该过程每个字需要10个时钟周期，也就是20MHz的时钟需要2.0ms，10MHz的时钟需要4.1ms。
硬件复位后通过读取“内建自测试状态寄存器“（Buildin Self Test State Register，0x1CH）可知芯片协议自测试结果，若值为：0xA800h则测试通过。

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/9d42918715a84b07918ddbaa5cdef8a2.png">
<div align='center'>图 1-1 自测试状态标记寄存器</div>
</div>
### 1.2 硬件复位

复位引脚 MSTCLR_n 上电为低电平，至少为持续100ns的负脉冲。

硬件复位后会有自测试，软件复位不会有。自测试时间在50ms以内。所以，复位之后，最好有个延时函数，延时100ms，然后再进行寄存器的初始化。

### 1.3 软件复位

软件复位：对“开始复位寄存器“ （Start/Reset Register，0x03H）的bit0写1；

### 1.4 RT复位命令

RT复位命令：RT接收到BC发来的复位命令也会复位。

___

## 2 寄存器与配置

B64843GC共有64个寄存器，其中地址为0x00-0x1F的共32个寄存器用于配置和测试，地址为0x20-0x3F的共32个寄存器用来存储测试失败时的向量信息。

0x00-0x1F 共32个寄存器中只有24个有用的操作寄存器，剩下的用于测试。

只需要配置重要的寄存器，其他功能更加复杂的，只需要保持默认的初始值即可保证正常工作。

### 2.1 配置寄存器#1（Configuration Register #1）

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/eaa89dd8ac0e4f1983206cc87f93e942.png">
<div align='center'>图 2-2 配置寄存器#1</div>
</div>

bit15、bit14共同决定工作模式为BC还是RT；

bit8控制是否周期发送，周期通过写“BC帧间时间寄存器”（BC FRAME TIME，0x0D）设置，在输入时钟频率为10MHz时钟时1LSB为100us，40MHz时1LSB为25us；

bit5使能消息间定时器；

bit4使能消息重试。

### 2.2 配置寄存器#2（Configuration Register #2）

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/5a14d7c5f8f64b2795b8e88ae90496b0.png">

<div align='center'>图 2-3 配置寄存器#2</div>
</div>

bit15：开启增强中断模式，无论中断屏蔽寄存器是0是1，所有中断都会发生。也就是说发生中断后，就去读中断状态寄存器，然后判断是不是我们预想的那种情况，主要是消息结束中断（bit0：End Of Message）；

bit4：中断是读取中断状态寄存器后清零，还是写“开始复位寄存器“（Start/Reset Register，0x03H）的bit2主动清除中断；

bit3：中断模式为脉冲还是电平。

### 2.3 配置寄存器#3（Configuration Register #3）

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/133d35b881f54ca58cd6f0eb0184f5d0.png">

<div align='center'>图 2-4 配置寄存器#3</div>
</div>

bit15：使能增强模式，在使能任何其他增强模式功能，包括配置其他涉及增强模式功能的“配置寄存器#3-7”（Configuration Register #3-7）之前必须先使能这一位。

### 2.4 配置寄存器#6（Configuration Register #6）

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/e2c39086469e4137a5a580c7f44cb216.png">

<div align='center'>图 3-5 典型BC存储器结构</div>
</div>

bit5：RT地址源（RT ADDRESS SOURCE）。

如果RT地址源（RT ADDRESS SOURCE）为1，RT地址和RT奇偶校验位通过以下步骤配置：

1. 写配置寄存器3的第15位（增强模式使能）为1；
2. 写配置寄存器4的第三位（与配置寄存器5一起锁存RT地址）为1；

如果RT地址源（RT ADDRESS SOURCE）为0：

1. RT_AD_LAT连接到逻辑“0”：RT地址和RT奇偶校验位即为RTAD4-RTAD0和RTADP引脚输入的值；
2. RT_AD_LAT从0跳变到1，RTAD4-RTAD0和RTADP引脚输入的值，将在上升沿被锁存，作为RT地址和RT奇偶校验位的值；

___

## 3 BC模式

### 3.1 非增强总线控制器

#### 3.1.1 典型存储器结构

***！B64843GC手册并未详细说明BC模式存储器结构，该部分为BU61580手册内容。***

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/8f43fdfb0fe74e9c8e6bf13934187d18.png">

<div align='center'>图 3-5 典型BC存储器结构</div>
</div>

仅两个“栈指针”（0x0100,0x0104）和两个“消息计数”(0x0101,0x0105)的地址是固定的，

若启用“帧自动重复”（Frame Auto-Repeat）则“初始栈指针”(0x0102,0x0106)和“初始消息计数”（0x0103,0x0107）四个地址将被保留用于增强BC模式。

#### 3.1.2 存储器管理

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/3e714f4801024a9e82c399a773edf4b9.png">

<div align='center'>图 3-6 非增强BC存储器管理</div>
</div>

在该模式下需要用户设置两个“栈指针”（0x0100,0x0104）和两个“消息计数”(0x0101,0x0105)的值，以指定描述块的开始地址以及消息数量。

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/7687153dcfea4f00a5d4d21a6a699250.png">

<div align='center'>图 3-7 非增强BC消息</div>
</div>

### 3.2 增强总线控制器（Enhanced BC）

#### 3.2.1 典型存储器结构

#### 3.2.2 存储器管理

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/e920947d169c486ba1e8deb4fb596392.png">

<div align='center'>图 3-8 BC消息序列控制</div>
</div>

   通过BC初始化指令列表指针寄存器（BC Initial Instruction List Pointer Register, 0x0D）来初始化BC指令列表(BC Instruction List)的开始地址。

   通过增强BC指令表指针寄存器(Enhanced BC Instruction List Registe,0x03)来获取当前正在处理的指令列表的位置，这个寄存器的值会随着消息的处理，每次两个字递增。

   从BC指令列表(BC Instruction List)的首个地址开始，写入操作码和第一块消息控制块的地址。操作码在最简单的情况下，可以全部是执行消息（XEQ）操作。然后我们作为主机只需要读取需要的结果即可。第二种情况是在每一个执行消息操作的后面紧跟着一些压栈操作，将我们需要读取的信息按顺序压入到通用序列(General Purpose Queue)中，这时主机只需要按顺序读取这个序列即可。

   通用序列(General Purpose Queue)的初始地址通过BC通用序列寄存器（0x1F）寄存器设置，且只需要规定高10位的地址即可，低六位应该为0，由1553B芯片从0开始递增，到3F翻转。

   为简化操作，可以将操作码中条件码的第四位写0，可以使所有操作都不取决于条件。（在更复杂的情况下，也可以使用条件码，判断是否出错，或重试了几次，然后再决定是否继续执行消息或者压栈）。

将消息队列的内容写好后，通过对开始复位寄存器(Start/Reset Register,0x03H)的BIT1（BC/MT START）写1使之开始处理消息。
   停止处理消息是通过在最后一个指令的后面写入halt操作码(HAT 0007)，就会停止处理消息

---

#### 3.2.3 消息内容

在增强BC模式下，包括“指令列表”、“消息/控制/状态块”和“数据块”，在RAM中的地址都是没有固定要求的，可以根据自己的需求自行规定,对这三种形式的数据在RAM中的地址进行分区后，应当通过寄存器设定相应的roll over 就是对应的地址范围翻转，就是到达地址分区的最大值后回到地址分区的起始地址，以防止不同分区之间的覆盖。

##### 3.2.3.1 BC指令列表

一个指令包含“操作码”（OP Code）和“参数”（消息/控制/状态块的地址），所以一个指令占用两个字，且指令按地址顺序执行。

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/d5b5e6e38b124a0a91205d01605d9bc1.png">

<div align='center'>图 3-9 OP操作码</div>
</div>

##### 3.2.3.2 消息/控制/状态块

一个块占用10个字。地址由BC指令列表的指针决定。但为了便于管理，应当按顺序排列在一片区域内。

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/37ba9038045d49558679ca05447244db.png">

<div align='center'>图 3-10 消息/控制/状态块</div>
</div>

* BC控制字

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/4bf0706bb7bf4e3db523ab926e5f5124.png">

<div align='center'>图 3-11 BC控制字</div>
</div>

控制字不会被发送出去，只是作为消息的配置信息。

在最简单的情况下，可以只配置最低五位，其他位默认为0。

bit4决定消息结束是否产生中断，

bit3置0，决定1553B协议 ，而不是1553A

bit2-bit0与”命令字”（Command Word）的T/R位一同决定是哪种消息格式。

当前正在处理的BC控制字可通过读取“BC控制字寄存器”（BC Control Word，0x04）得到。

* 命令字

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/dc3013d8eb44440699a81a5d85c6edf3.png">

<div align='center'>图 3-12 命令字</div>
</div>

* 数据块指针

指向数据所在位置。

* 消息间隔

默认为0，即本消息结束后，在最短的时间内发送下一个消息。不再额外使用周期控制的功能。

* 第二个发送命令字（RT-RT）

当消息格式为RT TO RT时，第二个控制字同样由上层提供，写入到此。

* 其他

其它为默认为0，会由1553b芯片来写入。我们只需对需要的信息进行读取即可。需要关注“RT状态字”（RT Status Word）和“块状态字”（Block Status Word），会指示消息是否完成，是否有错。

##### 3.2.3.3 数据块

大小取决于对应的1553b协议命令字的数据计数，地址由”消息/控制/状态块”的第三个字决定。顺序递增排列。
可以通过将一个”数据块”的首地址和它的长度相加，得到下一个”数据块”的首地址。

___

## 4 RT模式

### 4.1 存储器结构

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/157402828bae43a2a5d6a0aceee0c965.png">

<div align='center'>图 4-13 RT模式典型存储器结构</div>
</div>

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/ce5d59e28f4042a4a69719e5f88425cd.png">

<div align='center'>图 4-14 查找表地址映射</div>
</div>

查找表的地址分布如图 4-14所示，查找表的初始地址需要由主机来配置。结合循环缓冲的空间大小，即可将不同类型的数据分配在RAM中data block的空间中。
除此之外还需要结合中断信息对接收或要发送的数据进行处理。

### 4.2 存储器管理

#### 4.2.1 模式配置

设置“配置寄存器#2”（Configuration Register #2）的bit1为1开启增强RT内存管理。开启时可以使用更多的管理模式，否则就只能使用单缓冲模式。

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/ce57746ca5e242d2a12cc356b0d21dcc.png">

<div align='center'>图 4-15</div>
</div>

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/22a450585be24ce1a4aaaf4c8e8f6e2a.png">

<div align='center'>图 4-16</div>
</div>

bit15和MM0-2共同决定存储器管理模式。

当前正在处理的RT字地址控制字可通过读取“RT字地址控制字寄存器”（RT Subaddress Control Word，0x04）得到。

#### 4.2.2 单消息模式

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/073f4fe8c11145b7be9873713f0f3ba1.png">

<div align='center'>图 4-17 单缓冲模式</div>
</div>

架构与BC类似，只是没有指令列表做操作队列。只有一个栈指针。栈指针的位置是固定的(0x0100和0x0104)。栈区描述中，只有查找表入口地址需要自己写入，从循环缓冲接收或发送的数据字也是从查找表指针指向的地址开始的。

单个查找表入口由主机来配置，查找表入口将被存储在“描述符栈”（Descriptor Stacks）中每个消息的开头“消息描述块”的第三个字（“数据块指针”）处。

单缓冲模式功能有限，查找表（Look Up Tabel）需要自己来写，当同一个子地址数据被连续写入或者读取，而用户又没有及时更改查找表的指针，那么数据就会被覆盖写入或者重复读取。

#### 4.2.3 循环缓冲模式

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/19b632b3241440599d54a9d1cdcfd771.png">

<div align='center'>图 4-18 循环缓冲模式</div>
</div>

#### 4.2.4 子地址双缓冲模式

<div align='center'>
<img src="https://i-blog.csdnimg.cn/direct/c0c9f9c3de7b419aaa19fdf4d66b69ed.png">

<div align='center'>图 4-19 子地址双缓冲模式</div>
</div>

在双缓冲模式下，每个接收子地址将被分配两个大小为32字的数据块，在任意时刻，两个数据块中的一个处于激活状态，另一个处于非激活状态，下个接收到的数据将被存储在激活数据块中，在接收到有效数据后，控制器将自动切换数据块的激活状态。

在双缓冲模式下读取最近接收到的数据的操作流程：

* 通过子地址控制字禁用双缓冲模式以暂时将子地址存储器管理模式切换为单消息模式；
* 读取当前“接收子地址查找表指针”的值，该指针指向当前的非激活数据块；
* 读取数据字；
* 重新使能双缓冲模式。
