#include <linux/semaphore.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

static int __init down_interruptible_init(void);
static void __exit down_interruptible_exit(void);
struct semaphore sema;

static int polling_isr_state_routine(void *ctxt)
{
    for (;;)
    {
        mdelay(1000);
        printk("in %s %d up!\n", __func__, __LINE__);
        up(&sema);
    }
    return 0;
}

static int thread_routine(void *ctxt)
{
    for (;;)
    {
        int down;

        printk("in %s %d down ... \n", __func__, __LINE__);
        down = down_interruptible(&sema);
        if (down && down != -EINTR)
            return down;
        printk("in %s %d wakeup ... \n", __func__, __LINE__);
    }
    return 0;
}

int __init down_interruptible_init(void)
{
    int ret;
    struct task_struct *task;
    sema_init(&sema, 5);

    printk("after sema_init, sema.count: %d\n", sema.count);
    ret = down_interruptible(&sema);

    printk("first down_interruptible, ret = %d\n", ret);
    printk("first down_interruptible, sema.count: %d\n", sema.count);

    sema_init(&sema, 0);

    task = kthread_run(thread_routine, (void *)0, "thread_routine");
    task = kthread_run(polling_isr_state_routine, (void *)0, "polling_isr_state_routine");
    ret = down_interruptible(&sema);

    printk("second down_interruptible, ret = %d\n", ret);
    printk("second down_interruptible, sema.count: %d\n", sema.count);
    return 0;
}

void __exit down_interruptible_exit(void)
{
    printk("exit! \n");
}

module_init(down_interruptible_init);
module_exit(down_interruptible_exit);
