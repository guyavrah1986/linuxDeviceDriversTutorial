// ========================================================================================================================================================================  
//  Demonstrating the module_init() and module_exit() macros.
//  This is preferred over using init_module() and cleanup_module().
//
// 1) The fisrt thing that needs to be done for a module is to load it into the kernel. This is done simply by implementing a function with the following signature:
//    static int __init <function_name>. By convention the function should return 0 upon success and another value upon failure.
//    Basically, this function is the "entry point" of the module --> this is where it begin "to live", which technically it means that it is registered with the kernel
//    so other modules and/or the OS and/or user application program can ask from it to perfrom what it registered (its functunalities).
// =========================================================================================================================================================================  


#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/fs.h>		/* Needed for alloc_chrdev_region */

dev_t dev;

// 1)
static int __init hello_init(void)
{
	
	unsigned int firstminor = 0, count = 0;
	char name [] = "My_module2";
	int res = alloc_chrdev_region(&dev, firstminor, count, name);
	printk(KERN_INFO "Within hello_init - got res (from alloc_chrdev_region):%d \n", res);
	return 0;
}

static void __exit hello_exit(void)
{
	unsigned int count = 0;
	unregister_chrdev_region(dev, count);
	printk(KERN_INFO "Within hello_exit\n");
}

module_init(hello_init);
module_exit(hello_exit);
