#ifndef PAGE_H
#define PAGE_H

#include <stdint.h>
#include <stddef.h>

#define PAGING_CACHE_DISABLED   0b00010000
#define PAGING_WRITE_THROUGH    0b00001000
#define PAGING_ACCESS_FROM_ALL  0b00000100
#define PAGING_IS_WRITEABLE     0b00000010
#define PAGING_IS_PRESENT       0b00000001

#define PAGING_TOTAL_ENTRIES_PER_TABLE 1024
#define PAGING_TOTAL_PAGE_TABLES 1024
#define PAGING_PAGE_SIZE 4096

extern void enable_paging();
uint32_t* paging_create_directory(uint8_t flags);
int page_set(uint32_t* directory , void* virtual_address, uint32_t physical_address, uint8_t flags);
void paging_switch_directory(uint32_t* directory);


#endif