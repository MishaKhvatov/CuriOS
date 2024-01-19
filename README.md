# PathOS Documentation

## Overview

PathOS is a simple educational and hobbyist operating system kernel designed to demonstrate fundamental OS concepts and provide a platform for learning low-level system programming. It starts from a basic bootloader, enters 32-bit protected mode, and sets up a minimal runtime environment.

## Components

- **Bootloader (`boot.asm`):** Initializes the system and loads the kernel into memory.
- **Kernel (`kernel.asm`):** Represents the core part of the operating system, managing system resources and providing basic services.
- **Makefile:** Automates the compilation and linking process.
- **Linker Script (`linker.ld`):** Defines the memory layout of the kernel.

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

### Kernel (`kernel.asm`)

#### Key Features

- **32-bit Protected Mode:** Operates entirely in 32-bit protected mode.
- **Segment Register Setup:** Configures segment registers for protected mode operation.
- **Stack Initialization:** Establishes a 32-bit stack for kernel operations.
- **A20 Line Enable:** Ensures the A20 line is enabled for accessing higher memory.

#### Operation

Upon execution, the kernel sets up the necessary data structures and enters an infinite loop, representing the simplest form of a running system.

### Makefile

- Compiles and links the kernel and bootloader.
- Creates a binary image of the OS by combining the bootloader and kernel.

### Linker Script (`linker.ld`)

- Defines the memory layout for the kernel.
- Sets the entry point and organizes various sections (.text, .rodata, .data, .bss) of the kernel.

## Build Instructions for PathOS

### Prerequisites

- Ensure you have `make`, NASM (Netwide Assembler), and GCC installed on your system for compiling and assembling the code.
- Install QEMU for emulating the x86 architecture.

### Steps

1. **Clean Previous Builds:**
    - Before starting a new build, it's good practice to clean up any previous build artifacts to ensure a fresh start.
    - Run the following command:
      ```sh
      make clean
      ```

2. **Compile and Build:**
    - Use the provided build script to compile and link the bootloader and kernel.
    - Run the build script using:
      ```sh
      ./build.sh
      ```
    - This script will invoke the `make` command to assemble the bootloader and kernel, and link them together into a single binary.

3. **Run with QEMU:**
    - Once the build is complete, you can run PathOS using QEMU to emulate an x86 system.
    - Use the following command to start the emulation:
      ```sh
      qemu-system-x86_64 -hda ./bin/os.bin
      ```
    - This command tells QEMU to emulate an x86_64 system and use the `os.bin` file as the hard disk image.

#### Notes

- The `build.sh` script should be present in your project directory and executable. If it's not executable, run `chmod +x build.sh` to make it so.
- If you encounter any issues, ensure all dependencies are correctly installed and your environment is set up for x86 assembly and C development.
- The QEMU command provided assumes that the final OS binary is located at `./bin/os.bin`. Adjust the path accordingly if your setup differs.

## Additional Notes

- The current state of PathOS is fundamental, primarily for educational purposes.
- It demonstrates basic OS development concepts like bootloader implementation, mode transitions, and low-level memory management.
- This documentation covers the initial commit and is subject to change as PathOS evolves.
