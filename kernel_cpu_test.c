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

unsigned long cal_sqrt(unsigned long x)
{
	unsigned long op, res, one;
	op = x;
	res = 0;
	one = 1UL << (BITS_PER_LONG - 2);
	while (one > op)
		one >>= 2;

	while (one != 0) {
		if (op >= res + one) {
			op = op - (res + one);
			res = res +  2 * one;
		}
		res /= 2;
		one /= 4;
	}
	return res;
}

static int MAX_PRIME = 1000000;

#define CPUID 3
#define DISABLE_IRQ 1
#define BIND_CPU 1

uint64_t rdtscp(void)
{
  uint32_t lo, hi;
  __asm__ __volatile__(
      "rdtscp"
      : "=a"(lo), "=d"(hi));
  return (uint64_t)hi << 32 | lo;
}
static struct task_struct *tss = NULL;

int test_prime(void *arg)
{
  unsigned long long c;
  unsigned long long l, t;
  unsigned long long n = 0;
  uint64_t begin, end;
  int i;

  if (DISABLE_IRQ)
    local_irq_disable();

  for (i = 0; i < 10; ++i)
  {
    n = 0;
    begin = rdtscp();
    for (c = 3; c < MAX_PRIME; c++)
    {
      t = cal_sqrt(c);
      for (l = 2; l <= t; l++)
        if (c % l == 0)
          break;
      if (l > t)
        n++;
    }

    end = rdtscp();
    printk("DISABLE_IRQ=%s BIND_CPU=%s MAX_PRIME=%d %llu", tf[DISABLE_IRQ], tf[BIND_CPU], MAX_PRIME, end - begin);
  }

  if (DISABLE_IRQ)
    local_irq_enable();

  return 0;
}

static int __init irq_switch_init(void)
{
  if (BIND_CPU)
  {
    tss = kthread_create(test_prime, NULL, "Test_prime_task");
    if (!IS_ERR(tss))
    {
      kthread_bind(tss, CPUID);
      wake_up_process(tss);
    }
  }
  else
  {
    unsigned long long c;
    unsigned long long l, t;
    unsigned long long n = 0;
    uint64_t begin, end;
    int i;

    if (DISABLE_IRQ)
      local_irq_disable();

    for (i = 0; i < 10; ++i)
    {
      n = 0;
      begin = rdtscp();
      for (c = 3; c < MAX_PRIME; c++)
      {
        t = cal_sqrt(c);
        for (l = 2; l <= t; l++)
          if (c % l == 0)
            break;
        if (l > t)
          n++;
      }

      end = rdtscp();
      printk("DISABLE_IRQ=%s BIND_CPU=%s MAX_PRIME=%d %llu", tf[DISABLE_IRQ], tf[BIND_CPU], MAX_PRIME, end - begin);
    }

    if (DISABLE_IRQ)
      local_irq_enable();
  }

  return 0;
}

static void __exit irq_switch_exit(void)
{
}

module_init(irq_switch_init);
module_exit(irq_switch_exit);
