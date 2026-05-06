#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/atomic.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");

static int __init cmpxchg_test_init(void) {
    atomic_t v;
    int old_val = 0x80000000;
    int new_val = 0x7ffffffe;
    int ret;

    printk(KERN_INFO "cmpxchg_test: Initializing the cmpxchg_test LKM\n");

    // 初始化原子变量 v 为 0x80000000
    atomic_set(&v, 0x80000000);

    // 尝试原子比较交换：如果 v 的值是 old_val，则将其设置为 new_val
    ret = atomic_cmpxchg(&v, old_val, new_val);

    // 检查操作是否成功
    if (ret == old_val) {
        printk(KERN_INFO "cmpxchg_test: The expected old value was correct.\n");
    } else {
        printk(KERN_INFO "cmpxchg_test: The expected old value was not correct.\n");
    }

    // 打印 v 的新值
    printk(KERN_INFO "cmpxchg_test: New value of v: %d\n", atomic_read(&v));

    return 0;
}

static void __exit cmpxchg_test_exit(void) {
    printk(KERN_INFO "cmpxchg_test: Exiting the cmpxchg_test LKM\n");
}

module_init(cmpxchg_test_init);
module_exit(cmpxchg_test_exit);
