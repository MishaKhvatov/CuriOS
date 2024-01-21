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
