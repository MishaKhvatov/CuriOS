; Bootloader Setup
ORG 0x7c00                  ; Origin set to 0x7C00, standard for bootloaders
BITS 16                     ; Set operation mode to 16-bit for compatibility

CODE_SEG equ gdt_code - gdt_start  ; Define code segment offset
DATA_SEG equ gdt_data - gdt_start  ; Define data segment offset

; BIOS Parameter Block Avoidance
; Some BIOS modify the bootloader without a valid BPB.
; The following code prevents any damage from such modifications.
boot_entry:
    jmp short main_entry       ; Jumps over the BPB area, to avoid executing it
    nop                        ; No-operation, aligns the next instructions
 
times 33 db 0                 ; Padding to align the code properly

; Main Entry Point
main_entry: 
    jmp 0:initialize_segments ; Jump far to set CS and proceed with segment initialization


; Segment Initialization
initialize_segments:

    cli                        ; Clear interrupt flag to disable interrupts
    ; Set up segment registers with 'flat model' (same base address, no segmentation)
    xor ax, ax                 ; Zero out AX register (XOR-ing a register with itself sets it to 0)
    mov ds, ax                 ; Set DS (Data Segment) to 0
    mov es, ax                 ; Set ES (Extra Segment) to 0

    ; Initialize Stack Segment
    mov ss, ax                 ; Set SS (Stack Segment) to 0
    mov sp, 0x7c00             ; Set SP (Stack Pointer) to the bootloader start address
    ;Get Map of Memory
    call memory_map
    
    sti ;Enable interrupt

; Enter Protected Mode
.enter_protected_mode:
    cli                        ; Clear interrupt flag to disable interrupts
    lgdt [gdt_descriptor]      ; Load Global Descriptor Table
    mov eax, cr0               ; Move contents of CR0 to EAX
    or eax, 0x1                ; Set PE bit in CR0 to enable protected mode
    mov cr0, eax               ; Move updated EAX back to CR0
    jmp CODE_SEG: load_kernel32

;Using INT 0x15, eax= 0xE820 function to get a memory map
memory_map:
.first_call:
    mov di, 0x504 ; Destination buffer, 0x504 -> magic number
    xor ebx, ebx ; Clear EBX
    xor bp, bp  ; BP will be entry count
    
    ;Preparing to call BIOS function
    mov edx, 0x0534D4150 ; ASCII characters for "SMAP" - the bios needs this signature for error checking
    mov eax, 0xE820 ; Function Numbers
    mov [es:di +20], dword 1 ; Sets 'Extended Attributes' field to 0x01 to ensure ACPI compatability
    mov ecx, 24 ;Asking for 24 bytes

    ;Function Call
    int 0x15

    ;Test to see if function call worked
    jc short .failed ; Failed on first call means unsupported function
    test ebx, ebx ; check if ebx = 0 (means list is one entry long, so worthless)
    je short .failed 

    jmp short .loop_start

.loop_start:
    jcxz .skip_entry ;Skips 0 byte entries (ECX is the number of bytes returned by the BIOS )
    cmp cl,20 ;make sure we got a 20 byte entry back
    
    jbe short .no_text
    test byte [es:di +  20],1
    je short .skip_entry
.loop:
    mov eax, 0xE820 ;eax gets trashed on every call
    mov [es:di +20], dword 1
    mov ecx,24
    int 0x15
    jc short .final ;if carry set, end of list reached
    mov edx, 0x0534D4150 ; repaire potentially trashed register

.no_text:
    ;Test for 0 
    mov ecx, [es:di + 8]
    or ecx, [es:di+12]
    jz .skip_entry
    inc bp ;if it is not 0, move to next storage spot
    add di, 24

.skip_entry:
    test ebx, ebx  ;if ebx = 0, list is over
    jne short .loop 
.final:
    mov [0x500],bp ;store the entry count
    clc ;clear carry
    ret
.failed:
    stc
    ret

; Global Descriptor Table (GDT) Definitions
gdt_start:
gdt_null:                     ; Null descriptor as required by x86 architecture
    dd 0x0
    dd 0x0
; Code segment descriptor, offset 0x8
gdt_code:
    dw 0xfff
    dw 0
    db 0
    db 0x9a                    ; Access byte: present, ring 0, code segment
    db 11100111b               ; 4-bit flags, limit 16-19 bits
    db 0
; Data segment descriptor, offset 0x10
gdt_data:
    dw 0xfff
    dw 0
    db 0
    db 0x92                    ; Access byte: present, ring 0, data segment
    db 11100111b
    db 0
gdt_end:

; GDT Descriptor Definition
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Size of GDT
    dd gdt_start               ; Address of GDT

[BITS 32]                  ; Define the code as 32-bit, important for the processor mode.

load_kernel32:
    mov eax, 1             ; EAX = Starting Sector from which to read (sector 1).
    mov ecx, 100           ; ECX = Total number of sectors to read.
    mov edi, 0x0100000     ; EDI = Destination memory address for the data (1 MB mark, where the kernel starts).
    call ata_lba_read      ; Call the ata_lba_read subroutine.
    jmp CODE_SEG:0x0100000   

ata_lba_read:
    mov ebx, eax           ; Backup the original LBA (Logical Block Address) in EBX.

    ; Send the highest 8 bits of the LBA to the hard disk controller.
    shr eax, 24            ; Shift EAX right by 24 bits, moving the highest 8 bits into the lowest 8 bits.
    or eax, 0xE0            ;Selects mater drive
    mov dx, 0x1F6          ; DX = Port address for the highest 8 bits of the LBA.
    out dx, al             ; Output the highest 8 bits to the port.

    ; Send the total sectors to read.
    mov eax, ecx           ; EAX = Total sectors to read.
    mov dx, 0x1F2          ; DX = Port address for sector count.
    out dx, al             ; Output the sector count to the port.

    ; Restore the original LBA and send its lower 24 bits in three steps.
    mov eax, ebx           ; Restore the original LBA.
    
    ; Send the lowest 8 bits of the LBA.
    mov dx, 0x1F3          ; DX = Port address for the lowest 8 bits of the LBA.
    out dx, al             ; Output the lowest 8 bits to the port.

    ; Send the next 8 bits of the LBA.
    mov dx, 0x1F4          ; DX = Port address for the next 8 bits of the LBA.
    mov eax, ebx ; Restore the backup LBA
    shr eax, 8           ; Shift right by 8 bits to position the next part of the LBA.
    out dx, al             ; Output the next 8 bits to the port.

    ; Send the upper-middle 8 bits of the LBA.
    mov dx, 0x1F5          ; DX = Port address for the upper-middle 8 bits of the LBA.
    mov eax, ebx ; Restore the backup LBA
    shr eax, 16             ; Shift right again by 8 bits.
    out dx, al             ; Output the upper-middle 8 bits to the port.

    ; Send the command to read sectors.
    mov dx, 0x1F7          ; DX = Command port address.
    mov al, 0x20           ; AL = Read sectors command.
    out dx, al             ; Output the command to the port.

    ; Read all requested sectors into memory.
.next_sector:
    push ecx               ; Save ECX (total sectors to read) on the stack.
.try_again: 
    mov dx, 0x1F7          ; DX = Status port address.
    in al, dx              ; Read the status from the port.
    test al, 8          ; Test if the DRQ (Data Request) bit is set.
    jz .try_again          ; If not set, loop until it is.

    mov ecx, 256           ; ECX = Words to read per sector (512 bytes/2 bytes per word).
    mov dx, 0x1F0          ; DX = Data port address.
    rep insw               ; Read words from the port and store them at ES:EDI, auto-incrementing EDI.
    pop ecx                ; Restore the original value of ECX.
    loop .next_sector       ; If there are more sectors to read, repeat.
    ret                    ; Return from the subroutine.


; Bootloader Padding and Magic Number
times 510 - ($ - $$) db 0 ; Pad the bootloader to 510 bytes
dw 0xAA55 ; Boot signature (Magic number)


; This bootloader sets up the CPU to enter 32-bit protected mode. It starts in 16-bit real mode as required by the BIOS.
; The GDT is set up with a flat memory model, meaning all segments effectively point to the same memory range.
; This simplifies memory management in the early stage of the bootloader.
; The code then transitions the CPU into 32-bit protected mode, which allows access to more memory and enables advanced CPU features.
; The infinite loop at the end of 'enter_32_bit' prevents the CPU from executing beyond the bootloader's code.
; The magic number 0xAA55 at the end of the bootloader is required by the BIOS to recognize the bootloader.

; Additional Note: In the transition to protected mode, segment selectors are simplified for the bootloader context. 
; In a more complex OS environment, segment selectors would include proper TI and RPL bits for finer control over segment access and privilege levels.
