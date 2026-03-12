#ifndef _LINUX_STRING_H
#define _LINUX_STRING_H

#include <linux/types.h>

size_t strlen(const char *s);
int strcmp(const char *cs, const char *ct);
void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *s, int c, size_t count);

#endif /* _LINUX_STRING_H */
