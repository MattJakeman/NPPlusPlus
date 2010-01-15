#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x903a5ec4, "module_layout" },
	{ 0xd8778690, "per_cpu__current_task" },
	{ 0xadee93f3, "kmalloc_caches" },
	{ 0x5a34a45c, "__kmalloc" },
	{ 0xf9a482f9, "msleep" },
	{ 0x25ec1b28, "strlen" },
	{ 0x67053080, "current_kernel_time" },
	{ 0x4661e311, "__tracepoint_kmalloc" },
	{ 0x745c6eb8, "remove_proc_entry" },
	{ 0xc3ab6224, "nf_register_hook" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xde0bdcff, "memset" },
	{ 0x67ddd2d4, "proc_mkdir" },
	{ 0x234509f3, "strncat" },
	{ 0xea147363, "printk" },
	{ 0x40cca6a5, "init_net" },
	{ 0x61651be, "strcat" },
	{ 0x893412fe, "kmem_cache_alloc" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x8627f445, "create_proc_entry" },
	{ 0x6e0b3bff, "wake_up_process" },
	{ 0x522f6f75, "nf_unregister_hook" },
	{ 0x37a0cba, "kfree" },
	{ 0xa086a80d, "kthread_create" },
	{ 0x236c8c64, "memcpy" },
	{ 0xe24050c7, "scnprintf" },
	{ 0xb742fd7, "simple_strtol" },
	{ 0x945bc6a7, "copy_from_user" },
	{ 0xe914e41e, "strcpy" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "0F64A398AC9C8336C77F801");
