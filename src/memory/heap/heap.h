#ifndef HEAP_H
#define HEAP_H
#include "stdint.h"
#include <stddef.h>

void heap_init();
typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

typedef struct heap_table{
    HEAP_BLOCK_TABLE_ENTRY* entries; 
    size_t count;
} heap_table_t;

typedef struct heap {
    heap_table_t* table;
    void* saddr;
} heap_t;

#define HEAP_TABLE_ENTRY_TAKEN 0x01
#define HEAP_TABLE_ENTRY_FREE 0x00

#define HEAP_TABLE_ENTRY_HAS_NEXT 0b10000000
#define HEAP_TABLE_ENTRY_IS_FIRST 0b01000000

int create_heap (heap_t *heap, heap_table_t* table , void* start, size_t size);
void* heap_malloc(heap_t* heap, size_t size);
void heap_free(heap_t* heap, void* ptr);

#endif
