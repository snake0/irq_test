# 内存测试报告
## 一、测试环境
- OS: Ubuntu 18.04.4 LTS
- Kernel Version: 5.3.13
- Memory: 7.2G
- Processor: Intel Core i7-6500U CPU @ 2.50GHz x4
- Threads per core: 2
- Cores per socket: 2
- Sockets: 1
- Virtualization: VT-x
- L1d cache: 32k
- L1i cache: 32k
- L2 cache: 256k
- L3 cache: 4096k
## 二、测试概述
本次实验主要测试了在不同环境下，内存的读写性能。同CPU测试相同，我们分别在物理机和虚拟机的内核态与用户态进行了内存读写测试。读写方式为随机读写。测试代码见Reference。
## 三、测试结果
### 1. 物理机内核态测试
| | IRQ Enable Bind CPU | IRQ Disable Bind CPU | IRQ Enable All CPU | IRQ Disable All CPU 
| :-: | :-: | :-: | :-: | :-: |
| Read | 851263892.2 | 847449202.8 | 842823725.5 | 846318097 |
| Write | 847730371 | 848662684 | 846866645.2 | 849767175.9 |
### 2. 物理机用户态测试
| | IRQ Enable CPU 3 | IRQ Enable CPU 0 | IRQ Enable All CPU |
| :-: | :-: | :-: | :-: | 
| Read | 849303470.8 | 852506713.4 | 849062971 | 
| Write | 865753500.8 | 849012273.8 | 850361421.6 |
### 3. 虚拟机内核态测试
| | IRQ Enable Bind CPU | IRQ Disable Bind CPU | IRQ Enable All CPU | IRQ Disable All CPU 
| :-: | :-: | :-: | :-: | :-: |
| Read | 862421233.8 | 862793262 | 862098841.2 | 858472721 |
| Write | 865579691 | 865791505.2 | 869943889.4 | 867212392.4 |
### 4. 虚拟机用户态测试
| | IRQ Enable CPU 3 | IRQ Enable CPU 0 | IRQ Enable All CPU |
| :-: | :-: | :-: | :-: | 
| Read | 867471471.8 | 864124327.2 | 858489399.2 | 
| Write | 872842101 | 867127260.4 | 865859490.4 |
## 四、测试结果分析
### 1. 读写性能分析
分别计算不同实验条件下，写操作相比较于读操作的性能变化如下
| PM Kernel Space | IRQ Enable Bind CPU | IRQ Disable Bind CPU | IRQ Enable All CPU | IRQ Disable All CPU 
| :-: | :-: | :-: | :-: | :-: |
| Performance Change | 0.42% ↑ | 0.14% ↓ | 0.48% ↓ | 0.41% ↓ |

| PM User Space | IRQ Enable CPU 3 | IRQ Enable CPU 0 | IRQ Enable All CPU |
| :-: | :-: | :-: | :-: | 
| Performance Change | 1.94% ↓ | 0.41% ↑ | 0.15% ↓ | 

| VM Kernel Space | IRQ Enable Bind CPU | IRQ Disable Bind CPU | IRQ Enable All CPU | IRQ Disable All CPU 
| :-: | :-: | :-: | :-: | :-: |
| Performance Change | 0.35% ↓ | 0.35% ↓ | 0.91% ↓ | 1.12% ↓ |

| VM User Space | IRQ Enable CPU 3 | IRQ Enable CPU 0 | IRQ Enable All CPU |
| :-: | :-: | :-: | :-: | 
| Performance Change | 0.62% ↓ | 0.35% ↓ | 0.86% ↓ | 
从上表可以看出，相比较于读操作，写操作往往耗时更久，但是程序性能下降不多。
### 2. OS Overhead
分别计算虚拟环境与物理环境下，用户态相比较于内核态读写操作性能变化
| PM | IRQ Enable Bind CPU | IRQ Enable All CPU |  
| :-: | :-: | :-: |
| Read Performance Change | 0.04% ↑ | 0.74% ↓ | 
| Write Performance Change | 1.39% ↓ | 0.41% ↓ | 

| VM | IRQ Enable Bind CPU | IRQ Enable All CPU |  
| :-: | :-: | :-: |
| Read Performance Change | 0.39% ↓ | 0.42% ↑ | 
| Write Performance Change | 0.51% ↓ | 0.47% ↑ | 
### 3. OS & Hypervisor Overhead
计算OS，Hypervisor双重介入时，读写操作性能变化。
|  | IRQ Enable Bind CPU | IRQ Enable All CPU |  
| :-: | :-: | :-: |
| Read Performance Change | 1.71% ↓  | 1.86% ↓ | 
| Write Performance Change | 2.63% ↓ | 2.24% ↓ |
### 4. Interrupt Overhead
分别计算虚拟环境与物理环境下，禁用中断相比较于开启中断读写操作性能变化。
| PM Kernel Space | Bind CPU | All CPU |  
| :-: | :-: | :-: |
| Read Performance Change | 0.45% ↓  | 0.41% ↑ | 
| Write Performance Change | 0.11% ↑ ↓ | 0.34% ↑ |

| VM Kernel Space | Bind CPU | All CPU |  
| :-: | :-: | :-: |
| Read Performance Change | 0.04% ↑  | 0.04% ↓ | 
| Write Performance Change | 0.04% ↑ | 0.03% ↓ |
## 五、结论
1. 写操作相对于读操作，耗时更久，但就整个程序运行时间来看，影响不大。
2. 当程序运行在用户态时，相比较于程序运行在内核态，性能十分接近，波动幅度小于1%。因此OS介入对内存操作影响不大。
3. 当OS于Hypervisor双重介入时，程序性能有所下降，读写操作均是如此，但是下降幅度并不明显，约在1%~2%左右。
4. 是否开启中断对程序性能影响不大，关闭中断后，程序性能波动在0.5%以内。
## 六、代码
[内核态测试代码](https://github.com/snake0/irq_test/blob/mem_test_dev/memory/kernel_memory_test.c)
[用户态测试代码](https://github.com/snake0/irq_test/blob/mem_test_dev/memory/user_memory_test.c)



