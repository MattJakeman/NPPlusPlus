cmd_/home/matt/Dropbox/phd_stuff/code/np-mod/test_map/sample_map.mod.o := /home/matt/openwrt/kamikaze/staging_dir/toolchain-mipsel_gcc-4.1.2_uClibc-0.9.30.1/usr/bin/mipsel-openwrt-linux-uclibc-gcc -Wp,-MD,/home/matt/Dropbox/phd_stuff/code/np-mod/test_map/.sample_map.mod.o.d  -nostdinc -isystem /home/matt/openwrt/kamikaze/staging_dir/toolchain-mipsel_gcc-4.1.2_uClibc-0.9.30.1/usr/lib/gcc/mipsel-openwrt-linux-uclibc/4.1.2/include -D__KERNEL__ -Iinclude  -I/home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include -include include/linux/autoconf.h -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -O2 -ffunction-sections -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=mips32 -Wa,-mips32 -Wa,--trap -I/home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/mach-bcm47xx -ffreestanding -I/home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/mach-generic -D"VMLINUX_LOAD_ADDRESS=0xffffffff80001000" -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fwrapv  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(sample_map.mod)"  -D"KBUILD_MODNAME=KBUILD_STR(sample_map)"  -DMODULE -mno-long-calls -c -o /home/matt/Dropbox/phd_stuff/code/np-mod/test_map/sample_map.mod.o /home/matt/Dropbox/phd_stuff/code/np-mod/test_map/sample_map.mod.c

deps_/home/matt/Dropbox/phd_stuff/code/np-mod/test_map/sample_map.mod.o := \
  /home/matt/Dropbox/phd_stuff/code/np-mod/test_map/sample_map.mod.c \
    $(wildcard include/config/module/unload.h) \
  include/linux/module.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/markers.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/sysfs.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc4.h \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/poison.h \
  include/linux/prefetch.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbd.h) \
    $(wildcard include/config/lsf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
  include/linux/posix_types.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/posix_types.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/sgidefs.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/types.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/64bit/phys/addr.h) \
    $(wildcard include/config/64bit.h) \
  include/asm-generic/int-ll64.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/processor.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/mips/mt/fpaff.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/cpumask/offstack.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/printk/debug.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /home/matt/openwrt/kamikaze/staging_dir/toolchain-mipsel_gcc-4.1.2_uClibc-0.9.30.1/usr/lib/gcc/mipsel-openwrt-linux-uclibc/4.1.2/include/stdarg.h \
  include/linux/linkage.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/linkage.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/bitops.h \
    $(wildcard include/config/cpu/mipsr2.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/x86.h) \
  include/linux/typecheck.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/irqflags.h \
    $(wildcard include/config/mips/mt/smtc.h) \
    $(wildcard include/config/irq/cpu.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/cpu-features.h \
    $(wildcard include/config/cpu/mipsr2/irq/vi.h) \
    $(wildcard include/config/cpu/mipsr2/irq/ei.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/cpu.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/cpu-info.h \
    $(wildcard include/config/mips/mt/smp.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/mach-generic/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/mach-bcm47xx/cpu-feature-overrides.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/barrier.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/sgi/ip28.h) \
    $(wildcard include/config/cpu/has/wb.h) \
    $(wildcard include/config/weak/ordering.h) \
    $(wildcard include/config/weak/reordering/beyond/llsc.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/break.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/debug/bugverbose.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/byteorder.h \
    $(wildcard include/config/cpu/mips64/r2.h) \
  include/linux/byteorder.h \
  include/linux/swab.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/mach-bcm47xx/war.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/find.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/ext2-non-atomic.h \
  include/asm-generic/bitops/le.h \
  include/asm-generic/bitops/ext2-atomic.h \
  include/asm-generic/bitops/minix.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/ratelimit.h \
  include/linux/param.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/dynamic_printk.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/string.h \
    $(wildcard include/config/cpu/r3000.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/cachectl.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/prefetch.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/system.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/addrspace.h \
    $(wildcard include/config/cpu/r8000.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/mach-generic/spaces.h \
    $(wildcard include/config/dma/noncoherent.h) \
  include/linux/const.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/cmpxchg.h \
  include/asm-generic/cmpxchg-local.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/dsp.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/watch.h \
    $(wildcard include/config/hardware/watchpoints.h) \
  include/linux/stat.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/stat.h \
  include/linux/time.h \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/seqlock.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/thread_info.h \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/prove/locking.h) \
  include/linux/spinlock_up.h \
  include/linux/spinlock_api_up.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/atomic.h \
  include/asm-generic/atomic.h \
  include/linux/math64.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/kmod.h \
  include/linux/gfp.h \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/unevictable/lru.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
  include/linux/wait.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/current.h \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/init.h \
    $(wildcard include/config/hotplug.h) \
  include/linux/nodemask.h \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/bounds.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/page.h \
    $(wildcard include/config/cpu/mips32.h) \
  include/linux/pfn.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/io.h \
  include/asm-generic/iomap.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/pgtable-bits.h \
    $(wildcard include/config/cpu/tx39xx.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/mach-generic/ioremap.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/mach-generic/mangle-port.h \
    $(wildcard include/config/swap/io/space.h) \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/out/of/line/pfn/to/page.h) \
  include/asm-generic/page.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/memory/hotremove.h) \
  include/linux/notifier.h \
  include/linux/errno.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/topology.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/mach-generic/topology.h \
  include/asm-generic/topology.h \
  include/linux/elf.h \
  include/linux/elf-em.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/elf.h \
    $(wildcard include/config/mips32/n32.h) \
    $(wildcard include/config/mips32/o32.h) \
    $(wildcard include/config/mips32/compat.h) \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kref.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/marker.h \
  include/linux/tracepoint.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/classic/rcu.h) \
  include/linux/percpu.h \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
  include/linux/slab_def.h \
  include/linux/kmalloc_sizes.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/percpu.h \
  include/asm-generic/percpu.h \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/completion.h \
  include/linux/rcuclassic.h \
    $(wildcard include/config/rcu/cpu/stall/detector.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/local.h \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/module.h \
    $(wildcard include/config/cpu/mips32/r1.h) \
    $(wildcard include/config/cpu/mips32/r2.h) \
    $(wildcard include/config/cpu/mips64/r1.h) \
    $(wildcard include/config/cpu/r4300.h) \
    $(wildcard include/config/cpu/r4x00.h) \
    $(wildcard include/config/cpu/tx49xx.h) \
    $(wildcard include/config/cpu/r5000.h) \
    $(wildcard include/config/cpu/r5432.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/r6000.h) \
    $(wildcard include/config/cpu/nevada.h) \
    $(wildcard include/config/cpu/rm7000.h) \
    $(wildcard include/config/cpu/loongson2.h) \
  /home/matt/openwrt/kamikaze/build_dir/linux-brcm47xx/linux-2.6.28.10/arch/mips/include/asm/uaccess.h \
  include/linux/vermagic.h \
  include/linux/utsrelease.h \

/home/matt/Dropbox/phd_stuff/code/np-mod/test_map/sample_map.mod.o: $(deps_/home/matt/Dropbox/phd_stuff/code/np-mod/test_map/sample_map.mod.o)

$(deps_/home/matt/Dropbox/phd_stuff/code/np-mod/test_map/sample_map.mod.o):
