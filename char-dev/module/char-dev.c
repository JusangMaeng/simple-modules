#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/fs.h>
#include <asm/errno.h>

/* Prototypes */
static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);

#define DEVICE_NAME "char-dev" /* Dev name about '/proc/devices' */

/* Global variables */
static int char_dev_major; 				/* major number at device */
static int char_dev_device_open = 0;	/* to prevent multiple access to the device */

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = device_ioctl,
	.open = device_open,
	.release = device_release
};

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	printk(KERN_INFO "device_ioctl \n");
	
	return 0;
}

static int device_open(struct inode *inode, struct file *file)
{
	if(char_dev_device_open) return -EBUSY;
	
	char_dev_device_open++;
	
	printk(KERN_INFO "device_open \n");
	
	return 0;
}
static int device_release(struct inode *inode, struct file *file)
{
	char_dev_device_open--;
	
	printk(KERN_INFO "device_release \n");
	
	return 0;
}
static int __init simple_start(void)
{
	printk(KERN_INFO "module init \n");
	
	char_dev_major = register_chrdev(0, DEVICE_NAME, &fops);
	if (char_dev_major < 0) {
		printk ("Registering the character device failed with %d\n", char_dev_major);
		return -EAGAIN;
	}
	
	return 0;
}

static void __exit simple_end(void)
{
	unregister_chrdev(char_dev_major, DEVICE_NAME);
	
	printk(KERN_INFO "module exit \n");
}

module_init(simple_start);
module_exit(simple_end);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Maeng"); 
MODULE_DESCRIPTION("A character device module init");
MODULE_VERSION("0.1"); 

