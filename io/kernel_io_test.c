#include "../utils.h"
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/uaccess.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert W.Oliver II");
MODULE_DESCRIPTION("Disable IRQ");
MODULE_VERSION("0.01");

static struct task_struct *tss = NULL;

#define FILE_NAME "/home/snake0/test.0"
#define FILE_SIZE (1 << 12)
#define OPER_COUNT 10000


int test_io(void *arg)
{
  int ret, i, j;
  char read_buf[FILE_SIZE];
  char write_buf[FILE_SIZE];
  uint64_t begin, end;
  mm_segment_t fs;
  loff_t pos = 0;
  struct file *fp = NULL;
  memset(write_buf, 'c', FILE_SIZE);

  fs = get_fs();
  set_fs(KERNEL_DS);

  // init file for read
  if (IS_ERR(fp = filp_open(FILE_NAME, O_WRONLY | O_CREAT, 0644)))
  {
    printk("File prepare error! exit.\n");
    return 0;
  }

  ret = vfs_write(fp, write_buf, FILE_SIZE, &pos);
  ret = vfs_write(fp, write_buf, FILE_SIZE, &pos);
  filp_close(fp, NULL);

  // test read
  for (i = 0; i < 10; ++i)
  {
    fp = filp_open(FILE_NAME, O_RDONLY, 0644);
    if (IS_ERR(fp))
    {
      printk("Open file error!\n");
      return -1;
    }

    begin = rdtscp();
    for (j = 0; j < OPER_COUNT; ++j)
    {
      pos = 0;
      ret = vfs_read(fp, read_buf, FILE_SIZE, &pos);
    }
    end = rdtscp();

    printk("[READ BIND_CPU=%d DISABLE_IRQ=%d] %llu\n", BIND_CPU, DISABLE_IRQ, end - begin);
    filp_close(fp, NULL);
  }

  // test sequential write
  for (i = 0; i < 10; ++i)
  {
    fp = filp_open(FILE_NAME, O_WRONLY | O_TRUNC, 0644);
    if (IS_ERR(fp))
    {
      printk("Open file error!\n");
      return -1;
    }

    begin = rdtscp();
    for (j = 0; j < OPER_COUNT; ++j)
    {
      pos = 0;
      ret = vfs_write(fp, write_buf, FILE_SIZE, &pos);
    }
    end = rdtscp();

    printk("[WRITE BIND_CPU=%d DISABLE_IRQ=%d] %llu\n", BIND_CPU, DISABLE_IRQ, end - begin);
    filp_close(fp, NULL);
  }

  set_fs(fs);
  return 0;
}

static int __init irq_switch_init(void)
{
  if (BIND_CPU)
  {
    tss = kthread_create(test_io, NULL, "Test_io_task");
    if (!IS_ERR(tss))
    {
      kthread_bind(tss, CPUID);
      wake_up_process(tss);
    }
  }
  else
  {
    test_io(0);
  }

  return 0;
}

static void __exit irq_switch_exit(void) {}

module_init(irq_switch_init);
module_exit(irq_switch_exit);
