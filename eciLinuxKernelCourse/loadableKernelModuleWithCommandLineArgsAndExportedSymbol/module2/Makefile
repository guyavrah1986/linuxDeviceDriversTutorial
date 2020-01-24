# Makefile for the simple kernel module that exports symbols from module1
KBUILD_EXTRA_SYMBOLS := /home/gavraham/dev/linuxDeviceDriversTutorial/eciLinuxKernelCourse/loadableKernelModuleWithCommandLineArgsAndExportedSymbol/module1/build/Module.symvers

obj-m += module2.o 
KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

EXTRA_CFLAGS = -g

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
