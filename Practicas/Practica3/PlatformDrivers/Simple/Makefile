ifneq ($(KERNELRELEASE),)
obj-m += simple_platform_driver.o
else
KERNEL_DIR ?= $(HOME)/dev_rpi3_64_glibc_labs/kernel/linux-stable
all:
	$(MAKE) -C $(KERNEL_DIR) M=$$PWD
endif
