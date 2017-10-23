/*
 * простой модуль ядра
 */

#include <linux/module.h>     /* Необходим для любого модуля ядра */


static int __init init_driver(void)
{
        return 0;
}

static void __exit cleanup_driver(void)
{
        printk(KERN_ALERT "Goodbye world 1.\n");
}

module_init(init_driver);
module_exit(cleanup_driver);

MODULE_LICENSE("GPL");