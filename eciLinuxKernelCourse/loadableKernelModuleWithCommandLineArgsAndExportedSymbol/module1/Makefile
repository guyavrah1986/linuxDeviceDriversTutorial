# Makefile for the simple kernel module
obj-m += module1.o 
KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

EXTRA_CFLAGS = -g

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
