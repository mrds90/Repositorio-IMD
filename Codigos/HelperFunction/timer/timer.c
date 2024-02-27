#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>

static struct timer_list my_timer;

void timer_callback(struct timer_list* timer)
{
	pr_info("%s called (%ld).\n", __FUNCTION__, jiffies);
}

static int __init timer_init(void)
{
	int retval;

    pr_info("Timer module loaded\n");
	timer_setup(&my_timer, timer_callback, 0);
	pr_info("Setup timer to fire in 300ms (%ld)\n", jiffies);
	retval = mod_timer( &my_timer, jiffies + msecs_to_jiffies(3000));

    if (retval)
    {
	    pr_info("Timer firing failed\n");
    }

    return 0;
}

static __exit void timer_exit(void)
{
	int retval;

	retval = del_timer(&my_timer);

	/* Is timer still active (1) or no (0) */
	if (retval)
    {
		pr_info("The timer is still in use...\n");
    }

	pr_info("Timer module unloaded\n");
}


module_init(timer_init);
module_exit(timer_exit);
MODULE_AUTHOR("Hanes Nahuel Sciarrone <hsciarrone@gmail.com>");
MODULE_DESCRIPTION("Standard timer example");
MODULE_LICENSE("GPL");
