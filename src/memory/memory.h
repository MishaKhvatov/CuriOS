#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>
// Define a structure to represent a memory map entry.
typedef struct {
    uint64_t baseAddr;
    uint64_t length;
    uint32_t type;
    uint32_t extendedAttributes;
} MemoryMapEntry;

#define MAX_ENTRIES 128

// Function to parse the memory map.
void parseMemoryMap(MemoryMapEntry *map);
void* memset(void* ptr, int c , size_t size);
#endif