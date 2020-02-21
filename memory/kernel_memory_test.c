#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include "../utils.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert W.Oliver II");
MODULE_DESCRIPTION("Disable IRQ");
MODULE_VERSION("0.01");

static struct task_struct *tss = NULL;
static rand_t rand;
#define 

int test_memory(void *arg)
{
  rand_init(&rand);
  
  return 0;
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
