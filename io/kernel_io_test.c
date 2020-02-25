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

#define READ_FILE_PATH "/home/sin/test_read.txt"
#define WRITE_FILE_PATH "/home/sin/test_write.txt"
#define RW_SIZE (1 << 12)

int test_io(void *arg) {
  int ret, i, j;
  char read_buf[RW_SIZE];
  char write_buf[RW_SIZE];
  uint64_t begin, end;
  mm_segment_t fs;
  loff_t pos = 0;
  struct file *fp = NULL;
  memset(write_buf, 'c', RW_SIZE);

  fs = get_fs();
  set_fs(KERNEL_DS);
  
  // test read
  for (i = 0; i < 10; ++i)
  {
    fp = filp_open(READ_FILE_PATH, O_RDONLY | O_SYNC, 0644);
    if (IS_ERR(fp)) {
      printk("Open file error!\n");
      return -1;
    }

    begin = rdtscp();
    for (j = 0; j < 100; ++j)
    {
      ret = vfs_read(fp, read_buf, RW_SIZE, &pos);
    }
    end = rdtscp();

    printk("[READ BIND_CPU=%d DISABLE_IRQ=%d] %llu\n", BIND_CPU, DISABLE_IRQ, end - begin);
    filp_close(fp, NULL);
  } 
  
  // test sequential write
  for (i = 0; i < 10; ++i)
  {
    fp = filp_open(WRITE_FILE_PATH, O_WRONLY | O_TRUNC | O_SYNC, 0644);
    if (IS_ERR(fp)) {
      printk("Open file error!\n");
      return -1;
    }

    begin = rdtscp();
    for (j = 0; j < 100; ++j)
    {
      ret = vfs_write(fp, write_buf, RW_SIZE, &pos);
      pos += RW_SIZE;
    }
    end = rdtscp();

    printk("[WRITE BIND_CPU=%d DISABLE_IRQ=%d] %llu\n", BIND_CPU, DISABLE_IRQ, end - begin);
    filp_close(fp, NULL);
  } 

  set_fs(fs);
  return 0;
}

static int __init irq_switch_init(void) {
  if (BIND_CPU) {
    tss = kthread_create(test_io, NULL, "Test_io_task");
    if (!IS_ERR(tss)) {
      kthread_bind(tss, CPUID);
      wake_up_process(tss);
    }
  } else {
    test_io(0);
  }

  return 0;
}

static void __exit irq_switch_exit(void) {}

module_init(irq_switch_init);
module_exit(irq_switch_exit);
