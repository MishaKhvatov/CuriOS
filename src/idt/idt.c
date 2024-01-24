// Include necessary headers
#include "idt.h"
#include "kernel.h"
#include "io/io.h"
#include "config.h"
#include "memory/memory.h"

// IDT (Interrupt Descriptor Table) data structures
__attribute__((aligned(0x10))) 

// IDT Entries
// Note: These should probably be static, but this causes some weird linkage issues.
static idt_entry_t idt[256]; // Array of IDT entries
static idtr_t idtr; // IDT register structure

// External Assembly Functions Declarations
extern void isr_21h();       // ISR for interrupt 0x21
extern void isr_default();    // Default ISR
extern void idt_load();       // Function to load the IDT

// C-wrapper functions called from the ISR (Interrupt Service Routine) routine
void int_21h();      // Handler for interrupt 0x21 (Keyboard Interrupt)
void int_default();  // Default interrupt handler

/**
 * Handles interrupt 0x21 (Keyboard Interrupt).
 */

void int_21h() {
    unsigned char scancode = insb(0x60);

    // Check if the most significant bit (bit 7) is set to determine key release
    if ((scancode & 0x80) == 0) print("Keyboard pressed!\n");
    else print("Keyboard released!\n");
    // Send End of Interrupt signal to PIC
    outb(0x20, 0x20);
}

/**
 * Default interrupt handler.
 */
void int_default() {
    // Send End of Interrupt signal to PIC
    outb(0x20, 0x20);
}

/**
 * Sets an IDT entry.
 * @param vector The interrupt vector.
 * @param isr Pointer to the ISR function.
 * @param flags IDT flags.
 */
void idt_set(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t *gate = &idt[vector];
    gate->offset_low = (uint32_t)isr & 0xFFFF;
    gate->segment = KERNEL_CODE_SELECTOR;
    gate->reserved = 0x00;
    gate->flags = flags;
    gate->offset_high = (uint32_t)isr >> 16;
}

/**
 * Initializes the Interrupt Descriptor Table.
 */
void idt_init() {
    // Initialize IDT to 0
    memset(idt, 0, sizeof(idt));

    // Set the IDT register values
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t)idt;

    // Set default ISR for all vectors except 0x21
    for (int i = 0; i < 255; i++) {
        if (i == 0x21) continue;
        idt_set(i, isr_default, 0xEE);
    }

    // Set ISR for interrupt 0x21
    idt_set(0x21, isr_21h, 0xEE);

    // Load the IDT
    idt_load(&idtr);
}
