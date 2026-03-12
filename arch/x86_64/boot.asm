; boot.asm - Early bootstrap code for Plinux
; Assumes x86_64 architecture and Multiboot compliance

; Multiboot header constants
MBALIGN     equ  1 << 0                   ; Align loaded modules on page boundaries
MEMINFO     equ  1 << 1                   ; Provide memory map
FLAGS       equ  MBALIGN | MEMINFO        ; Multiboot 'flag' field
MAGIC       equ  0x1BADB002               ; 'Magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)          ; Checksum of above, to prove we are multiboot

; Declare Multiboot Header
section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; Allocate an initial stack
section .bss
align 16
stack_bottom:
	resb 16384                            ; 16 KiB stack
stack_top:

section .text
global _start
extern kernel_main                        ; Defined in kernel/main.c

_start:
	; Set up the stack pointer
	mov esp, stack_top

	; Multiboot puts the magic number in EAX and the
	; info structure pointer in EBX.
	; Since GRUB Multiboot 1 drops us into 32-bit protected mode,
	; we cannot safely use 64-bit registers (rax, rdi) without setting up
	; long mode first. If we compile our C code as 32-bit (-m32), we push args
	; to the stack.
	push ebx
	push eax

	; Enter the C kernel main function
	call kernel_main

	; If we unexpectedly return from kernel_main, halt the CPU
	cli
.hang:
	hlt
	jmp .hang
