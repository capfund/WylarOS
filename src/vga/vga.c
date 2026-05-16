#include "vga.h"

/* VGA text mode buffer address */
static uint16_t *vga_buffer = (uint16_t *)0xB8000;

/* Current cursor position */
static size_t vga_row = 0;
static size_t vga_col = 0;

/* Create a VGA entry (character + attribute byte) */
static inline uint16_t vga_entry(unsigned char c, uint8_t fg, uint8_t bg) {
    return (uint16_t)c | ((uint16_t)(fg | (bg << 4)) << 8);
}

/* Clear the VGA screen */
void vga_clear(void) {
    uint16_t blank = vga_entry(' ', VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = blank;
    }
    vga_row = 0;
    vga_col = 0;
}

/* Scroll screen up by one line */
static void vga_scroll(void) {
    for (size_t i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
        vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
    }
    
    uint16_t blank = vga_entry(' ', VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    for (size_t i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = blank;
    }
    
    vga_row--;
}

/* Start a new line */
void vga_newline(void) {
    vga_col = 0;
    if (++vga_row == VGA_HEIGHT) {
        vga_scroll();
    }
}

/* Put a single character */
void vga_putchar(char c, uint8_t fg, uint8_t bg) {
    if (c == '\n') {
        vga_newline();
        return;
    }
    
    size_t idx = vga_row * VGA_WIDTH + vga_col;
    vga_buffer[idx] = vga_entry(c, fg, bg);
    
    if (++vga_col == VGA_WIDTH) {
        vga_newline();
    }
}

/* Print a string */
void vga_print(const char *str, uint8_t fg, uint8_t bg) {
    for (size_t i = 0; str[i]; i++) {
        vga_putchar(str[i], fg, bg);
    }
}
