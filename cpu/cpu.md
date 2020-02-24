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
### **0. 取消disable irq & bind cpu测试的说明：**

* 关中断并且绑核测得的数据不稳定，尚未查明原因。但关中断必然绑核，即scheduler不会打断线程进行CPU迁移，我们**取消**了这个配置的测试。

* 其他配置的测试数据较为**稳定**，波动在 **`0.2%` 到 `0.3%`** 之间(标准差/平均值，见详细测试数据)

### 1. 物理机内核态测试
| NO BIND, WITH IRQ | BIND, WITH IRQ | NO BIND, WITHOUT IRQ |
| :-: | :-: | :-: | 
| <font color=red>1508746974</font> | <font color=#FFA500>1434743820</font> | <font color=green>1345530254</font> |

### 2. 物理机用户态测试
| IRQ Enable CPU 3 | IRQ Enable CPU 0 | NO BIND, WITH IRQ |
| :-: | :-: | :-: |
| <font color=red>1492132357</font> | <font color=#FFA500>1491205220</font> | <font color=green>1471545419</font> |
### 3. 虚拟机内核态测试
| NO BIND, WITH IRQ | BIND, WITH IRQ | NO BIND, WITHOUT IRQ|
| :-: | :-: | :-: |
| <font color=red>1535383348</font> | <font color=#FFA500>1462007863</font> | <font color=green>1359371000</font> |

### 4. 虚拟机用户态测试
| IRQ Enable CPU 3 | IRQ Enable CPU 0 | NO BIND, WITH IRQ |
| :-: | :-: | :-: |
| <font color=red>1500371071</font> | <font color=#FFA500>1511851971</font> | <font color=green>1502607961</font> |
## 四、测试结果分析
### 1. **Kernel Overhead**
以 `disable irq,no bind cpu` 为基准(**硬件最快速度，即线速(line speed)**)，分析CPU迁移(CPU migration)、中断(interrupt)等对内核线程的影响
| NO BIND, WITH IRQ | BIND, WITH IRQ | NO BIND, WITHOUT IRQ |
| :-: | :-: | :-: |
|<font color=red>**12.13% ↓**</font>	| <font color=#FFA500>**6.63% ↓**</font>| <font color=green>**0%**</font> |


### 2. **OS Overhead**
以`disable irq,no bind cpu` 为基准(**硬件最快速度，即线速(line speed)**)，分析系统调用(syscall)，中断(interrupt)等用户空间开销对用户空间APP的影响

| IRQ Enable CPU 3 | IRQ Enable CPU 0 | NO BIND, WITH IRQ |
| :-: | :-: | :-: |
| <font color=red>**10.89548918% ↓**</font> | <font color=red>**10.82658422% ↓**</font> | <font color=#FFA500>**9.365464998% ↓**</font> |


### 3. **OS & Hypervisor Overhead**
以`disable irq,no bind cpu` 为基准(**硬件最快速度，即线速(line speed)**)，分析虚拟机中 **`Hypervisor`** 和 **`客户机OS`** 开销

**3.1 Hypervisor + Kernel**
		
| NO BIND, WITH IRQ | BIND, WITH IRQ | NO BIND, WITHOUT IRQ |
| :-: | :-: | :-: |
| <font color=red>**14.1797% ↓**</font> | <font color=#FFA500>**8.67252% ↓**</font> | <font color=#FFD700>**1.02813% ↓**</font> |

**3.2 Hypervisor + Guest OS**

| IRQ Enable CPU 3 | IRQ Enable CPU 0 | NO BIND, WITH IRQ |
| :-: | :-: | :-: |
| <font color=red>**12.4454% ↓**</font> | <font color=red>**12.4359% ↓**</font> | <font color=red>**11.2556% ↓**</font> |



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

