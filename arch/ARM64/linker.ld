ENTRY(_start)

SECTIONS
{
    /* Standard load address for AArch64 bare-metal (e.g., Raspberry Pi) */
    . = 0x80000;

    .text : {
        /* Ensure the boot assembly is physically the first thing in the binary */
        KEEP(*(.text.boot))
        *(.text .text.*)
    }

    .rodata : {
        . = ALIGN(8);
        *(.rodata .rodata.*)
    }

    .data : {
        . = ALIGN(8);
        *(.data .data.*)
    }

    .bss (NOLOAD) : {
        . = ALIGN(8);
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    }

    /* Used by boot.S to know how much memory to zero out */
    __bss_size = __bss_end - __bss_start;
}