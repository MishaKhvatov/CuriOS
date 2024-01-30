# Kernel Heap Implementation Documentation

This document provides a detailed overview of the heap implementation used in the kernel. It includes descriptions of key functions and structures, and guidelines for usage.

## Overview

The kernel heap implementation is designed to provide dynamic memory allocation capabilities within the kernel. It is an essential component for managing memory in a structured and efficient manner.

## Files

- `heap.c`: Contains the core logic for heap management.
- `kheap.c`: Provides an interface for kernel-level heap operations.
- `boot.asm`: Relevant section for setting up the memory map using BIOS interrupt.

## Data Structures

### `MemoryMapEntry`

- Represents a memory segment as identified by the BIOS during system startup.
- Used by the kernel to understand the system's memory layout.

### `heap_t`

- Represents the heap structure.
- Contains information about the start address of the heap and the associated heap table.

### `heap_table_t`

- Represents the table keeping track of the heap's blocks.
- Contains an array of `HEAP_BLOCK_TABLE_ENTRY` and the count of these entries.

## Functions

### `int create_heap(heap_t *heap, heap_table_t* table, void* start, size_t size)`

- Initializes the heap with given parameters.
- Validates memory range and alignment.
- Returns 0 on success, error code on failure.

### `void* heap_malloc(heap_t* heap, size_t size)`

- Allocates a memory block of the specified size from the heap.
- Searches for contiguous free blocks in the heap table.
- Returns a pointer to the allocated memory or `NULL` if allocation fails.

### `void heap_free(heap_t* heap, void* ptr)`

- Frees a previously allocated memory block.
- Validates the pointer and updates the heap table.

### `void kheap_init()`

- Initializes the kernel heap using predefined settings from `config.h`.
- Invokes `create_heap` to set up the kernel heap.

### `void* kmalloc(size_t size)`

- Allocates memory in the kernel heap.
- Wrapper around `heap_malloc`.

### `void kfree(void* ptr)`

- Frees memory allocated with `kmalloc`.
- Wrapper around `heap_free`.

## BIOS Memory Map

- The BIOS-provided memory map is used during the kernel startup to understand the system's memory layout.
- Memory segments are identified using the INT 0x15, eax=0xE820 BIOS interrupt.

## Error Codes

- `-EINVARG`: Invalid argument or validation failure.

## Notes

- The heap implementation assumes the kernel occupies a specific memory range and avoids this range for heap allocation.
- The kernel should call `kheap_init` during its initialization phase to set up the heap.

## Usage

To use the kernel heap, first ensure that `kheap_init` is called during kernel initialization. Afterwards, `kmalloc` and `kfree` can be used for dynamic memory allocation within the kernel.


