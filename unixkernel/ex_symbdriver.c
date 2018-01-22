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
#define KBUF_SIZE (size_t)((10) * PAGE_SIZE)



static int ex_symbdriver_open(struct inode *inode, struct file *file)
{
		static int counter = 0;
		char *kbuf = kmalloc(KBUF_SIZE, GFP_KERNEL);
		file->private_data = kbuf;

		printk(KERN_INFO "Opening device %s: \n\n", MYDEV_NAME);
		counter++;
		printk(KERN_INFO "Counter: %d\n", counter);
		printk(KERN_INFO "Module refcounter: %d\n", module_refcount(THIS_MODULE));

		return 0;
}

static int ex_symbdriver_release(struct inode *inode, struct file *file)
{
		printk(KERN_INFO "Release device %s: \n\n", MYDEV_NAME);
		char *kbuf = file->private_data;

		printk(KERN_INFO "Free buffer");
		if (kbuf) kfree(kbuf);
		kbuf = NULL;
		file->private_data = NULL;

		return 0;
}

static ssize_t ex_symbdriver_read(struct file *file, char __user *buf, 
	size_t lbuf, loff_t *ppos)
{
		char *kbuf = file->private_data;

		int nbytes = lbuf - copy_to_user(buf, kbuf + *ppos, lbuf);
		*ppos += nbytes;

		printk(KERN_INFO "Read device %s: nbytes = %d, ppos = %d; \n\n", MYDEV_NAME, nbytes, (int)*ppos);
		return nbytes;
}

static ssize_t ex_symbdriver_write(struct file *file, const char __user *buf, 
	size_t lbuf, loff_t *ppos)
{
		char *kbuf = file->private_data;
		
		int nbytes = lbuf - copy_from_user(kbuf + *ppos, buf, lbuf);
		*ppos += nbytes;

		printk(KERN_INFO "Write device %s: nbytes = %d, ppos = %d; \n\n", MYDEV_NAME, nbytes, (int)*ppos);
		return nbytes;
}

static const struct file_operations mycdev_fops ={
	.owner = THIS_MODULE,
	.read = ex_symbdriver_read,
	.write = ex_symbdriver_write,
	.open = ex_symbdriver_open,
	.release = ex_symbdriver_release
};

static int __init init_ex_symbdriver(void)
{
        printk(KERN_ALERT "Hello Kernel\n");

        kbuf = kmalloc(KBUF_SIZE,  GFP_KERNEL);
        first = MKDEV(my_major, my_minor);
        register_chrdev_region(first, count, MYDEV_NAME);

        my_cdev = cdev_alloc();

        cdev_init(my_cdev, &mycdev_fops);
        cdev_add(my_cdev, first, count);
        
        return 0;
}
static void __exit cleanup_ex_symbdriver(void)
{		
        printk(KERN_ALERT "Goodbye world 1.\n");

        if (my_cdev) cdev_del(my_cdev);
        unregister_chrdev_region(first, count);
}

module_init(init_ex_symbdriver);
module_exit(cleanup_ex_symbdriver);

MODULE_LICENSE("GPL");
