#include "heap.h"
#include "./memory/memory.h"
#include "status.h"
#include "config.h"

/*
 * This array 'memoryMap' is used to store entries from the BIOS-provided memory map. Each entry in the array represents 
 * a memory segment as identified by the BIOS during system startup. This array is critical for the kernel to understand 
 * the system's memory layout before taking control.
 *
 * The array is initialized based on the BIOS memory map using the INT 0x15, eax=0xE820 BIOS interrupt. The entries include
 * information such as the base address, length, type, and extended attributes of each memory segment. 
 *
 * Note: This snapshot of the memory map is static and serves as an initial reference for the kernel. It may not 
 * accurately reflect dynamic changes in memory layout. The kernel is assumed to occupy an estimated 1 MB starting from
 * address 0x100000 for safety. This conservative estimate accounts for possible variations in the actual size of the kernel.
 */

MemoryMapEntry memoryMap[MAX_ENTRIES];

static int validate_mem(void* start, size_t size) {
    uint32_t heap_start = (uint32_t)start;
    uint32_t heap_end = heap_start + size;

    // Constants for kernel memory bounds
    const uint32_t KERNEL_START_ADDR = 0x100000;
    const uint32_t KERNEL_END_ADDR = 0x300000;

    // Check for overlap with kernel code
    if ((heap_start < KERNEL_END_ADDR && heap_start >= KERNEL_START_ADDR) || 
        (heap_end > KERNEL_START_ADDR && heap_end <= KERNEL_END_ADDR)) {
        return -EINVARG;
    }

    for (int i = 0; i < MAX_ENTRIES; i++) {
        MemoryMapEntry entry = memoryMap[i];

        // Check if the heap lies within a usable memory segment
        if (entry.type == 1) {
            uint32_t segment_start = entry.baseAddr;
            uint32_t segment_end = segment_start + entry.length;

            if (heap_start >= segment_start && heap_end <= segment_end) {
                return 0; // Valid range
            }
        }
    }

    return -EINVARG; // Invalid range
}

static int validate_table(heap_table_t* table,size_t size){
    //Make sure the heap table alligns 
    size_t total_blocks = table->count;
    if(size != total_blocks * CURIOS_HEAP_BLOCK_SIZE) return -EINVARG;
    return 0;
    
}

static int validate_alignment(void* ptr){
    return ((unsigned int)ptr % CURIOS_HEAP_BLOCK_SIZE) == 0;
}

int create_heap (heap_t *heap, heap_table_t* table , void* start, size_t size) {    
    parseMemoryMap(memoryMap);
    //Validations
    if (!validate_alignment(start) || !validate_alignment( (void*) ( (unsigned int)start + size) ) ){
        return -EINVARG;
    }

    // Validate the memory range and validate table
    int val_mem = validate_mem(start, size);
    if (val_mem != 0) return -val_mem;

    //Validate the table
    int val_table = validate_table(table, size);
    if (val_table != 0) return -val_table;

    //Set the addresses poined to by heap and table to have 0's 
    memset(heap, 0, sizeof(heap_t)); 

    heap->saddr = start;
    heap->table =table;

    size_t table_size = sizeof(HEAP_TABLE_ENTRY_FREE) * table->count;
    memset(table->entries, HEAP_TABLE_ENTRY_FREE , table_size); //Fill in heap_table as initially free

    return 0; // Success
}

void* heap_malloc(heap_t* heap, size_t size){
    size_t table_entries=heap->table->count;
    uint8_t* entry = (uint8_t*) heap->table->entries;
    void* ret = NULL;

    int free_contiguous_blocks= 0; 
    for (int i = 0; i < table_entries; i++)
    {
        // Check if the current block is free
        if (!(entry[i] & HEAP_TABLE_ENTRY_TAKEN)) {
            free_contiguous_blocks++;
        } else {
            free_contiguous_blocks = 0;
        }

         // If we found a contiguous block of the required size
        if (free_contiguous_blocks * CURIOS_HEAP_BLOCK_SIZE >= size) { //note the >= handles the rounding of size
            int start_block = i - free_contiguous_blocks + 1;

            // Marking blocks as taken
            for (int j = start_block; j <= i; j++) {
                entry[j] |= HEAP_TABLE_ENTRY_TAKEN;
                if(j != i) entry[j] |= HEAP_TABLE_ENTRY_HAS_NEXT;
            }

            entry[start_block] |= HEAP_TABLE_ENTRY_IS_FIRST;
            ret = (void*)((uintptr_t)heap->saddr + CURIOS_HEAP_BLOCK_SIZE * start_block);
            break;

    }
    }
     return ret;
}

void heap_free(heap_t* heap, void* ptr) {
    // Validation: Check if the ptr is NULL or outside the heap range
    if (ptr == NULL || (ptr < heap->saddr) || (ptr > (heap->saddr + (heap->table->count * CURIOS_HEAP_BLOCK_SIZE)))) {
        return; // Invalid pointer
    }

    // Calculate the start block index
    int start_block = ((uintptr_t)ptr - (uintptr_t)(heap->saddr)) / CURIOS_HEAP_BLOCK_SIZE;
    uint8_t* entry = (uint8_t*)heap->table->entries;

    // Iterate over the blocks starting from 'start_block' and free them
    for (int i = start_block; ; ++i) {
        // Check if the current block is valid
        if (i >= heap->table->count) {
            break; // Reached the end of the table
        }

        // Check if the current block is part of the allocation
        if (!(entry[i] & HEAP_TABLE_ENTRY_TAKEN)) {
            break; // This block is not part of the allocation
        }

        // Clear the flags for this block
        entry[i] &= ~(HEAP_TABLE_ENTRY_TAKEN | HEAP_TABLE_ENTRY_HAS_NEXT);

        // Check if this is the last block in the allocation
        if (!(entry[i] & HEAP_TABLE_ENTRY_HAS_NEXT)) {
            // Clear the 'IS_FIRST' flag and break the loop
            entry[start_block] &= ~HEAP_TABLE_ENTRY_IS_FIRST;
            break;
        }
    }
}
