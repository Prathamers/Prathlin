# Contributing to Plinux

First off, thank you for considering contributing to Plinux! 🎉 
This project is meant to be a foundation for anyone wanting to learn Operating System development.

## 🛠 How to Contribute
1. **Fork the Repository** on GitHub/GitLab.
2. **Create a Feature Branch** (`git checkout -b feature/my-cool-driver`).
3. **Commit your changes**: Ensure your code is clean and works flawlessly in an emulator like QEMU.
4. **Push to the Branch** (`git push origin feature/my-cool-driver`).
5. **Open a Pull Request**.

## 💻 Contribution Areas (Roadmap)
Looking for something to build? Here is what Plinux needs next:
- **Global Descriptor Table (GDT)**: Protect kernel memory!
- **Interrupts (IDT)**: Allow the kernel to handle hardware IRQs and Exceptions.
- **Physical Page Allocator**: Dynamically allocate RAM frames.
- **Keyboard Driver**: Hook into IRQ 1 so we can type!

## 🧑‍💻 Code Style Guidelines
- **C Code**: We try to follow the Linux Kernel coding style where possible (8-character indents via tabs, brace placement).
- **No external Libc**: Remember, we are building a kernel! You cannot `#include <stdio.h>`. You must write the functionality yourself using the `include/linux/` headers.

By contributing, you agree that your code will be licensed under the MIT License.
