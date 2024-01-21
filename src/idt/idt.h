#ifndef IDT_H
#define IDT_H
#include <stdint.h> 

// Structure defining an individual interrupt descriptor.
struct idt_desc {
    uint16_t offset_1; // Lower 16 bits of the interrupt handler's address
    uint16_t selector; // Code segment selector in GDT or LDT
    uint8_t zero;      // This byte is always set to zero
    uint8_t type_attr; // Type and attributes; see below for details
    uint16_t offset_2; // Higher 16 bits of the interrupt handler's address
} __attribute__((packed));

// Structure defining the IDTR (Interrupt Descriptor Table Register) descriptor.
struct idtr_desc {
    uint16_t limit; // Size of the IDT - 1 (since IDT size cannot be zero)
    uint32_t base;  // Base address of the start of the IDT
} __attribute__((packed));

void idt_init();
#endif
