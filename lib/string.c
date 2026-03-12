#include <linux/string.h>

size_t strlen(const char *s)
{
    size_t count = 0;
    while (*s++)
        count++;
    return count;
}

int strcmp(const char *cs, const char *ct)
{
    unsigned char c1, c2;
    while (1) {
        c1 = *cs++;
        c2 = *ct++;
        if (c1 != c2)
            return c1 < c2 ? -1 : 1;
        if (!c1)
            break;
    }
    return 0;
}

void *memcpy(void *dest, const void *src, size_t count)
{
    char *tmp = dest;
    const char *s = src;
    while (count--)
        *tmp++ = *s++;
    return dest;
}

void *memset(void *s, int c, size_t count)
{
    char *xs = s;
    while (count--)
        *xs++ = c;
    return s;
}
