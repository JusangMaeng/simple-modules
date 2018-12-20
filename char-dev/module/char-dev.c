#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/fs.h>
#include <asm/errno.h>

/* Prototypes */
static long char_dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static int char_dev_open(struct inode *inode, struct file *file);
static int char_dev_release(struct inode *inode, struct file *file);

#define DEVICE_NAME "char-dev" /* Dev name about '/proc/devices' */

/* Global variables */
static int char_dev_major; 				/* major number at device */
static int char_dev_device_open = 0;	/* to prevent multiple access to the device */

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = char_dev_ioctl,
	.open = char_dev_open,
	.release = char_dev_release
};

static long char_dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	printk(KERN_INFO "device_ioctl \n");
	
	return 0;
}

static int char_dev_open(struct inode *inode, struct file *file)
{
	if(char_dev_device_open) return -EBUSY;
	
	char_dev_device_open++;
	
	printk(KERN_INFO "device_open \n");
	
	return 0;
}
static int char_dev_release(struct inode *inode, struct file *file)
{
	char_dev_device_open--;
	
	printk(KERN_INFO "device_release \n");
	
	return 0;
}
static int __init char_dev_init(void)
{
	printk(KERN_INFO "module init \n");
	
	char_dev_major = register_chrdev(0, DEVICE_NAME, &fops);
	if (char_dev_major < 0) {
		printk ("Registering the character device failed with %d\n", char_dev_major);
		return -EAGAIN;
	}
	
	return 0;
}

static void __exit char_dev_exit(void)
{
	unregister_chrdev(char_dev_major, DEVICE_NAME);
	
	printk(KERN_INFO "module exit \n");
}

module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Maeng"); 
MODULE_DESCRIPTION("A character device module init");
MODULE_VERSION("0.1"); 

