//======================================================================================================================
//
// 1) These two function are needed in order to load (init_module) and unload (cleanup_module) the driver.
// a) The simpleCharDevInit() method is the entry point for the device when it is loaded (by, say, insmod). This is the place where all desired initialization of the
//    device takes place. It is important to state the "all desired" due to the fact that upon termination of this function --> the kernel, as well as the application
//    programs are able to invoke (call) any registered function of the module.
// b) The simpleCharDevExit() method is the exit point for the device. This is the place where all the resources the device used MUST be freed. 
// 
// 2) These functions are basically the "driver itself" - here all the logic (implementation) of how to communicate 
//    with our device lies in. All of these functions pointers will be placed in the struct file_operations which
//    is technically speaking - kind of an interface of our module: usually the implemented functions will be read/
//    write/open/close.Each funtion which is not set by us (the driver developers) will be set to NULL by the kernel.
//    Part of the __init function goals is to set the this structure with the function pointerts of the respective 
//    functions. 
//
// 3) This is our device file_operations structure: 
//    a) The first filed is the association of this struct to this actual module. It is (almost) always needs to be
//    initialized with the macro THIS_MODULE
//    b) The rest of the initialization are optional. We are using here the C99 notation to save code and have the code
//    more portable. Each function which is not explictly initiazlied here is set to NULL by the kernel.
//======================================================================================================================

/*
 *  simpleCharDev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
*/

#include <linux/kernel.h>	// needed for every module
#include <linux/module.h>	// needed for every module
#include <linux/device.h>       // needed for device_create() device_destroy()

#include <linux/types.h>	// needed for dev_t type
#include <linux/fs.h>		// needed for alloc_chrdev_region() and unregister_chrdev_region()

#include <linux/cdev.h>		// needed for the cdev_init() and cdev_add()

#include <asm/uaccess.h>	/* for put_user */

#define SUCCESS 0
#define DEVICE_NAME "my_char_dev"			// Device name as it appears in /proc/devices
#define DRIVER_CLASS_NAME "my_char_dev_class"		// class name (for the device) as it appears in /sys/class
#define BUF_LEN 80					// Max length of the message from the device
#define MY_SIMPLE_CHAR_DEV "mySimpleCharDev"		// for debug (common prefix for messages in the logs).

static char bufferToRead [] = "Hey there from my_char_dev";

MODULE_LICENSE("GPL");

// init and cleanup methods
// ------------------------
static int __init simpleCharDevInit(void);
static void __exit simpleCharDevExit(void);

// basic device operations
// -----------------------
static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);

// device needed structures (objects)
// ----------------------------------
static dev_t dev;		// will hold the device data type variable (major and minor numbers)
static struct cdev* cDev;	// will hold the kernel's character device type for our character device
struct class* my_class;		// will hold the class struct for our device

// global variables needed for the module
// ---------------------------------------
static int major;		// will hold the major number assigned to our device driver 
static int Device_Open = 0;	// Is device open? - Used to prevent multiple access to device
static unsigned int count = 1;  // how many devices we wish to create
static unsigned int firstminor = 0;


// 3)
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

// 1a)
static int __init simpleCharDevInit(void)
{
	int i, res;

	// allocate major & minor(s) numbers for the device
	// do the following to see for yourself that a file named DEVICE_NAME
	major = alloc_chrdev_region(&dev, firstminor, count, DEVICE_NAME);

	if (major < 0) 
	{
		printk(KERN_ALERT "mySimpleCharDev::simpleCharDevInit - Registering char device %s failed with %d\n", DEVICE_NAME, major);
	  	return major;
	}

	// create a class for the device
    	my_class = class_create(THIS_MODULE, DRIVER_CLASS_NAME);

	// allocate a character device structure - it is used by the kernel (we can also define
	// cDev on the stack or as global and then we can skip this call
	cDev = cdev_alloc();

	// initizliae it with the respective file_operations sturct and "attach" it to this module.
	cdev_init(cDev, &fops);
	cDev->owner = THIS_MODULE;

	// add it to the kernel. 
	// NOTE: From the moment this function returns successfully the device
	// should be able to handle all sort of requests --> so any initializations 
	// that needs to be made (which the module depends on) MUST be complete up 
	// to this stage.
	res = cdev_add(cDev, dev, count);
	if (res < 0)
	{
		printk(KERN_ALERT "mySimpleCharDev::simpleCharDevInit - Adding char device %s failed with %d\n", DEVICE_NAME, res);
	  	return major;
	}

        // finally, create a device node for this device. Once the function returns,
	// device nodes will be created as /dev/my_char_dev_node__i 
	// We can also view the devices under /sys/class/DRIVER_CLASS_NAME
	// Once this function returns - the module is "live" and can recive invocations 
	// of its methods from user-space programs.
	i = 12;
        device_create(my_class, NULL, dev, NULL, "my_char_dev_node_%d", i);

	printk(KERN_INFO "mySimpleCharDev::simpleCharDevInit - device got major number:%d\n", major);
	printk(KERN_INFO "mySimpleCharDev::simpleCharDevInit - Remove the device file and module when done.\n");

	return SUCCESS;
}


static void __exit simpleCharDevExit(void)
{
	// deallocate the major & minor number(s) we have allocated for 
	// this module
	unregister_chrdev_region(dev, count);

	// destroy the device and deallocate the cdev object we allocated
	// in the __init method
	device_destroy(my_class, major);
	cdev_del(cDev);
	
	// destroy the 
	class_destroy(my_class);

	printk(KERN_ALERT "mySimpleCharDev::simpleCharDevExit - unregister_chrdev_region done\n");
}


/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/my_char_dev_node__i" (i == 12)
 */
static int device_open(struct inode *inode, struct file *file)
{
	static int counter = 0;

	if (Device_Open)
	{
		printk(KERN_INFO "mySimpleCharDev::device_open - device is already open, returning BUSY indication...\n");
		return -EBUSY;
	}


	Device_Open++;
	printk(KERN_INFO "mySimpleCharDev::device_open - I already told you %d times Hello world!\n", counter++);
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	// We're now ready for our next caller
	Device_Open--;		

	// Decrement the usage count, or else once you opened the file, you'll
	// never get rid of the module. 
	module_put(THIS_MODULE);
	printk(KERN_INFO "mySimpleCharDev::device_release - released device file\n");

	return 0;
}

/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file* filp,
			   char __user* buffer,	
			   size_t length,	
			   loff_t* offset)
{

	unsigned long bytesRead, sizeOfBufferToRead;
	sizeOfBufferToRead  = sizeof(bufferToRead);
	printk(KERN_INFO "mySimpleCharDev::device_read - got read request of %lu starting at offset %lld \n", length, *offset);
	bytesRead = copy_to_user((void*)buffer, (const void*)bufferToRead, sizeOfBufferToRead);

	printk(KERN_INFO "mySimpleCharDev::device_read - done reading %lu bytes \n", bytesRead);
	return bytesRead;
}

/*  
 * Called when a process writes to dev file: echo "hi" > /dev/hello 
 */
static ssize_t device_write(struct file* filp, const char* buff, size_t len, loff_t* off)
{
	printk(KERN_ALERT "mySimpleCharDev::device_write - %lu bytes , at offset %lld \n", len , *off);
	return -EINVAL;
}

// 1 c)
module_init(simpleCharDevInit);
module_exit(simpleCharDevExit);
