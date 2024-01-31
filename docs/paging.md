# Paging Implementation Documentation

This documentation provides an overview and detailed information about the paging implementation in a system, as defined in `paging.h`, `paging.c`, and `paging.asm`. This implementation is designed for managing virtual memory in a system.

## Overview

Paging is a memory management scheme that eliminates the need for contiguous allocation of physical memory. This implementation allows the system to use a paging mechanism for virtual memory, enabling efficient and flexible memory management. It is specifically designed for a 32-bit address space.

## Files and Key Functions

### 1. paging.h

This header file defines constants, function prototypes, and structures used in the paging implementation.

#### Constants
- `PAGING_CACHE_DISABLED`, `PAGING_WRITE_THROUGH`, etc.: Flags for paging behavior.
- `PAGING_TOTAL_ENTRIES_PER_TABLE`: Number of entries per page table.
- `PAGING_TOTAL_PAGE_TABLES`: Number of page tables.
- `PAGING_PAGE_SIZE`: The size of a single page.

#### Function Prototypes
- `enable_paging()`: Enables paging mode.
- `paging_create_directory(uint8_t flags)`: Creates a new page directory with specified flags.
- `page_set(uint32_t* directory, void* virtual_address, uint32_t physical_address, uint8_t flags)`: Sets a page with a given virtual address to a specific physical address.
- `paging_switch_directory(uint32_t* directory)`: Switches the current page directory.

### 2. paging.c

This source file implements the functions declared in `paging.h`.

#### Key Functions
- `paging_create_directory()`: Allocates and initializes a page directory and corresponding page tables for a 4GB address space.
- `paging_switch_directory()`: Loads a given page directory into the CR3 register and updates the current directory.
- `page_set()`: Maps a virtual address to a physical address in the page table.
- Auxiliary functions like `get_page_directory_index()` and `paging_is_aligned()` assist in managing addresses.

### 3. paging.asm

This assembly file contains the low-level functions to interact with the processor's paging features.

#### Assembly Functions
- `paging_load_directory`: Loads the page directory base address into the CR3 control register.
- `enable_paging`: Enables paging by setting the appropriate bit in the CR0 control register.

## Usage

1. **Initialization**:
   - Call `paging_create_directory()` to initialize the paging structures.
   - Use `paging_switch_directory()` to activate the newly created directory.

2. **Setting Pages**:
   - Use `page_set()` to map virtual addresses to physical addresses. Ensure addresses are page-aligned.

3. **Enabling Paging**:
   - Invoke `enable_paging()` to turn on paging in the system.

## Notes

- The implementation assumes a 4GB address space, divided into pages of 4KB each.
- Proper alignment of addresses is crucial for the correct functioning of the paging system.
- It's the caller's responsibility to provide valid addresses to `page_set()`.
- The physical address alignment is handled internally in `page_set()`.
- Error handling is implemented for memory allocation failures.

This documentation provides a comprehensive guide to understanding and using the paging implementation in your system. For further customization or specific use cases, modifications to the source code might be required.
