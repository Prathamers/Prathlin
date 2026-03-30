.section .text.boot
.global _start

_start:
    // 1. Read the CPU ID. We only want core 0 to initialize the kernel.
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, master_core

    // Secondary cores are parked here in an infinite wait-for-event loop.
hang:
    wfe
    b       hang

master_core:
    // 2. Set up the stack pointer. 
    // We safely use the memory below the loaded kernel (_start) for the initial stack.
    ldr     x1, =_start
    mov     sp, x1

    // 3. Clear the BSS segment (uninitialized variables must be zeroed out).
    ldr     x1, =__bss_start
    ldr     x2, =__bss_end
    sub     x2, x2, x1      // Calculate size of BSS
    cbz     x2, jump_to_c   // If BSS size is 0, skip clearing

clear_bss_loop:
    str     xzr, [x1], #8   // Store 64-bit zero (xzr) and increment address by 8
    sub     x2, x2, #8
    cbnz    x2, clear_bss_loop

jump_to_c:
    // 4. Branch to the C entry point.
    // Register x0 currently holds the physical address of the DTB passed by the bootloader.
    // Ensure your main() signature in kernel/main.c looks like: void main(uint64_t dtb_ptr)
    bl      main

    // Fallback halt if main() ever returns
    b       hang