#export-objs := np-api.o np-mappings.o
#obj-y := np-api.o np-mappings.o
#EXTRA_CFLAGS += -std=c99
#EXTRA_CFLAGS += -DEXPORT_SYMTAB

CURRENT = $(shell uname -r)
DEST = /lib/modules/$(CURRENT)/kernel/$(MDIR)

ifneq ($(KERNELRELEASE),)
	TARGET = np++
	obj-m := np++.o
	np++-objs := np-mod.o np-procfs.o np-mappings.o np-api.o
else
	TARGET = np++-gum
	obj-m := np++-gum.o
	np++-gum-objs := np-mod.o np-procfs.o np-mappings.o np-api.o
	GUMSTIX_BUILDROOT   = /home/matt/code/gumstix-buildroot-1444
	BUILD_ARM           = $(wildcard $(GUMSTIX_BUILDROOT)/build_arm*)
	KERNELDIR          ?= $(wildcard $(BUILD_ARM)/linux-*)
	CROSS_COMPILE       = $(BUILD_ARM)/staging_dir/bin/arm-linux-uclibcgnueabi-
	TARGET_ARCH=-Os -march=armv5te -mtune=xscale -Wa,-mcpu=xscale
	CC = $(CROSS_COMPILE)gcc
	PWD       := $(shell pwd)
endif

default: all

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

install:
	su -c "cp -v $(TARGET).ko $(DEST) && /sbin/depmod -a"

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

gumstix:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) modules
