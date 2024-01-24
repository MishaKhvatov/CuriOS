#include <stdint.h>
void idt_init();
extern void enable_interrupts();
extern void disable_interrupts();
typedef struct{
    uint16_t offset_low; //low part of the address
    uint16_t segment; //segment selector
    uint8_t reserved;
    uint8_t flags;
    uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idtr_t;
