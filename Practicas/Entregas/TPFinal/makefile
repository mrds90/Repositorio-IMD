PROGRAM_PATH := $(CURDIR)/ads111x
DOC_PATH := $(CURDIR)/Documentation
TESTDIR := $(CURDIR)/test
NAME := ads111x_test
NAME_HOST := ads111x_test
NAME_TARGET := ads111x_test
BUILDDIR := $(CURDIR)/build
CFLAGS := -Wall -g -static # Add the -g option for debugging information
CC_HOST := gcc
# CC_TARGET := aarch64-poky-linux-gcc  -mcpu=cortex-a53+crc -fstack-protector-strong  -D_FORTIFY_SOURCE=0 -Wformat -Wformat-security -Werror=format-security --sysroot=/opt/poky/3.1.29/sysroots/aarch64-poky-linux
CC_TARGET := /home/marcos/dev_rpi3_64_glibc_labs/tools/aarch64-rpi3-linux-gnu/bin/aarch64-rpi3-linux-gnu-gcc

# Buscar todos los archivos .c y .h de forma recursiva en src y test
ALL_FILES := $(shell find $(PROGRAM_PATH) -type f \( -name "*.c" -o -name "*.h" \))

# Buscar todos los archivos .c de forma recursiva en PROGRAM_PATH
APPLST := $(shell find $(PROGRAM_PATH) -type f -name "*.c")

# Buscar todos los archivos .h de forma recursiva en PROGRAM_PATH
INCDIR := $(shell find $(PROGRAM_PATH) -type f -name "*.h")
INCDIR := $(sort $(dir $(INCDIR)))

all: $(BUILDDIR)
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) $(APPLST) -o $(BUILDDIR)/$(NAME).o

host: $(BUILDDIR)
	$(CC_HOST) $(CFLAGS) $(addprefix -I,$(INCDIR)) $(APPLST) -o $(BUILDDIR)/$(NAME_HOST).o

target: $(BUILDDIR)
	$(CC_TARGET) $(CFLAGS) $(addprefix -I,$(INCDIR)) $(APPLST) -o $(BUILDDIR)/$(NAME_TARGET).o
	
uncrustify:
	uncrustify -c uncrustifyC.cfg --replace --no-backup -q --if-changed $(ALL_FILES)


clean:
	rm -r $(BUILDDIR)

clean_docs:
	rm -r $(DOC_PATH)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)
