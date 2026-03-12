#ifndef _LINUX_TAR_H
#define _LINUX_TAR_H

#include <linux/types.h>

/* Standard USTAR 512-byte block */
struct tar_header {
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char padding[12];
} __attribute__((packed));

/* Parses a TAR archive starting at the given memory address */
void parse_tar(uint64_t archive_addr);

#endif /* _LINUX_TAR_H */
