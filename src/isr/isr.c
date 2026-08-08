#include "isr.h"
#include "pit.h"
#include "../libc/libc.h"
#include "../pic/pic.h"
#include "../util/logger.h"
#include "../vga/vga.h"

volatile uint32_t ticks = 0;

// enable and connect to stub
void isr_pit() {
    ticks++;
    pic_eoi();
}