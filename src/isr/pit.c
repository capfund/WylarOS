#include "pit.h"
#include "../libc/libc.h"

#define PIT_COMMAND 0x43
#define PIT_CHANNEL0 0x40

void pit_init(uint32_t frequency) {
    uint32_t divisor = 1193182 / frequency;

    outb(PIT_COMMAND, 0x36);

    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}