/*
 * kernel hello-4 -Module description
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> /* need for macros */

#define DRIVER_AUTHOR "max max@max.com"
#define DRIVER_DESC "A simple driver"

static int __init hello_4_init(void) 
{
	printk(KERN_ALERT "HELLO, 4\n");
	return 0;
}

static void __exit hello_4_exit(void)
{
	printk(KERN_ALERT "GOODBAY, 4\n");
}


module_init(hello_4_init);
module_exit(hello_4_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

MODULE_SUPPORTED_DEVICE("testdevice");
