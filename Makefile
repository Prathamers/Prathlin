# Prathlin OS Multi-Architecture Makefile
# Supports x86_64 (Intel/AMD) and arm64 (AArch64)
# Usage: 
#   make              (Builds x86_64 by default)
#   make ARCH=arm64   (Builds ARM64)
#   make iso          (Builds GRUB ISO for x86_64)

ARCH ?= x86_64

# Isolate build directories by architecture
BUILD_DIR = build/$(ARCH)
ISO_DIR = isodir

TARGET_BIN = $(BUILD_DIR)/prathlin.bin
TARGET_INITRD = $(BUILD_DIR)/initrd.tar
TARGET_ISO = prathlin_$(ARCH).iso

# ==========================================
# 1. Architecture-Specific Toolchains & Flags
# ==========================================
ifeq ($(ARCH),x86_64)
    # x86_64 / Multiboot Environment
    CC = x86_64-elf-gcc
    LD = x86_64-elf-ld
    AS = nasm
    
    # x86_64 Kernel Flags (No Red Zone, No SIMD to prevent FPU corruption on interrupts)
    CFLAGS = -m64 -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
             -ffreestanding -nostdlib -fno-stack-protector -Wall -Wextra -O2 -Iinclude
    LDFLAGS = -nostdlib -z max-page-size=0x1000 -T arch/x86_64/linker.ld
    ASFLAGS = -f elf64

    ARCH_DIR = arch/x86_64
    
else ifeq ($(ARCH),arm64)
    # ARM64 / Bare-metal Environment
    CC = aarch64-elf-gcc
    LD = aarch64-elf-ld
    AS = $(CC)
    
    # ARM64 Kernel Flags
    CFLAGS = -ffreestanding -nostdlib -fno-stack-protector -Wall -Wextra -O2 -Iinclude
    LDFLAGS = -nostdlib -T arch/arm64/linker.ld
    ASFLAGS = -c

    ARCH_DIR = arch/arm64
    
else
    $(error Unknown architecture: $(ARCH). Supported: x86_64, arm64)
endif

# ==========================================
# 2. Source Discovery & Object Mapping
# ==========================================
# Find generic files and ONLY the files for the active architecture
GENERIC_C_SRCS = $(shell find kernel lib -name '*.c')
ARCH_C_SRCS = $(shell find $(ARCH_DIR) -name '*.c')
ARCH_S_SRCS = $(shell find $(ARCH_DIR) -name '*.S' -o -name '*.s')
ARCH_ASM_SRCS = $(shell find $(ARCH_DIR) -name '*.asm')

# Map sources to object files in the targeted build directory
GENERIC_C_OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(GENERIC_C_SRCS))
ARCH_C_OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(ARCH_C_SRCS))
ARCH_S_OBJS = $(patsubst %.S, $(BUILD_DIR)/%.o, $(patsubst %.s, $(BUILD_DIR)/%.o, $(ARCH_S_SRCS)))
ARCH_ASM_OBJS = $(patsubst %.asm, $(BUILD_DIR)/%.o, $(ARCH_ASM_SRCS))

OBJECTS = $(ARCH_ASM_OBJS) $(ARCH_S_OBJS) $(ARCH_C_OBJS) $(GENERIC_C_OBJS)

# ==========================================
# 3. Build Rules
# ==========================================
.PHONY: all clean iso initrd

all: $(TARGET_BIN) initrd

initrd: $(TARGET_INITRD)

# Package the initial ramdisk
$(TARGET_INITRD):
	@mkdir -p $(BUILD_DIR)
	@echo "  TAR     $@"
	@tar -cf $@ -C packages . 2>/dev/null || true

# Link the final kernel binary
$(TARGET_BIN): $(OBJECTS)
	@mkdir -p $(dir $@)
	@echo "  LD      $@"
	@$(LD) $(LDFLAGS) -o $@ $^

# Compile C Files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "  CC      $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile NASM Assembly (x86_64 boot.asm)
$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	@echo "  NASM    $<"
	@$(AS) $(ASFLAGS) $< -o $@

# Compile GNU Assembly (ARM64 boot.S)
$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	@echo "  AS      $<"
	@$(AS) $(ASFLAGS) $< -o $@

# ==========================================
# 4. ISO Generation (x86_64 Only)
# ==========================================
iso: all
ifeq ($(ARCH),x86_64)
	@echo "  GEN-ISO $(TARGET_ISO)"
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(TARGET_BIN) $(ISO_DIR)/boot/
	@cp $(TARGET_INITRD) $(ISO_DIR)/boot/ 2>/dev/null || true
	@echo 'menuentry "Prathlin OS" {' > $(ISO_DIR)/boot/grub/grub.cfg
	@echo '  multiboot /boot/prathlin.bin' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '  module /boot/initrd.tar "Initrd Packages"' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '}' >> $(ISO_DIR)/boot/grub/grub.cfg
	@if command -v grub2-mkrescue >/dev/null 2>&1; then \
		grub2-mkrescue -o $(TARGET_ISO) $(ISO_DIR); \
	elif command -v grub-mkrescue >/dev/null 2>&1; then \
		grub-mkrescue -o $(TARGET_ISO) $(ISO_DIR); \
	else \
		echo "Error: grub-mkrescue not found." >&2; exit 1; \
	fi
else
	@echo "Notice: ISO generation is for x86_64 GRUB only."
	@echo "For ARM64, boot the raw binary in QEMU:"
	@echo "  qemu-system-aarch64 -M virt -cpu cortex-a53 -kernel $(TARGET_BIN) -nographic"
endif

# ==========================================
# 5. Cleanup
# ==========================================
clean:
	@echo "  CLEAN"
	@rm -rf build $(ISO_DIR) *.iso