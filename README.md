# Plinux 🐧

Plinux is an open-source, custom Operating System Kernel written from scratch in C and x86_64 Assembly. It is designed to be lightweight, educational, and highly modular. 

Currently, Plinux is a bare-metal kernel that supports GRUB Multiboot, custom hardware abstraction layers (HAL), and an in-memory Initrd Package Manager!

## 🚀 Features
- **GRUB Multiboot 1 Compliant**
- Lightweight VGA Text-Mode Console Driver (`printk`)
- Custom Standard Library Baselines (`string.h`)
- **Memory-based Initrd Package Manager** (Reads TAR packages on boot)
- Fully Open Source (MIT License)

## 🛠 Building the OS

### Prerequisites
To compile the kernel and build a bootable ISO, you will need the following dependencies installed on your Linux host:
```bash
# On Fedora/RHEL Systems:
sudo dnf install -y make gcc nasm grub2-tools grub2-tools-extra xorriso

# On Debian/Ubuntu Systems:
sudo apt install -y make gcc nasm grub-pc-bin xorriso
```

### Compiling
Simply run the Makefile in the root directory:
```bash
make clean && make iso
```
This will compile the C and Assembly files, bundle the `packages/` directory into a TAR file, and generate `plinux.iso`.

### Running
You can boot Plinux using an emulator like QEMU or VirtualBox:
```bash
qemu-system-x86_64 -cdrom plinux.iso
```

## 📁 Directory Structure
- `arch/` - Architecture-specific bootstrap code (`boot.asm`)
- `include/` - Standard standard C library headers (`linux/types.h`, `linux/string.h`)
- `kernel/` - Core OS logic (`main.c`, `printk.c`, `tar.c`)
- `lib/` - Standard capabilities (`string.c`)
- `packages/` - Text files bundled into the Initrd Package Manager

## 🤝 Contributing
Want to build your own OS on top of Plinux or contribute features? Please read our [CONTRIBUTING.md](CONTRIBUTING.md) guide!

## 📜 License
Plinux is released under the permissive [MIT License](LICENSE). Do whatever you want with it!
