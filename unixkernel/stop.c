/*
 * Example of many sources module
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
void __exit stop_6_module(void)
{
    printk("<1>Stop kernel module");
}

module_exit(stop_6_module);
