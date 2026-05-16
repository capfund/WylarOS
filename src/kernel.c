#include <stdint.h>
#include <stdbool.h>
#include "vga.h"

void kernel_main() {
    vga_clear();
    vga_print("Hello, World!", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}