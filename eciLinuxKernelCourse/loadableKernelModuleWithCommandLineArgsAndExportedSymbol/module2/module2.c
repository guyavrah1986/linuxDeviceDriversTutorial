#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

#define MAX_LEN 30

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple kernel module");
MODULE_AUTHOR("Guy Avraham");

const char* moduleName = "module2";

//1)  this variable is defined in another module (in this case, in module1)
extern const int exportedIntVarSymbol;


// 2) this is a place holder with a default value of 9, for an int argument that will be
//    passed via a command line argument once the module is loaded
static int arg1 = 9;
module_param(arg1, int, 0);


static int __init hello_init(void)
{
	printk(KERN_ALERT "hello_init - loading module:%s\n", moduleName);
	printk(KERN_ALERT "hello_init - %s: the int got as a command line argument is:%d, and the exported variable from module1 is:%d\n", moduleName, arg1, exportedIntVarSymbol);
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_ALERT "hello_exit - unloading module\n");
}

module_init(hello_init);
module_exit(hello_exit);




