#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/tar.h>

/* Only include multiboot headers if we are building for x86 */
#if defined(__x86_64__)
#include <linux/multiboot.h>
#endif

extern void ag_hal_init(void);
extern void ag_hal_console_clear(void);
extern void ag_hal_console_write(const char *str);
extern void ag_hal_idle(void);

/**
 * kernel_main - Boot entry for the Prathlin kernel
 * * Arguments dynamically adapt to the architecture:
 * - x86_64: arg1 = Multiboot Magic, arg2 = Multiboot Info Address
 * - arm64:  arg1 = Device Tree Blob (DTB) Address, arg2 = unused
 */
void kernel_main(uint64_t arg1, uint64_t arg2)
{
    /* Initialize the AntiGravity hardware abstraction layer */
    ag_hal_init();
    ag_hal_console_clear();

    printk("Booting Prathlin Kernel...\n");

#if defined(__x86_64__)
    /* ==========================================
     * x86_64 MULTIBOOT LOGIC
     * ========================================== */
    uint32_t magic = (uint32_t)arg1;
    uint32_t addr = (uint32_t)arg2;

    if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        printk("[x86_64] Multiboot header parsed correctly.\n");
        struct multiboot_info *mbi = (struct multiboot_info *)(uint64_t)addr;

        /* Check if we have modules loaded (like our initrd) */
        if (mbi->flags & (1 << 3)) { 
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

#elif defined(__aarch64__)
    /* ==========================================
     * ARM64 DEVICE TREE LOGIC
     * ========================================== */
    uint64_t dtb_ptr = arg1;
    
    printk("[ARM64] Architecture initialized.\n");
    printk("Device Tree Blob (DTB) located at: 0x%x\n", dtb_ptr);

    /* * Note for future: ARM bootloaders do not use Multiboot structs. 
     * To load your initrd.tar on ARM, you will need to parse the DTB 
     * to read the 'linux,initrd-start' and 'linux,initrd-end' memory locations.
     */
    printk("FDT parsing for ARM initrd not yet implemented.\n");

#else
    #error "Compiler target is neither x86_64 nor aarch64!"
#endif

    printk("Kernel initialization complete. Entering idle state.\n");

    /* Main system loop */
    for (;;) {
        /* Yield CPU until next hardware interrupt */
        ag_hal_idle();
    }
}