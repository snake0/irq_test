#include "../utils.h"
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert W.Oliver II");
MODULE_DESCRIPTION("Disable IRQ");
MODULE_VERSION("0.01");

static struct task_struct *tss = NULL;
static rand_t rand_;

#define TOTAL_SIZE (1 << 20)
// #define BLOCK_SIZE (1 << 10)
#define BLOCK_NUM (TOTAL_SIZE / BLOCK_SIZE)
#define OPER_COUNT ((BLOCK_NUM / sizeof(size_t)) << 20)

#define WRITE_DATA 3
#define IF_WRITE 1

static size_t *buffer;

static inline void SIZE_T_STORE(size_t *pos, size_t data)
{
  asm volatile("movq %1, (%0);"
               :
               : "r"(pos),
                 "r"(data)
               : "memory");
}

static inline size_t SIZE_T_LOAD(size_t *pos)
{
  size_t res;
  asm volatile("movq (%1), %0"
               : "=r"(res)
               : "r"(pos)
               : "memory");
  return res;
}

int test_memory(void *arg)
{
  int i, j;
  uint64_t begin, end;

  size_t *pos;  // r/w position
  uint64_t res; // read op result

  size_t *offset; // start position for every operation

  rand_init(&rand_);

  buffer = (size_t *)vmalloc(TOTAL_SIZE);
  if (buffer == NULL)
  {
    printk("vmalloc failed.\n");
    return 0;
  }

  if (DISABLE_IRQ)
    local_irq_disable();

  if (IF_WRITE)
  {
    // write
    for (i = 0; i < 10; ++i)
    {
      offset = (size_t *)((void *)buffer + BLOCK_SIZE * (rand_gen(&rand_) % BLOCK_NUM));
      begin = rdtscp();
      for (j = 0; j < OPER_COUNT; ++j)
      {
        pos = (size_t *)((void *)offset + (rand_gen(&rand_) % BLOCK_SIZE));
        SIZE_T_STORE(pos, j);
      }
      end = rdtscp();

      printk("DISABLE_IRQ=%s BIND_CPU=%s WRITE=%d %llu", tf[DISABLE_IRQ],
             tf[BIND_CPU], 1, end - begin);
    }
  }
  else
  {

    // read
    for (i = 0; i < 10; ++i)
    {
      offset = (size_t *)((void *)buffer + BLOCK_SIZE * (rand_gen(&rand_) % BLOCK_NUM));
      begin = rdtscp();
      for (j = 0; j < OPER_COUNT; ++j)
      {
        pos = (size_t *)((void *)offset + (rand_gen(&rand_) % BLOCK_SIZE));
        res += SIZE_T_LOAD(pos);
      }
      end = rdtscp();

      printk("DISABLE_IRQ=%s BIND_CPU=%s READ=%d %llu", tf[DISABLE_IRQ],
             tf[BIND_CPU], 1, end - begin);
    }
  }
  if (DISABLE_IRQ)
    local_irq_enable();

  vfree(buffer);
  return (int)res;
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
    test_memory(0);
  }

  return 0;
}

static void __exit irq_switch_exit(void) {}

module_init(irq_switch_init);
module_exit(irq_switch_exit);
