# CPU测试报告
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
* 本次实验主要测试了在不同环境下 **纯CPU运算型APP** 性能的变化。我们分别在物理机和虚拟机的内核态与用户态进行了素数筛查测试。筛查范围为 `1` 到 `1000000`。

* 经过objdump反汇编，我们的用户态、内核态测试代码的**汇编码**基本相同，保证了公平性。
## 三、测试结果
**取消disable irq & bind cpu测试的说明：**

* 关中断并且绑核测得的数据不稳定（kthread_bind函数与local_irq_disable函数同时使用时）。且通过htop查看程序运行情况时，发现程序并没有运行在kthread_bind指定的核上，去掉local_irq_disable，单独使用kthread_bind则无此现象。但禁用中断时，程序运行不受影响，即scheduler不会打断线程进行CPU迁移，我们取消了这个配置的测试。

* 其他配置的测试数据较为**稳定**，波动在 **`0.2%` 到 `0.3%`** 之间(标准差/平均值，见详细测试数据)

### 1. 物理机内核态测试
| NO BIND, WITH IRQ | BIND, WITH IRQ | NO BIND, WITHOUT IRQ |
| :-: | :-: | :-: | 
| 1508746974 | 1434743820 | 1345530254 |
### 2. 物理机用户态测试
| IRQ Enable CPU 3 | IRQ Enable CPU 0 | NO BIND, WITH IRQ |
| :-: | :-: | :-: |
| 1492132357 | 1491205220 | 1471545419 |
### 3. 虚拟机内核态测试
| NO BIND, WITH IRQ | BIND, WITH IRQ | NO BIND, WITHOUT IRQ|
| :-: | :-: | :-: |
| 1535383348 | 1462007863 | 1359371000 |
### 4. 虚拟机用户态测试
| IRQ Enable CPU 3 | IRQ Enable CPU 0 | NO BIND, WITH IRQ |
| :-: | :-: | :-: |
| 1500371071 | 1511851971 | 1502607961 |
## 四、测试结果分析
### 1. Kernel Overhead
以 `disable irq,no bind cpu` 为基准(**硬件最快速度，即线速(line speed)**)，分析CPU迁移(CPU migration)、中断(interrupt)等对内核线程的影响
| NO BIND, WITH IRQ | BIND, WITH IRQ | NO BIND, WITHOUT IRQ |
| :-: | :-: | :-: |
| 12.13% ↓ | 6.63% ↓ | 0% |
### 2. OS Overhead
以`disable irq,no bind cpu` 为基准(**硬件最快速度，即线速(line speed)**)，分析系统调用(syscall)，中断(interrupt)等用户空间开销对用户空间APP的影响
| IRQ Enable CPU 3 | IRQ Enable CPU 0 | NO BIND, WITH IRQ |
| :-: | :-: | :-: |
| 10.90% ↓ |10.83% ↓ | 9.37% ↓ |

### 3. OS & Hypervisor Overhead
以`disable irq,no bind cpu` 为基准(**硬件最快速度，即线速(line speed)**)，分析虚拟机中 **`Hypervisor`** 和 **`客户机OS`** 开销

**3.1 Hypervisor + Kernel**
		
| NO BIND, WITH IRQ | BIND, WITH IRQ | NO BIND, WITHOUT IRQ |
| :-: | :-: | :-: |
| 14.18% ↓ | 8.67% ↓ | 1.03% ↓ |

**3.2 Hypervisor + Guest OS**
| IRQ Enable CPU 3 | IRQ Enable CPU 0 | NO BIND, WITH IRQ |
| :-: | :-: | :-: |
| 12.45% ↓ | 12.44% ↓ | 11.26% ↓ |
## 五、结论
根据第四节得出的性能变化百分比，我们有如下结论：

1. CPU性能受中断影响较大（观察1节的**BIND, WITH IRQ**配置）
2. CPU性能受CPU迁移影响较大（观察1节的**NO BIND, WITH IRQ**配置）
3. CPU性能受Hypervisor影响不大（观察3.1节的**NO BIND, WITHOUT IRQ**配置）
4. Hypervisor性能受CPU迁移、中断影响较大（观察3.1节的**NO BIND, WITH IRQ**、**BIND, WITH IRQ**配置）
5. 内核中中断、CPU迁移对APP的影响比用户态更大（对比1、2节或3.1、3.2节数据）
6. Hypervisor对CPU性能影响不大，Kernel、OS对CPU性能影响很大

## 六、代码
[**测试代码 Github Link**](https://github.com/snake0/irq_test/tree/master/cpu)

## 七、详细测试数据

[**全部测试数据**](https://docs.qq.com/sheet/DRnlpeHNGdWt0V0Zo?tdsourcetag=s_macqq_aiomsg&tab=BB08J2&c=L17A0A0)

