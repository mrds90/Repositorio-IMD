// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>

static int __init hello_init(void)
{
	pr_info("Hola Mundo. Estás utilizando Linux %s\n", init_uts_ns.name.release);
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("Adiós Mundo.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marcos Dominguez");
MODULE_DESCRIPTION("Trabajo Practico de MSE 1");
