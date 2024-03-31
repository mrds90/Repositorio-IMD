ifneq ($(KERNELRELEASE),)
obj-m += simple_platform_driver.o
else
KDIR := /path/to/kernel/sources
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif
