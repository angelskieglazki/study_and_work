/*
 * kernel hello-2 - using module_init() and module_exit() macros
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> /* need for macros */

static int __init hello_2_init(void) 
{
	printk(KERN_ALERT "HELLO, MAX\n");
	return 0;
}

static void __exit hello_2_exit(void)
{
	printk(KERN_ALERT "GOODBAY, MAX\n");
}

/* macros */
module_init(hello_2_init);
module_exit(hello_2_exit);
