# Simple Linux kernel module

## 0) Prerequisites:
- In order to be able to build (compile) a kernel module where the target machine is the development machine (i.e. - the kernel module will run on the same machine where it is developed on) then the
only thing that needs to be avialble are the kernel headers of the kernel that is currently running on the system (NOTE: the "other" components of the toolchain are already exist on the machine - GCC and binutils).
  * In case the development machine is CentOs, use the following command to get the current running kernel's headers:
    * `sudo yum install "kernel-devel-uname-r == $(uname -r)"`
- In all the examples in this folder, to build the kernel module the only thing to 

## 1) The init method of the kernel module:
  * This is the function that is called when the module first installed. Any "initializations actions" that needs to be run before the module can be used, should take place here.
  * Note that it is good practice to add the `__init` macro in the function's signature, See nice explnation here: https://stackoverflow.com/a/8832142/1971003
  * In order to "register" the function that will be used as the "init" method of this module, use the macro: `module_init(hello_init);` (where `hello_init` is the name of the modul's init function).
 
## 2) The exit method of the kernel module:
  * This is the function that is called when the module is "un-loaded" from the kernel. It is the place to put all actions that should take place when the module stops to be active. 
  * In the same manner, the `__exit` macro should be part of the exit function's signature.
  * In order to "register" the `hello_exit` as the "exit" function of the module use the following macro: `module_exit(hello_exit);`
  
