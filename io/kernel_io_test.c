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

#define FILE_SIZE (1 << 30)
#define FILE_PATH "/home/snake0/test.txt"

#define BUF_SIZE (1<<10)

static int prepare(void) {

}

int test_io(void *arg) {
  struct file *fp;
  mm_segment_t fs;
  loff_t pos;

  fs = get_fs();
  set_fs(KERNEL_DS);

  fp = filp_open(FILE_PATH, O_RDWR | O_CREAT, 0644);
  if (IS_ERR(fp) & prepare()) {
    printk("prepare error. exit.\n")
    return 0;
  }



  pos = 0;
  vfs_read(fp, key, sizeof(key), &pos);
  memset(key + pos, 0, 200 - pos);
  printk("file : %s|", key);
  filp_close(fp, NULL);


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
