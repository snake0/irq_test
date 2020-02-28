# File I/O 测试报告
## 一、测试环境
- OS: Ubuntu 18.04.4 LTS
- Kernel Version: 5.3.13
- Memory: 7.2G
- Processor: Intel Core i7-6500U CPU @ 2.50GHz x 4
- Threads per core: 2
- Cores per socket: 2
- Sockets: 1
- Virtualization: VT-x
- L1d cache: 32k
- L1i cache: 32k
- L2 cache: 256k
- L3 cache: 4096k
- qemu version: 2.11.1
- gcc version: 7.4.0
- qemu 启动命令:
```bash
sudo qemu-system-x86_64 -drive file=ubuntu-server.img,if=virtio -cpu host -smp 4 -m 4096 -enable-kvm
```

## 二、测试概述
* 本次实验主要测试了在不同环境下 **file I/O 密集型 APP** 性能的变化。我们分别在物理机和虚拟机的内核态与用户态进行了**磁盘文件**读写测试。
* 读操作为从文件开头读取，写操作为从文件开头写入(覆盖)。每次读取、写入的大小为 `4k`， 即一个内存页的大小。
* 由于没有参数能指定读操作必须直接从磁盘读取，为了公平起见，不用 `O_SYNC` 标志打开文件， 即写操作不将数据写回磁盘。
## 三、测试结果
### 1. **测试结果说明：**

* 本次测试每组数据的波动较大，`(标准差/平均值)*100%` 在物理机上保持 `1%` ~`10%` 左右， 在qemu环境中最高 `23%`， 最低 `1.98%`
* 本次测试没有 `NO_IRQ` 选项， 因为磁盘的读写依赖于中断，关闭中断会使 APP 卡死 

### 2. 物理机内核态测试 (CPU cycles)
| DISABLE_IRQ/BIND_CPU | false/false | false/true |
| :-: | :-: | :-: |
| READ | 7010696 | 8107065 |
| WRITE | 23594149 | 24710759 |

### 3. 物理机用户态测试 (CPU cycles)
| BIND_CPU | BIND CPU 3 | BIND CPU 0 | NO BIND |
| :-: | :-: | :-: | :-: |
| READ | 22085129.6 | 21950394.2 | 21326624.6 |
| WRITE | 41937019.4 | 41857672.2 | 40444060.8 |

### 4. 虚拟机内核态测试 (CPU cycles)
| DISABLE_IRQ/BIND_CPU | false/false | false/true |
| :-: | :-: | :-: |
| READ | 7966626 | 8451120.8 |
| WRITE | 25567929 | 26936275.6 |

### 5. 虚拟机用户态测试 (CPU cycles)
| BIND_CPU | BIND CPU 3 | BIND CPU 0 | NO BIND |
| :-: | :-: | :-: | :-: |
| READ | 25472281.6 | 25027264.2 | 23469545.4 |
| WRITE | 49939502.8 | 45046073 | 44842156 |

## 四、测试结果分析
### 1. Kernel Overhead
以 **DISABLE_IRQ/BIND_CPU = false/false** 为基准, 即**硬件最快速度**，线速（line speed)，分析CPU迁移(CPU migration)、中断(interrupt)等对内核线程 `file I/O` 的影响

| DISABLE_IRQ/BIND_CPU | false/false | false/true |
| :-: | :-: | :-: |
| READ | 0% | 15.63912144% ↓ |
| WRITE | 0% | 4.732570774% ↓ |

### 2. OS Overhead
以 **DISABLE_IRQ/BIND_CPU = false/false** 为基准, 即**硬件最快速度**，线速（line speed)，分析系统调用(syscall)，中断(interrupt)等用户空间开销对用户空间APP的影响

| BIND_CPU | BIND CPU 3 | BIND CPU 0 | NO BIND |
| :-: | :-: | :-: | :-: |
| READ | 215.0221267% ↓ | 213.100262% ↓ | 204.202817% ↓ |
| WRITE | 77.74330153% ↓ | 77.4070012% ↓ | 71.41563699% ↓ |

### 3. OS & Hypervisor Overhead
以 **DISABLE_IRQ/BIND_CPU = false/false** 为基准, 即**硬件最快速度**，线速（line speed)，分析虚拟机中 **`Hypervisor`** 和 **`客户机OS`** 开销

### 3.1 Hypervisor + Kernel Overhead

| DISABLE_IRQ/BIND_CPU | false/false | false/true |
| :-: | :-: | :-: |
| READ | 13.63589487% ↓ | 20.54672515% ↓ |
| WRITE | 8.365548594% ↓ | 14.16506525% ↓ |

### 3.2 Hypervisor + Guest OS Overhead

| BIND_CPU | BIND CPU 3 | BIND CPU 0 | NO BIND |
| :-: | :-: | :-: | :-: |
| READ | 263.336438% ↓ | 256.9887131% ↓ | 234.7694236% ↓ |
| WRITE | 111.6605384% ↓ | 90.92052441% ↓ | 90.05625505% ↓ |

### 4. READ/WRITE Ratio
和 memory 性能测试不同，我们分析了磁盘读写操作的性能比例。由于数据量大，未在报告中列出。

见 https://docs.qq.com/sheet/DRk1kU0twT2xadXVT?tab=BB08J2&c=Q36H0S0 第 `34 & 35` 行

## 五、结论
根据第三节的 `CPU cycles` 数据，以及第四节得出的性能变化百分比，我们有如下结论：

1. 相同环境下，写磁盘花费的时钟周期数 `(CPU cycles)` 是读的大约 `2.55` 倍 (见数据表格)
2. 相同环境下，写磁盘相较于线速的开销 是读的大约 `2.56` 倍 (见数据表格)
3. `OS` 和 `Hypervisor` 对读的影响大于对写的影响 (对比第四节中 `READ/WRITE` 行的百分比)
4. Kernel/Hypervisor 对于读写性能的影响小于 OS 对于读写性能的影响 (对比第四节中 `1 & 2` 节 或 `3.1 & 3.2` 节)
5. 无论是用户态还是内核态，无论是物理机还是虚拟机，绑定 `CPU` 核对 `I/O` 密集型 `APP` 都会降低性能
6. 关闭中断会使得 `I/O` 操作阻塞

## 六、代码
[**测试代码 Github Link**](https://github.com/snake0/irq_test/tree/master/io)

## 七、详细测试数据

[**全部测试数据**](https://docs.qq.com/sheet/DRk1kU0twT2xadXVT?tab=BB08J2&c=B31A0A0)

