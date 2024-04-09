ifneq ($(KERNELRELEASE),)
obj-m += holamundo_i2c_driver.o
else
KERNEL_DIR ?= $(HOME)/dev_rpi3_64_glibc_labs/kernel/linux-stable
all:
	$(MAKE) -C $(KERNEL_DIR) M=$$PWD
endif
