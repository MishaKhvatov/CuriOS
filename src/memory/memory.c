#include "memory.h"

/*
 * Sets the first 'size' bytes of the block of memory pointed by 'ptr' 
 * to the specified value (interpreted as an unsigned char).
*/
void* memset(void * ptr, int c, size_t size) {
    // Cast the pointer to a char pointer, as we are dealing with bytes
    char* c_ptr = (char*) ptr;

    // Loop through each byte of the memory block and set it to 'c'
    for (size_t i = 0; i < size; i++) {
        c_ptr[i] = (char) c;
    }

    // Return the original pointer
    return ptr;
}

void parseMemoryMap(MemoryMapEntry *map) {

    //Note: 0x500 and 0x504 are magic numbers where the bootloader loads the memory map too
    // Pointer to the BIOS memory map.
    const uint8_t *biosMemMap = (const uint8_t *)0x504;
    // Number of entries in the memory map.
    uint32_t numEntries = *((uint32_t *)0x500);

    // Iterate over each entry in the BIOS memory map.
    for ( int i = 0; i < numEntries && i < MAX_ENTRIES; ++i) {
        const uint8_t *entryPtr = biosMemMap + i * 24; // Each entry is 24 bytes.

        // Read the base address, length, type, and extended attributes.
        map[i].baseAddr = *((uint64_t *)(entryPtr));
        map[i].length = *((uint64_t *)(entryPtr + 8));
        
        // Ignore entry if length is 0.
        if (map[i].length == 0) {
            continue;
        }

        map[i].type = *((uint32_t *)(entryPtr + 16));
        map[i].extendedAttributes = *((uint32_t *)(entryPtr + 20));

        // Check Extended Attributes bit 0.
        if ((map[i].extendedAttributes & 1) == 0) {
            // Ignore this entry as per Extended Attributes bit 0.
            continue;
        }
    }
}