# CuriOS Documentation

## Overview

CuriOS is a simple educational and hobbyist operating system kernel designed to demonstrate fundamental OS concepts and provide a platform for learning low-level system programming. Starting from a basic bootloader, it enters 32-bit protected mode and sets up a minimal runtime environment, including screen output capabilities.

## Components

- **Bootloader (`boot.asm`):** Initializes the system and loads the kernel into memory.
- **Kernel (`kernel.asm`, `kernel.c`):** Represents the core part of the operating system, managing system resources, and providing basic services including screen output.
- **Makefile:** Automates the compilation and linking process.
- **Linker Script (`linker.ld`):** Defines the memory layout of the kernel.
- **Run Script (`run_os.sh`):** Streamlines the process of building and running the OS.

### Bootloader (`boot.asm`)

#### Key Features

- **16-bit Real Mode Entry:** Begins execution in 16-bit real mode for BIOS compatibility.
- **Global Descriptor Table (GDT):** Sets up a flat memory model, simplifying early memory management.
- **32-bit Protected Mode Transition:** Shifts the CPU into 32-bit protected mode, allowing access to more memory and advanced features.
- **Sector Reading:** Reads sectors from the hard drive into memory, loading the kernel.
- **BIOS Parameter Block (BPB) Avoidance:** Skips over the BPB to prevent unintended BIOS modifications.

#### Detailed Functionality

- **Segment Initialization:** Disables interrupts and initializes segment registers for a flat memory model.
- **Stack Initialization:** Sets up a basic stack to facilitate subroutine calls.
- **Protected Mode Entry:** Loads the GDT and switches the CPU to 32-bit protected mode.
- **Kernel Loading:** Reads the kernel from disk into memory and transfers control to it.

### Kernel (`kernel.asm`, `kernel.c`)

#### Key Features

- **32-bit Protected Mode:** Operates entirely in 32-bit protected mode.
- **Segment Register Setup:** Configures segment registers for protected mode operation.
- **Stack Initialization:** Establishes a 32-bit stack for kernel operations.
- **A20 Line Enable:** Ensures the A20 line is enabled for accessing higher memory.
- **Screen Output:** Includes functionality to write "Hello World!" to the screen, demonstrating basic I/O capabilities.

#### Operation

Upon execution, the kernel sets up the necessary data structures and enters an infinite loop, representing the simplest form of a running system. The `kernel.c` file contains the screen output logic, initializing the VGA display and printing "Hello World!".

### Makefile

- Compiles and links the kernel and bootloader.
- Creates a binary image of the OS by combining the bootloader and kernel.

### Linker Script (`linker.ld`)

- Defines the memory layout for the kernel.
- Sets the entry point and organizes various sections (.text, .rodata, .data, .bss) of the kernel.

### Run Script (`run_os.sh`)

- Automates the process of cleaning, building, and running the OS using QEMU.

## Build Instructions for CuriOS

### Prerequisites

- Ensure you have `make`, NASM (Netwide Assembler), GCC, and QEMU installed on your system for compiling, assembling, and emulating the code.

### Steps

1. **Clean Previous Builds:**
    - Use `make clean` to remove any previous build artifacts.

2. **Compile and Build:**
    - Run `./build.sh` to compile and link the bootloader and kernel.

3. **Run with QEMU:**
    - Execute `./run_os.sh` to build and run CuriOS using QEMU.

#### Notes

- Make sure `build.sh` and `run_os.sh` are executable. Use `chmod +x build.sh run_os.sh` if necessary.
- If you encounter issues, verify that all dependencies are installed and your environment is set up for x86 assembly and C development.
- Adjust paths in the scripts if your directory structure differs.

## Additional Notes

- CuriOS is fundamental, primarily for educational purposes.
- It demonstrates basic OS development concepts like bootloader implementation, mode transitions, low-level memory management, and basic I/O operations.
- This documentation covers the latest updates and is subject to change as CuriOS evolves.
