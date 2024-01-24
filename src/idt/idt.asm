section .asm

; Interrupt Service Routines (ISRs)
; Declare global labels for ISR functions
global isr_21h        ; ISR for keyboard interrupt (interrupt 0x21)
global isr_default    ; Default ISR for other interrupts

; Low-level IDT (Interrupt Descriptor Table) functionality
; Declare global labels for IDT-related functions
global idt_load           ; Function to load the IDT register
global enable_interrupts  ; Function to enable hardware interrupts
global disable_interrupts ; Function to disable hardware interrupts

; External C function declarations
; These functions are defined in C and called from assembly ISRs
extern int_21h     ; External C handler for interrupt 0x21
extern int_default ; External C default interrupt handler

; idt_load:
; Loads the IDT register with the address of the IDT
idt_load:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8] ; Move the address of idtr into ebx
    lidt [ebx]       ; Load IDT register with the address stored in ebx
    pop ebp
    ret

; isr_21h:
; Interrupt Service Routine for keyboard interrupt (interrupt 0x21)
isr_21h:
    pushad      ; Push all general-purpose registers onto the stack
    cli         ; Clear interrupt flag to disable interrupts
    call int_21h; Call the external C handler for interrupt 0x21
    sti         ; Set interrupt flag to enable interrupts
    popad       ; Pop all general-purpose registers from the stack
    iret        ; Return from interrupt

; isr_default:
; Default Interrupt Service Routine for unhandled interrupts
isr_default:
    pushad      ; Push all general-purpose registers onto the stack
    cli         ; Clear interrupt flag to disable interrupts
    call int_default ; Call the external C default interrupt handler
    sti         ; Set interrupt flag to enable interrupts

enable_interrupts:
    sti
    ret
disable_interrupts:
    cli
    ret