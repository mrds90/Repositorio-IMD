ifneq ($(KERNELRELEASE),)
obj-m := timer.o
else
KDIR := /path/to/kernel/sources
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif
