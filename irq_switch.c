#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert W.Oliver II");
MODULE_DESCRIPTION("Disable IRQ");
MODULE_VERSION("0.01");

#define MAX_PRIME 100000
#define CPUID 4
#define WITHOUT 1


// uint64_t rdtsc(){
//   uint32_t lo,hi;
//   __asm__ __volatile__
//   (
//     "rdtsc":"=a"(lo),"=d"(hi)
//   );
//   return (uint64_t)hi << 32 | lo;
// }
static struct task_struct *tss = NULL;

const static int id = 1;


int test_prime(void *arg)
{
  
}

// static void disable(void *info)
// {
//   int cpu = get_cpu();
//   // if (cpu == id) {
//   printk("disable irq on target cpu: %d\n", cpu);
//   local_irq_disable();
//   // }
//   put_cpu();
// }

// static void enable(void *info)
// {
//   int cpu = get_cpu();
//   // if (cpu == id) {
//   printk("enable irq on target cpu: %d\n", cpu);
//   local_irq_enable();
//   // }
//   put_cpu();
// }

static int __init irq_switch_init(void)
{
  // on_each_cpu(disable, NULL, 1);

  // tss = kthread_create(test_prime, NULL, "Test_prime_task");
  // if (!IS_ERR(tss))
  // {
  //   kthread_bind(tss, CPUID);
  //   wake_up_process(tss);
  // }
  unsigned long long c;
  unsigned long long l, t;
  unsigned long long n = 0;

  uint64_t begin, end;
  int i;
  if (WITHOUT)
    local_irq_disable();

  for (i = 0; i < 10; ++i)
  {
    n = 0;
    begin = rdtsc();
    for (c = 3; c < MAX_PRIME; c++)
    {
      t = int_sqrt(c);
      for (l = 2; l <= t; l++)
        if (c % l == 0)
          break;
      if (l > t)
        n++;
    }

    end = rdtsc();

    if (WITHOUT)
      printk("kernel WITHOUT irq %llu MAX_PRIME=%d", end - begin, MAX_PRIME);
    else
      printk("kernel WITH irq %llu MAX_PRIME=%d", end - begin, MAX_PRIME);
  }
  if (WITHOUT)
    local_irq_enable();

  return 0;

  // on_each_cpu(disable, NULL, 1);
  // return 0;
}

static void __exit irq_switch_exit(void)
{
  // on_each_cpu(enable, NULL, 1);
}

module_init(irq_switch_init);
module_exit(irq_switch_exit);
