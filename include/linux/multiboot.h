#ifndef _LINUX_MULTIBOOT_H
#define _LINUX_MULTIBOOT_H

#include <linux/types.h>

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

/* The Multiboot information structure */
struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    /* ... other fields omitted for simplicity, we only need mods */
} __attribute__((packed));

/* The Multiboot module structure */
struct multiboot_mod_list {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t cmdline;
    uint32_t pad;
} __attribute__((packed));

#endif /* _LINUX_MULTIBOOT_H */
