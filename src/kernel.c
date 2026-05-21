#include <stdint.h>
#include <stdbool.h>
#include "vga/vga.h"
#include "gdt/gdt.h"
#include "idt/idt.h"

void divide_by_zero_handler() {
    vga_print("Divide by zero!", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    while (1) {
        // Halt the CPU to prevent further execution
        __asm__ volatile ("hlt");
    }
}

void kernel_main() {
    vga_clear();
    i686_GDT_Initialize();
    i686_IDT_Initialize();
    i686_IDT_EnableGate(0); // testing: enable div by 0 exception
    vga_print("Hello, World!\n", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    i686_IDT_SetGate(0, &divide_by_zero_handler, i686_GDT_CODE_SEGMENT, 
                 IDT_FLAG_GATE_32BIT_INT | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    
    vga_print("Triggering divide by zero exception!\n", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    int a = 1 / 0;
}