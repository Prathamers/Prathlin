#include <stddef.h>
#include <stdint.h>

/* Simple VGA text mode buffer */
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;

static inline uint8_t vga_entry_color(uint8_t fg, uint8_t bg) {
    return fg | (bg << 4);
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | ((uint16_t) color << 8);
}

static size_t string_length(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

// Forward declaration to fix implicit declaration warning
void ag_hal_console_clear(void);

void ag_hal_init(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(15, 0); // White on black
    ag_hal_console_clear();
}

void ag_hal_console_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = vga_entry(' ', terminal_color);
        }
    }
}

void ag_hal_console_write(const char *str) {
    size_t len = string_length(str);
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '\n') {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT)
                terminal_row = 0; // simplistic wrap around
            continue;
        }
        const size_t index = terminal_row * VGA_WIDTH + terminal_column;
        VGA_MEMORY[index] = vga_entry((unsigned char) str[i], terminal_color);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT)
                terminal_row = 0;
        }
    }
}

void ag_hal_idle(void) {
    // Simple inline assembly to halt until next interrupt
    __asm__ __volatile__("hlt");
}
