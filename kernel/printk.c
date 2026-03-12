#include <linux/kernel.h>
#include <linux/types.h>
#include <stdarg.h>

/* Using the external HAL write function directly */
extern void ag_hal_console_write(const char *str);

static void print_char(char c) {
    char buf[2] = {c, '\0'};
    ag_hal_console_write(buf);
}

static void print_string(const char *s) {
    if (!s) s = "(null)";
    ag_hal_console_write(s);
}

static void itoa(int value, char *str, int base) {
    char *ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;

    if (value < 0 && base == 10) {
        value = -value;
        *ptr++ = '-';
        ptr1++;
    }

    if (value == 0) {
        *ptr++ = '0';
    } else {
        while (value > 0) {
            tmp_value = value % base;
            *ptr++ = "0123456789abcdef"[tmp_value];
            value /= base;
        }
    }
    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

int printk(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 's': {
                    const char *s = va_arg(args, const char *);
                    print_string(s);
                    break;
                }
                case 'd': {
                    int d = va_arg(args, int);
                    char buf[32];
                    itoa(d, buf, 10);
                    print_string(buf);
                    break;
                }
                case 'x': {
                    int x = va_arg(args, int);
                    char buf[32];
                    itoa(x, buf, 16);
                    print_string(buf);
                    break;
                }
                case 'c': {
                    char c = (char) va_arg(args, int);
                    print_char(c);
                    break;
                }
                case '%': {
                    print_char('%');
                    break;
                }
                default: {
                    print_char('%');
                    print_char(*fmt);
                    break;
                }
            }
        } else {
            print_char(*fmt);
        }
        fmt++;
    }

    va_end(args);
    return 0; /* Returning 0 for simplicity right now */
}
