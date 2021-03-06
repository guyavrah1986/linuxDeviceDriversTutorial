#include <linux/init.h>
#include <linux/module.h>


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple kernel module");
MODULE_AUTHOR("Guy Avraham");


static int __init hello_init(void)
{
	printk(KERN_ALERT "hello_init - module is loaded\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_ALERT "hello_exit - module is unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);




