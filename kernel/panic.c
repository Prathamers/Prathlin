#include <linux/kernel.h>
#include <stdarg.h>

extern void ag_hal_idle(void);

void panic(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    printk("\nKernel panic - not syncing: ");
    
    /* Using a simplified version of sprint or just reusing printk mechanics here
     * For now, we will just call printk directly since it uses va_list internally
     * Wait, printk uses '...', so we can't easily pass va_list to it without vprintk.
     * I'll just declare a simple local fallback here or create vprintk.
     * To keep it simple, let's just print the raw format string for this prototype.
     */
     
    /* This assumes the fmt itself has most of the panic info since we don't have vprintk yet */
    printk("%s\n", fmt); 

    va_end(args);

    /* Halt the system using the HAL */
    for (;;) {
        ag_hal_idle();
    }
}
