#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

/* The hardware-agnostic contract provided by your HAL */
extern void ag_hal_console_write(const char *str);

/* Internal helper to convert integers to strings */
static void itoa(char *buf, int base, int d) {
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;
    
    // Handle hex vs decimal
    if (base == 16) divisor = 16;
    
    // Handle negative numbers for decimal base
    if (base == 10 && d < 0) {
        *p++ = '-';
        buf++;
        ud = -d;
    } else if (base == 16) {
        *p++ = '0';
        *p++ = 'x';
        buf += 2;
    }
    
    // Divide down to calculate characters
    do {
        int remainder = ud % divisor;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while (ud /= divisor);
    
    *p = 0; // Null terminate
    
    // Reverse the string
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

/* * The core kernel print function.
 * Supports basic specifiers: %d (int), %x (hex), %s (string), %c (char)
 */
int printk(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    char buffer[1024]; // Safe isolated buffer for formatted string
    char *buf_ptr = buffer;
    
    for (int i = 0; fmt[i] != '\0'; i++) {
        // Prevent buffer overflow (leave room for null terminator)
        if (buf_ptr - buffer >= 1023) {
            break;
        }

        if (fmt[i] != '%') {
            *buf_ptr++ = fmt[i];
            continue;
        }
        
        i++; // Move past the '%'
        
        switch (fmt[i]) {
            case 'd': { // Integer
                char int_str[32];
                itoa(int_str, 10, va_arg(args, int));
                for (int j = 0; int_str[j] != '\0'; j++) {
                    if (buf_ptr - buffer < 1023) *buf_ptr++ = int_str[j];
                }
                break;
            }
            case 'x': { // Hexadecimal
                char hex_str[32];
                itoa(hex_str, 16, va_arg(args, int));
                for (int j = 0; hex_str[j] != '\0'; j++) {
                    if (buf_ptr - buffer < 1023) *buf_ptr++ = hex_str[j];
                }
                break;
            }
            case 's': { // String
                char *s = va_arg(args, char *);
                if (!s) s = "(null)";
                for (int j = 0; s[j] != '\0'; j++) {
                    if (buf_ptr - buffer < 1023) *buf_ptr++ = s[j];
                }
                break;
            }
            case 'c': { // Character
                if (buf_ptr - buffer < 1023) {
                    *buf_ptr++ = (char)va_arg(args, int); // chars are promoted to int in variadic args
                }
                break;
            }
            case '%': { // Escaped percent sign
                *buf_ptr++ = '%';
                break;
            }
            default: { // Unknown specifier, print it raw
                *buf_ptr++ = '%';
                if (buf_ptr - buffer < 1023) *buf_ptr++ = fmt[i];
                break;
            }
        }
    }
    
    *buf_ptr = '\0'; // Null terminate the final string
    va_end(args);
    
    // Dispatch to the architecture-specific HAL
    ag_hal_console_write(buffer);
    
    return buf_ptr - buffer; // Return number of characters written
}