obj-m += src/hello_world.o

KERNEL_DIR ?= $(HOME)/dev_rpi3_64_glibc_labs/kernel/linux-stable

all:
	make -C $(KERNEL_DIR) ARCH=arm64 CROSS_COMPILE=aarch64-rpi3-linux-gnu- M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) ARCH=arm64 CROSS_COMPILE=aarch64-rpi3-linux-gnu- M=$(PWD) clean

deploy:
	cd src \
	cp *.ko $(HOME)/dev_rpi3_64_glibc_labs/rootfs/hello_world \
	cd ..



