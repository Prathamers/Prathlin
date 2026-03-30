#include <stddef.h>
#include <stdint.h>

/*
 * Memory-mapped PL011 UART base address for QEMU 'virt' machine.
 * This physically replaces the need for the x86 0xB8000 VGA buffer.
 */
#define UART0_BASE 0x09000000
static volatile uint32_t* const UART0_DR = (uint32_t*) (UART0_BASE + 0x00); // Data Register
static volatile uint32_t* const UART0_FR = (uint32_t*) (UART0_BASE + 0x18); // Flag Register

/* UART Flag Register bits */
#define UART_FR_TXFF (1 << 5) /* Transmit FIFO full */

static size_t string_length(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

// Forward declaration
void ag_hal_console_clear(void);

void ag_hal_init(void) {
    // QEMU pre-initializes the PL011 UART for us.
    // Note: If you port this to physical hardware later (like a Raspberry Pi), 
    // the baud rate and GPIO pins must be configured here.
    ag_hal_console_clear();
}

void ag_hal_console_clear(void) {
    // Because a UART is a 1D serial stream and not a 2D memory grid,
    // we send ANSI escape codes to tell the receiving terminal to clear itself.
    // \033[2J clears the screen, \033[H moves the cursor to home (0,0).
    ag_hal_console_write("\033[2J\033[H");
}

static void uart_putc(unsigned char c) {
    // Spinlock: Wait until the UART transmit FIFO has available space
    while (*UART0_FR & UART_FR_TXFF) {
        __asm__ __volatile__("nop");
    }
    // Push the character to the data register
    *UART0_DR = c;
}

void ag_hal_console_write(const char *str) {
    size_t len = string_length(str);
    for (size_t i = 0; i < len; i++) {
        // Serial consoles strictly require a carriage return '\r' 
        // before a newline '\n' to return to the left margin.
        if (str[i] == '\n') {
            uart_putc('\r');
        }
        uart_putc((unsigned char) str[i]);
    }
}

void ag_hal_idle(void) {
    // The ARM64 architectural equivalent of the x86 'hlt' instruction.
    // Wait For Interrupt (wfi) puts the core to sleep safely.
    __asm__ __volatile__("wfi");
}