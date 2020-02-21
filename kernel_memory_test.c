#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert W.Oliver II");
MODULE_DESCRIPTION("Disable IRQ");
MODULE_VERSION("0.01");

#define BITS_PER_LONG 64

static const char *tf[2] = {"false", "true"};

#define CPUID 3
#define DISABLE_IRQ 1
#define BIND_CPU 0

uint64_t rdtscp(void)
{
  uint32_t lo, hi;
  __asm__ __volatile__(
      "rdtscp"
      : "=a"(lo), "=d"(hi));
  return (uint64_t)hi << 32 | lo;
}
static struct task_struct *tss = NULL;

int test_memory(void *arg)
{
  
}

static int __init irq_switch_init(void)
{
  if (BIND_CPU)
  {
    tss = kthread_create(test_memory, NULL, "Test_memory_task");
    if (!IS_ERR(tss))
    {
      kthread_bind(tss, CPUID);
      wake_up_process(tss);
    }
  }
  else
  {
    
  }

  return 0;
}

static void __exit irq_switch_exit(void)
{
}

module_init(irq_switch_init);
module_exit(irq_switch_exit);
