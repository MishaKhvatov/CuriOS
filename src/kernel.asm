[BITS 32]
global _start
extern kernel_main
CODE_SEG equ 0x08
DATA_SEG equ 0x10


; Transition to 32-bit Protected Mode
[BITS 32]
_start:
    ; Update segment registers for 32-bit mode
    ; Explanation: In real mode, these registers hold the offset of the segments. In protected mode, they hold segment selectors.
    ; Here, we are using a simplified setup for segment selectors where the TI and RPL bits are set to their default values.
    ; TI bit is 0 (pointing to GDT) and RPL bits are 00 (ring 0, highest privilege).
    ; This is sufficient for the bootloader's purpose as it operates in a high privilege mode and uses a flat memory model.

    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax 
    mov gs, ax
    mov ss, ax
    ; Initialize 32-bit stack
    mov ebp, 0x700000          ; Base pointer set to a high memory address
    mov esp, ebp               ; Stack pointer set to the base pointer address
   
   ;Sets A20 line-
   in al, 0x92
   or al, 2
   out 0x92, al

   ; PIC Re-map
; The following code remaps the IRQs of the PICs. The IRQs 0-7 are handled by PIC1,
; and IRQs 8-15 are handled by PIC2. By default, the PICs are mapped to interrupts
; 0x08-0x0F (PIC1) and 0x70-0x77 (PIC2), which conflict with the CPU exceptions.
; The code remaps PIC1 to start at 0x20 (32) and PIC2 to start at 0x28 (40).

    mov al, 0x11
    out 0x20, al     ; Send ICW1 to PIC1: Initialize and require ICW4
    out 0xA0, al     ; Send ICW1 to PIC2: Same as above

    mov al, 0x20
    out 0x21, al     ; Send ICW2 to PIC1: Remap PIC1 to start at interrupt 32 (0x20)
    mov al, 0x28
    out 0xA1, al     ; Send ICW2 to PIC2: Remap PIC2 to start at interrupt 40 (0x28)

    mov al, 0x04
    out 0x21, al     ; Send ICW3 to PIC1: Setup PIC1 to acknowledge that PIC2 is at IRQ2
    mov al, 0x02
    out 0xA1, al     ; Send ICW3 to PIC2: Setup PIC2 to acknowledge it is connected to IRQ2 of PIC1

    mov al, 0x01
    out 0x21, al     ; Send ICW4 to PIC1: Set PIC1 in 8086/88 (MCS-80/85) mode
    out 0xA1, al     ; Send ICW4 to PIC2: Same as above for PIC2. PICs are now remapped and ready.


   call kernel_main
   jmp $ ; Infinite loop to halt CPU
times 512 - ($ - $$) db 0 ; For C-compiler allignment issues

