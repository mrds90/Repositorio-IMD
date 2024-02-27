/*
Este modulo muestra la funcionalidad sleep junto con work and wait queues
El proceso que se esta marcando para sleep es en realdad insmod, dado que
es el que inserta el modulo (recordar que ejecuta la funcion init e inserta
el modulo en espacio kernel)
*/


#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/delay.h>
#include<linux/workqueue.h>

static DECLARE_WAIT_QUEUE_HEAD(wait_queue);
static int condition = 0;

/* declare a work queue*/
static struct work_struct wrk;

static void work_handler(struct work_struct *work)
{
	pr_info("Waitqueue module handler %s\n", __FUNCTION__);
	msleep(5000);
	pr_info("Wake up the sleeping module\n");
	condition = 1;
	wake_up_interruptible(&wait_queue);
}

static int __init sleep_init(void)
{
	pr_info("Wait queue example\n");

	INIT_WORK(&wrk, work_handler);
	schedule_work(&wrk);

    pr_info("Going to sleep %s\n", __FUNCTION__);
	wait_event_interruptible(wait_queue, condition != 0);
	pr_info("woken up by the work job\n");

    return 0;
}

void sleep_exit(void)
{
	pr_info("waitqueue example cleanup\n");
}

module_init(sleep_init);
module_exit(sleep_exit);
MODULE_AUTHOR("Hanes Nahuel Sciarrone  <hanessciarrone@gmail.com>");
MODULE_LICENSE("GPL");
