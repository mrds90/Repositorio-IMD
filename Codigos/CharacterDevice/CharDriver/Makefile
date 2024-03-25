ifneq ($(KERNELRELEASE),)
obj-m += hello_world_char_driver.o
else
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif
