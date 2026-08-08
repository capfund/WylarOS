#include <stdint.h>
#include <stdbool.h>
#include "../vga/vga.h"
#include "../gdt/gdt.h"
#include "../idt/idt.h"
#include "../pic/pic.h"
#include "../util/logger.h"
#include "../isr/pit.h"
#include "../isr/isr.h"
#include "../libc/libc.h"

//extern void int32(); // irq 0 (timer)

void divide_by_zero_handler() {
    log_c("Divide by zero!", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    while (1) {
        // Halt the CPU to prevent further execution
        __asm__ volatile ("hlt");
    }
}

/*void timer_handler() {
    log_c("(Beats xD)", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    pic_eoi();
}*/

void kernel_main() {
    vga_clear();
    i686_GDT_Initialize();
    i686_IDT_Initialize();
    log("Kernel initialized successfully!");

    i686_IDT_EnableGate(0); // testing: enable div by 0 exception
    log("Hello, World!\n");
    i686_IDT_SetGate(0, &divide_by_zero_handler, i686_GDT_CODE_SEGMENT, 
                 IDT_FLAG_GATE_32BIT_INT | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    

    pic_remap(0x20, 0x28); // 32, 40 (offsets)
    log("PIC remapped successfully!");

    i686_IDT_EnableGate(32); // IRQ 0: PIT timer
    log("IRQ0 enabled, test.");
    i686_IDT_SetGate(32, &int32, i686_GDT_CODE_SEGMENT, 
                 IDT_FLAG_GATE_32BIT_INT | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    pit_init(4); // attempt pit init
    pic_unmask(0);
    asm volatile ("sti");

    uint32_t previous_tick = 0;

    while (1) {
        if (ticks % 50 == 0 && ticks != previous_tick) {
            log_c("Tick!", VGA_COLOR_WHITE, VGA_COLOR_BLACK);

            char buf[12];
            itoa(ticks, buf, 10);
            log_c(buf, VGA_COLOR_WHITE, VGA_COLOR_BLACK);
            previous_tick = ticks;
        }
    }
}