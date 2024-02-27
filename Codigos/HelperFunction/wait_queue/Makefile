ifneq ($(KERNELRELEASE),)
obj-m := wait_queue.o
else
KDIR := /path/to/kernel/sources
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif
