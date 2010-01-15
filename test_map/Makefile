obj-m := sample_map.o
#sample_map-objs := sample_map.o

#TARGET = sample_map
#CURRENT = $(shell uname -r)
#DEST = /lib/modules/$(CURRENT)/kernel/$(MDIR)

#ifeq ($(KERNELRELEASE),)
#
#GUMSTIX_BUILDROOT   = /home/matt/code/gumstix-buildroot-1444
#BUILD_ARM           = $(wildcard $(GUMSTIX_BUILDROOT)/build_arm*)
#KERNELDIR          ?= $(wildcard $(BUILD_ARM)/linux-*)
#CROSS_COMPILE       = $(BUILD_ARM)/staging_dir/bin/arm-linux-uclibcgnueabi-
#TARGET_ARCH=-Os -march=armv5te -mtune=xscale -Wa,-mcpu=xscale
#CC = $(CROSS_COMPILE)gcc
#PWD       := $(shell pwd)
#
#endif

    OPENWRT_BUILDROOT = /home/matt/openwrt/kamikaze
#   BUILD_WRT         = $(wildcard $(OPENWRT_BUILDROOT)/build_arm*)
    KERNELDIR         = /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10
    CROSS_COMPILE     = /home/matt/openwrt/kamikaze/staging_dir/toolchain-mipsel_gcc-4.1.2_uClibc-0.9.30.1/usr/bin/mipsel-openwrt-linux-uclibc-
    TARGET_ARCH       = -march=mipsel-openwrt-linux
    CC = $(CROSS_COMPILE)gcc
    PWD       := $(shell pwd)

default: all

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=mips CROSS_COMPILE=$(CROSS_COMPILE) modules
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

#gumstix:
#	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) modules
