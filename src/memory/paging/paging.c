#include "paging.h"
#include "memory/heap/kheap.h"
#include "status.h"
extern void paging_load_directory(uint32_t* directory);

static uint32_t* current_directory = 0;

//Returns Pointer to linearly-initlized Page Directory
// Initializes and returns a pointer to a linearly-initialized page directory for a 4GB address space
uint32_t* paging_create_directory(uint8_t flags) {
    uint32_t* directory = (uint32_t*) kzalloc(PAGING_TOTAL_PAGE_TABLES * sizeof(uint32_t));
    if (!directory) return NULL;
    
    uint32_t offset = 0;
    for (int i = 0; i < PAGING_TOTAL_PAGE_TABLES; i++) {
        uint32_t* page_table = (uint32_t*) kzalloc(PAGING_TOTAL_ENTRIES_PER_TABLE * sizeof(uint32_t));
        if (!page_table){
             // Free previously allocated page tables
            for (int j = 0; j < i; j++) {
                kfree((uint32_t*)(directory[j] & 0xFFFFF000));
            }
            kfree(directory);
            return NULL;
        }

        for (int j = 0; j < PAGING_TOTAL_ENTRIES_PER_TABLE; j++) {
            uint32_t frame_address = offset + j * PAGING_PAGE_SIZE;
            page_table[j] = frame_address | flags;
        }

        offset += PAGING_PAGE_SIZE * PAGING_TOTAL_ENTRIES_PER_TABLE;
        directory[i] = (uint32_t) page_table | flags;
    }
    return directory;
}

void paging_switch_directory(uint32_t* directory){
    paging_load_directory(directory);
    current_directory = directory;
}

uint32_t get_page_directory_index(void* virtual_address){
    uint32_t v_a = (uint32_t) virtual_address;
    return v_a / (PAGING_PAGE_SIZE * PAGING_TOTAL_ENTRIES_PER_TABLE) ; 
}

int paging_is_aligned(void* address){
    int ret = !((uint32_t)address % PAGING_PAGE_SIZE);
    return ret;
}

int page_set(uint32_t* directory , void* virtual_address, uint32_t physical_address, uint8_t flags){
    //Check that the addresses are alligned 
    //It is up to the caller to provide valid addresses, as NULL = 0 is a valid address, even if it is the result of a kmalloc error
    // We don't check for frame-alignment of physical_address , instead aligning it ourselves
    int res = 0;
    if(!paging_is_aligned(virtual_address) || !directory){
        res = -EINVARG;
        return res;
    }
    uint32_t v_a = (uint32_t) virtual_address;

    int directory_index = get_page_directory_index(virtual_address); //which page table it belongs too
    int page_index = (v_a % (PAGING_PAGE_SIZE * PAGING_TOTAL_ENTRIES_PER_TABLE)) / PAGING_PAGE_SIZE;
    uint32_t*  page_table = (uint32_t*) (directory[directory_index] & 0xFFFFF000); //Get address

    uint32_t frame_address = physical_address & ~0xFFF;
    page_table[page_index] = frame_address | flags;

    return res;
}