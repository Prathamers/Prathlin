#ifndef _LINUX_KERNEL_H
#define _LINUX_KERNEL_H

#include <linux/types.h>

/* Formatted kernel output */
int printk(const char *fmt, ...);

/* Kernel halt/error */
void panic(const char *fmt, ...);

#endif /* _LINUX_KERNEL_H */
