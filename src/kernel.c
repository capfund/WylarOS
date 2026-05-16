#include <stdint.h>
#include <stdbool.h>
#include "vga/vga.h"
#include "gdt/gdt.h"
#include "idt/idt.h"

void kernel_main() {
    vga_clear();
    i686_GDT_Initialize();
    i686_IDT_Initialize();
    i686_IDT_EnableGate(0); // Enable divide by zero exception for testing
    vga_print("Hello, World!", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}