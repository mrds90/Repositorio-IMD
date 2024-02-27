ifneq ($(KERNELRELEASE),)
obj-m := sleep.o
else
KDIR := /path/to/kernel/sources
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif
