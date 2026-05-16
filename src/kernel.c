#include <stdint.h>
#include <stdbool.h>
#include "vga/vga.h"
#include "gdt/gdt.h"

void kernel_main() {
    vga_clear();
    vga_print("HI!\n", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    i686_GDT_Initialize();
    vga_print("Hello, World!", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}