ifneq ($(KERNELRELEASE),)
obj-m += hello_world_char_driver.o
else
KDIR := /path/to/kernel/sources
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif
