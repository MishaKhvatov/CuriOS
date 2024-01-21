#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"
// Array of interrupt descriptors. 
// CURIOS_TOTAL_INTERRUPTS defines the total number of interrupts.
struct idt_desc idt_descriptors[CURIOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

extern void idt_load(struct idtr_desc* ptr);

// Example interrupt handler: Divide by Zero.
void idt_zero() {
    print("Divide by zero error\n");
}

// Function to set an entry in the IDT.
void idt_set(int interrupt_no, void* address) {
    struct idt_desc* desc = &idt_descriptors[interrupt_no];
    desc->offset_1 = (uint32_t) address & 0x0000FFFF; // Lower 16 bits of handler address
    desc->selector = KERNEL_CODE_SELECTOR;           // Segment selector from GDT
    desc->zero = 0x00;                               // Always zero
    desc->type_attr = 0xEE; // Sets type and attributes; here 0xEE is an example setting
    desc->offset_2 = (uint32_t) address >> 16;       // Higher 16 bits of handler address
}

// Initialize the IDT.
void idt_init() {
    memset(idt_descriptors, 0, sizeof(idtr_descriptor));
    idtr_descriptor.limit = sizeof(idt_descriptors) - 1; // Set limit of the IDT
    idtr_descriptor.base = (uint32_t) &idt_descriptors;  // Set base address of the IDT

    idt_set(0, idt_zero); // Set the first interrupt handler

    idt_load(&idtr_descriptor); // Load the IDT
}
