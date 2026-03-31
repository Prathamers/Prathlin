# 🐧 Plinux

**Plinux** is a lightweight, modular, open-source operating system kernel built from scratch using **C** and **x86_64 Assembly**.

---

## 🚀 Overview

Plinux is a **bare-metal kernel** designed for learning and experimentation.  
It boots using **GRUB Multiboot** and includes a minimal runtime environment with a simple in-memory package system.

---

## ✨ Features

- **GRUB Multiboot v1 Compatible**
- **VGA Text Mode Console (`printk`)**
- **Initrd Package Manager (TAR-based)**
- **Custom Minimal Standard Library**
- **Hardware Abstraction Layer (HAL)**
- **Fully Open Source (MIT License)**

---

## 🧰 Requirements

### **Debian / Ubuntu**
```bash
sudo apt install -y make gcc nasm grub-pc-bin xorriso
```

### **Fedora / RHEL**
```bash
sudo dnf install -y make gcc nasm grub2-tools grub2-tools-extra xorriso
```

---

## 🛠 Build

```bash
make clean && make iso
```

### **Build Process**
- Compile C and Assembly source files  
- Package `/packages` into a TAR archive  
- Generate bootable ISO → `plinux.iso`  

---

## ▶️ Run

### **Using QEMU**
```bash
qemu-system-x86_64 -cdrom plinux.iso
```

### **Using VirtualBox**
- Create a new VM (**Type: Other / Unknown**)  
- Attach `plinux.iso`  
- Start the VM  

---

## 📂 Project Structure

```bash
plinux/
├── arch/        # Architecture-specific code
├── include/     # Header files
├── kernel/      # Core kernel logic
├── lib/         # Standard library
├── packages/    # Initrd files
├── Makefile
└── README.md
```

---

## 📦 Initrd Package System

- `/packages` is bundled into a TAR archive  
- Loaded into memory during boot  
- Read directly by the kernel  

### **Use Cases**
- Configuration files  
- Static resources  
- Preloaded data  

---

## 🤝 Contributing

- Fork the repository  
- Create a new branch  
- Submit a Pull Request  

See: `CONTRIBUTING.md`

---

## 📜 License

**MIT License**

You are free to use, modify, and distribute this project.

---

## 🛣️ Roadmap

- [ ] Keyboard Driver  
- [ ] Memory Management (Paging / Heap)  
- [ ] Filesystem Support (FAT / EXT2)  
- [ ] User Mode Programs  
- [ ] Shell Interface  
- [ ] Interrupt Handling (IDT)  

---

## 💡 Purpose

Plinux is intended for:

- Learning OS development  
- Low-level programming  
- System experimentation  

> ⚠️ Not production-ready

---

## ⭐ Support

If you like this project:

- Star the repository  
- Fork it  
- Build your own OS  

---
