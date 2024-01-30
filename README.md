# CuriOS Documentation

## Overview

CuriOS is a simple educational and hobbyist operating system kernel designed to demonstrate fundamental OS concepts and provide a platform for learning low-level system programming. Starting from a basic bootloader, it enters 32-bit protected mode and sets up a minimal runtime environment, including screen output capabilities.

## Components

- **Bootloader (`boot.asm`):** Initializes the system and loads the kernel into memory.
- **Kernel (`kernel.asm`, `kernel.c`):** Represents the core part of the operating system, managing system resources, and providing basic services including screen output. It now includes functionality to remap the PIC and handle keyboard interrupts.
- **Makefile:** Automates the compilation and linking process.
- **Linker Script (`linker.ld`):** Defines the memory layout of the kernel.
- **Run Script (`run_os.sh`):** Streamlines the process of building and running the OS.
- **IDT Functionality (`/idt/idt.asm`, `/idt/idt.c`, `/idt/idt.h`):** Implements the Interrupt Descriptor Table (IDT) and handles keyboard interrupts.
- **Memory Utilities (`/memory`):** Contains a simple `memset` function for memory management.
- **I/O Utilities (`/io/io.asm`, `/io/io.h`):** Provides basic I/O functionality for writing to ports.

### IDT Functionality (`/idt`)

#### Key Features

- **IDT Setup:** Configures the Interrupt Descriptor Table for handling hardware interrupts.
- **Keyboard Interrupt Handler:** Handles keyboard press and release, displaying output on the screen.

#### Detailed Functionality

- **Interrupt Handlers:** Implements specific interrupt service routines, such as for keyboard interrupts.
- **IDT Initialization:** Sets up IDT entries for various interrupts and loads the IDT.

### Memory Utilities (`/memory`)

- Contains a simple `memset` function, essential for low-level memory operations.

### I/O Utilities (`/io`)

- Provides basic functions for interacting with hardware I/O ports, crucial for device communication.

### Kernel Modifications

- **PIC Remapping:** `kernel.asm` now includes functionality to remap the Programmable Interrupt Controller.
- **Keyboard Interaction:** Displays a message on the screen when a key is pressed and released.

## Build Instructions for CuriOS

### Prerequisites

- Ensure `make`, NASM (Netwide Assembler), GCC, and QEMU are installed on your system.

### Steps

The `run_os.sh` script automates the building and emulation of the kernel. Specifically, it does the following:

1. **Clean Previous Builds:** Use `make clean` to clear previous build artifacts.
2. **Compile and Build:** Execute `./build.sh` to compile and link components.
3. **Run with QEMU:** Run `qemu-system-i386 -hda ./os.bin`

#### Notes

- Ensure `build.sh` and `run_os.sh` are executable (`chmod +x`).
- Check all dependencies and environment setup for x86 assembly and C development.

## Additional Notes

- CuriOS, designed primarily for educational purposes, now includes extended functionality like IDT handling, simple memory operations, and basic I/O utilities.
- It demonstrates more advanced OS development concepts such as interrupt handling, memory management, and hardware I/O operations.
- This documentation reflects the latest state of CuriOS and will be updated as the project evolves.
