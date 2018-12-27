#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/fs.h>
#include <asm/errno.h>		/* Needed for error code */
#include <linux/cdev.h>		/* Needed for using cdev */
#include <linux/device.h>	/* Needed for class create */


/* Prototypes */
static long char_dev_sysfs_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static int char_dev_sysfs_open(struct inode *inode, struct file *file);
static int char_dev_sysfs_release(struct inode *inode, struct file *file);

#define DEVICE_NAME "char-dev-sysfs" /* Dev name about '/proc/devices' */

/* Global variables */
static int char_dev_sysfs_device_open = 0;	/* to prevent multiple access to the device */
static dev_t char_dev_sysfs_dev_t;	/* Global variable for the first device number */
static struct cdev* char_dev_sysfs_cdev;
static struct class* char_dev_sysfs_class;


static struct file_operations char_dev_sysfs_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = char_dev_sysfs_ioctl,
	.open = char_dev_sysfs_open,
	.release = char_dev_sysfs_release
};

static long char_dev_sysfs_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	printk(KERN_INFO "device_ioctl \n");
	
	return 0;
}

static int char_dev_sysfs_open(struct inode *inode, struct file *file)
{
	if(char_dev_sysfs_device_open) return -EBUSY;
	
	char_dev_sysfs_device_open++;
	
	printk(KERN_INFO "device_open \n");
	
	return 0;
}
static int char_dev_sysfs_release(struct inode *inode, struct file *file)
{
	char_dev_sysfs_device_open--;
	
	printk(KERN_INFO "device_release \n");
	
	return 0;
}
static int __init char_dev_sysfs_init(void)
{
	int retval;
	printk(KERN_INFO "module init \n");

	retval = alloc_chrdev_region(&char_dev_sysfs_dev_t, 0,1, DEVICE_NAME);
	if(retval < 0) {
		printk(KERN_ERR "alloc_chrdev_region() failed!\n");
		return -EBUSY;
	}

	printk("\n Major No: %d Minor_no : %d \n", MAJOR(char_dev_sysfs_dev_t), MINOR(char_dev_sysfs_dev_t));

	// argument name is class sub-dir as '/sys/class/char-dev'
	char_dev_sysfs_class = class_create(THIS_MODULE, "char-dev");
	if(char_dev_sysfs_class == NULL) {
		unregister_chrdev_region(char_dev_sysfs_dev_t,1);
		printk(KERN_ERR "class_create fail");
		return -ENOMEM;
	}

	if(device_create(char_dev_sysfs_class, NULL, char_dev_sysfs_dev_t, NULL, DEVICE_NAME) == NULL)
	{
		class_destroy(char_dev_sysfs_class);
		unregister_chrdev_region(char_dev_sysfs_dev_t,1);
		printk(KERN_ERR "device_create fail");
		return -ENOMEM;
	}

	char_dev_sysfs_cdev = cdev_alloc();
	if(char_dev_sysfs_cdev == NULL) {
		device_destroy(char_dev_sysfs_class, char_dev_sysfs_dev_t);
		class_destroy(char_dev_sysfs_class);
		unregister_chrdev_region(char_dev_sysfs_dev_t,1);
		printk(KERN_ERR "failed to alloc cdev\n");
		return -ENOMEM;
	}

	cdev_init(char_dev_sysfs_cdev, &char_dev_sysfs_fops);

	retval = cdev_add(char_dev_sysfs_cdev, char_dev_sysfs_dev_t, 1);
	if(retval < 0) {
		cdev_del(char_dev_sysfs_cdev);
		device_destroy(char_dev_sysfs_class, char_dev_sysfs_dev_t);
		class_destroy(char_dev_sysfs_class);
		unregister_chrdev_region(char_dev_sysfs_dev_t,1);
		printk(KERN_ERR "failed to add cdev\n");
		return -EBUSY;
	}

	return 0;
}

static void __exit char_dev_sysfs_exit(void)
{
	cdev_del(char_dev_sysfs_cdev);
	device_destroy(char_dev_sysfs_class, char_dev_sysfs_dev_t);
	class_destroy(char_dev_sysfs_class);
	unregister_chrdev_region(char_dev_sysfs_dev_t,1);
	
	printk(KERN_INFO "module exit \n");
}

module_init(char_dev_sysfs_init);
module_exit(char_dev_sysfs_exit);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Maeng"); 
MODULE_DESCRIPTION("A character device module init");
MODULE_VERSION("0.1"); 

