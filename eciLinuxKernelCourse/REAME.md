## Simple Linux kernel module

# Prerequisites:
- In order to be able to build (compile) a kernel module where the target machine is the development machine (i.e. - the kernel module will run on the same machine where it is developed on) then the
only thing that needs to be avialble are the kernel headers of the kernel that is currently running on the system (NOTE: the "other" components of the toolchain are already exist on the machine - GCC and binutils).
  * In case the development machine is CentOs, use the following command to get the current running kernel's headers:
    * `sudo yum install "kernel-devel-uname-r == $(uname -r)"`

# The init method of the kernel module:
  * This is the function that is called when the module first installed. Any "initializations actions" that needs to be run before the module can be used, should take place here.
  * Note that it is good practice to add the `__init` macro in the function's signature, See nice explnation here: https://stackoverflow.com/a/8832142/1971003
  
