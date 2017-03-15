/*
 * Example of many sources module
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
int __init start_init_module(void)
{
    printk("Hello, world - this is the kernel start\n");
    return 0;
}

module_init(start_init_module);
