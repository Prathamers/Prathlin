/*
 *  linux/kernel/main.c
 *
 *  (C) 2026 Plinux Contributors
 *
 *  Main kernel entry point for Plinux.
 *  Uses the AntiGravity hardware abstraction layer.
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/multiboot.h>
#include <linux/tar.h>

/*
 * AntiGravity Hardware Abstraction Layer Interface Hooks
 * These define how Plinux communicates with the host environment
 * over the AntiGravity framework.
 */
extern void ag_hal_init(void);
extern void ag_hal_console_clear(void);
extern void ag_hal_console_write(const char *str);
extern void ag_hal_idle(void);

/**
 * kernel_main - Boot entry for the Plinux kernel
 * 
 * This is called directly from the architecture specific 
 * bootloader stub (e.g. boot.asm). It initializes the HAL
 * and starts the main kernel event loop.
 */
void kernel_main(uint32_t magic, uint32_t addr)
{
	/* Initialize the AntiGravity hardware abstraction layer */
	ag_hal_init();

	/* Test our newly written string library */
	const char *hello_msg = "Booting Plinux with new features kernel!\n";
	size_t len = strlen(hello_msg);

	/* Clear screen and print early boot message using printk */
	ag_hal_console_clear();
	printk("Plinux Kernel initialized.\n");

	if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
		printk("Multiboot header parsed correctly.\n");
		struct multiboot_info *mbi = (struct multiboot_info *)(uint64_t)addr;

		/* Check if we have modules loaded (like our initrd) */
		if (mbi->flags & (1 << 3)) { /* Bit 3 indicates mods_* fields are valid */
			uint32_t mods_count = mbi->mods_count;
			printk("Found %d boot module(s).\n", mods_count);

			if (mods_count > 0) {
				struct multiboot_mod_list *mod = (struct multiboot_mod_list *)(uint64_t)mbi->mods_addr;
				printk("Initrd loaded at: 0x%x\n", mod->mod_start);
				parse_tar(mod->mod_start);
			}
		} else {
			printk("No modules found by bootloader.\n");
		}
	} else {
		panic("Invalid Multiboot magic: %x", magic);
	}

	/* Triggering a kernel panic to show it stops execution */
	// panic("Test panic initiated...");

	/* Main system loop */
	for (;;) {
		/* Yield CPU until next hardware interrupt */
		ag_hal_idle();
	}
}
