section .asm
global idt_load

; C compatible assembly routine for loading the IDT
idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8] ; Move the address of IDTR descriptor to ebx
    lidt [ebx]       ; Load IDT using lidt instruction

    pop ebp
    ret
