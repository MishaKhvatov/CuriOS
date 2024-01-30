#include "kernel.h"
#include <stdint.h> // Standard integer types.
#include <stddef.h> // Standard definition types.
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
// VGA text mode video memory start address.
uint16_t* video_mem = 0;

// Currentsition in the terminal.
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

// Create a VGA character with a specific color.
uint16_t terminal_make_char(char c, char color) {
    // VGA characters are 16 bits: high 8 bits for color, low 8 bits for ASCII character.
    return (color << 8) | c;
}

// Put a character at a specific position on the terminal.
void terminal_putchar(int x, int y, char c, char color) {
    // The VGA text mode memory is a grid of 80 columns by 25 rows.
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, color);
}

// Write a character to the terminal at the current cursor position.
void terminal_writechar(char c, char color) {
    // Newline handling: reset column, advance row.
    if (c == '\n') {
        terminal_col = 0;
        terminal_row += 1;
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, color);
    terminal_col += 1;

    // Handle line wrapping at the end of the row.
    if (terminal_col >= VGA_WIDTH) {
        terminal_col = 0;
        terminal_row += 1;
    }
}

// Initialize the terminal for use.
void terminal_initialize() {
    // Point to the VGA text mode memory.
    video_mem = (uint16_t*)(0xB8000);
    
    // Reset cursor position.
    terminal_col = 0;
    terminal_row = 0;

    // Clear the screen by filling it with spaces.
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_putchar(x, y, ' ', 0);
        }
    } 
}

// Compute the length of a string.
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

// Print a string to the terminal.
void print(const char* str) {
    size_t len = strlen(str);
    for (int i = 0; i < len; i++) {
        terminal_writechar(str[i], 15);
    }
}

// Helper function to print numbers as strings
void print_number(size_t num) {
    char buffer[20]; // Enough to hold any 64-bit number
    int i = 19;
    buffer[i] = '\0';
    i--;

    if (num == 0) {
        buffer[i] = '0';
        i--;
    } else {
        while (num != 0 && i >= 0) {
            buffer[i] = '0' + (num % 10);
            num /= 10;
            i--;
        }
    }

    print(&buffer[i + 1]);
}

// Main function of the kernel.
void kernel_main() {
    terminal_initialize();
    print("Hello World! \n");
    // Initialize the interrupt descriptor table
    idt_init();
    enable_interrupts();
    kheap_init(); 
}