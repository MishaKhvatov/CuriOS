#include "config.h"
#include "kheap.h"
#include "kernel.h"
#include "memory/memory.h"
static heap_table_t kheap_table;
static heap_t kheap;

void kheap_init(){
    kheap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*) (CURIOS_HEAP_TABLE_ADDRESS);
    kheap_table.count = CURIOS_HEAP_SIZE_BYTES / CURIOS_HEAP_BLOCK_SIZE;
    int res = create_heap(&kheap, &kheap_table, (void*)CURIOS_HEAP_ADDRESS, CURIOS_HEAP_SIZE_BYTES);
    if(res < 0){
        print("Failed to create heap\n");
    }   
}

void* kmalloc(size_t size){
    return heap_malloc(&kheap, size);
}
void* kzalloc(size_t size){
    void* ptr = kmalloc(size);
    if(!ptr) return NULL;
    memset(ptr, 0, size);
    return ptr;
}
void kfree(void* ptr)
{
    heap_free(&kheap, ptr);
}