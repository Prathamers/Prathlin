#include <linux/tar.h>
#include <linux/kernel.h>
#include <linux/string.h>

/* Convert octal ASCII string from TAR header to integer */
static uint32_t parse_octal(const char *str, size_t size) {
    uint32_t val = 0;
    for (size_t i = 0; i < size; i++) {
        if (str[i] >= '0' && str[i] <= '7') {
            val = val * 8 + (str[i] - '0');
        }
    }
    return val;
}

void parse_tar(uint64_t archive_addr) {
    uint64_t current_addr = archive_addr;
    printk("Scanning for packages in TAR at 0x%x...\n", (uint32_t)archive_addr);

    while (1) {
        struct tar_header *header = (struct tar_header *)current_addr;

        /* A TAR archive ends with two consecutive 512-byte blocks of zeros. */
        if (header->filename[0] == '\0') {
            break; 
        }

        uint32_t size = parse_octal(header->size, 11);

        /* Only process normal files */
        if (header->typeflag == '0' || header->typeflag == '\0') {
            printk(" [Package Manager] Installing: %s (%d bytes)\n", header->filename, size);
            
            /* The actual file data starts immediately after the 512-byte header */
            // const char *file_data = (const char *)(current_addr + 512);
            /* Here we COULD copy the file_data to a virtual filesystem, but for now 
             * we just log that we parsed it successfully. */
        }

        /* Move to the next block. File data is padded to 512-byte boundaries */
        uint64_t blocks = (size / 512) + ((size % 512 == 0) ? 0 : 1);
        current_addr += 512 + (blocks * 512);
    }
    
    printk("Package installation complete.\n");
}
