#include "../vga/vga.h"
#include "logger.h"
#include "../libc/libc.h"

void log_c(const char* message, uint8_t fgColor, uint8_t bgColor)
{
    char buffer[256];
    strcpy(buffer, "LOGGER: ");
    strcat(buffer, message);
    strcat(buffer, "\n");
    vga_print(buffer, fgColor, bgColor);
}

void log(const char* message)
{
    log_c(message, VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}