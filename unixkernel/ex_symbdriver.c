#include <linux/module.h>     /* Необходим для любого модуля ядра */
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/cdev.h>

static char *kbuf;
static dev_t first;
static unsigned int count = 1;
static int my_major = 700,  my_minor = 0;
static struct cdev *my_cdev; 

#define MYDEV_NAME "mychrdev"
#define KBUF_SIZE size_t ((10) * PAGE_SIZE)

static int __init init_driver(void)
{
        printk(KERN_ALERT "Hello Kernek\n");

        kbuf = kmalloc(KBUF_SIZE,  GFP_KERNEL);
        first = MKDEV(my_major, my_minor);
        register_driver_region(first, count, MYDEV_NAME);

        my_cdev = cdev_alloc();

        cdev_init(my_cdev, &mycdev_fops);
        cdev_add(my_major, first, count);
        return 0;
}

static void __exit cleanup_driver(void)
{
        printk(KERN_ALERT "Goodbye world 1.\n");
}

module_init(init_driver);
module_exit(cleanup_driver);

MODULE_LICENSE("GPL");
