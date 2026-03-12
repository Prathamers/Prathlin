# Plinux Makefile
# Mirrors modular early Linux kernel (v0.01 - 1.0) build style
# Designed for the AntiGravity framework on Linux

CC = gcc
AS = as
LD = ld

# Compiler Flags per requirements. Forcing 32-bit as GRUB multiboot 1
# drops us into 32-bit protected mode without long mode set up.
CFLAGS = -m32 -ffreestanding -nostdlib -fno-stack-protector -Wall -Wextra -O2 -Iinclude
LDFLAGS = -m elf_i386 -nostdlib -z noexecstack

SRC_DIR = .
BUILD_DIR = build
ISO_DIR = isodir

# Automatically find C and Assembly source files using find
C_SOURCES = $(shell find $(SRC_DIR) -name '*.c' ! -path '*/build/*')
S_SOURCES = $(shell find $(SRC_DIR) -name '*.s' ! -path '*/build/*')
ASM_SOURCES = $(shell find $(SRC_DIR) -name '*.asm' ! -path '*/build/*')

# Generate object file paths inside the build directory
C_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
S_OBJECTS = $(patsubst $(SRC_DIR)/%.s, $(BUILD_DIR)/%.o, $(S_SOURCES))
ASM_OBJECTS = $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/%.o, $(ASM_SOURCES))

TARGET_INITRD = $(BUILD_DIR)/initrd.tar

OBJECTS = $(ASM_OBJECTS) $(S_OBJECTS) $(C_OBJECTS) 

TARGET_BIN = plinux.bin
TARGET_ISO = plinux.iso

.PHONY: all clean iso initrd

all: $(BUILD_DIR)/$(TARGET_BIN) initrd

initrd: $(TARGET_INITRD)

$(TARGET_INITRD):
	@mkdir -p $(BUILD_DIR)
	tar -cf $@ -C packages .

$(BUILD_DIR)/$(TARGET_BIN): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $^

# Rule for compiling C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for compiling Assembly (.s) files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $< -o $@

# Rule for compiling Assembly (.asm) files (e.g. NASM)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@

# Create an ISO using grub(2)-mkrescue
iso: all
	@mkdir -p $(ISO_DIR)/boot/grub
	cp $(BUILD_DIR)/$(TARGET_BIN) $(ISO_DIR)/boot/
	cp $(TARGET_INITRD) $(ISO_DIR)/boot/
	echo 'menuentry "Plinux" {' > $(ISO_DIR)/boot/grub/grub.cfg
	echo '  multiboot /boot/plinux.bin' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '  module /boot/initrd.tar "Initrd Packages"' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '}' >> $(ISO_DIR)/boot/grub/grub.cfg
	@if command -v grub2-mkrescue >/dev/null 2>&1; then \
		grub2-mkrescue -o $(TARGET_ISO) $(ISO_DIR); \
	elif command -v grub-mkrescue >/dev/null 2>&1; then \
		grub-mkrescue -o $(TARGET_ISO) $(ISO_DIR); \
	else \
		echo "Error: Neither grub2-mkrescue nor grub-mkrescue found." >&2; \
		exit 1; \
	fi

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) $(TARGET_ISO)
