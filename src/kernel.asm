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
    mov ebp, 0x200000          ; Base pointer set to a high memory address
    mov esp, ebp               ; Stack pointer set to the base pointer address
   
   ;Sets A20 line-
   in al, 0x92
   or al, 2
   out 0x92, al
   
   call kernel_main
   
   jmp $ ; Infinite loop to halt CPU
times 512 - ($ - $$) db 0 ; For C-compiler allignment issues

