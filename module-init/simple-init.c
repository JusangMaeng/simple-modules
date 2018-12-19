#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Maeng"); 
MODULE_DESCRIPTION("A simple module init");
MODULE_VERSION("0.1"); 

static int __init simple_start(void)
{
	printk(KERN_INFO "module init \n");

	return 0;
}

static void __exit simple_end(void)
{
	printk(KERN_INFO "module exit \n");
}

module_init(simple_start);
module_exit(simple_end);
