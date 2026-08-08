#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

extern void int32();

void isr_pit();
extern volatile uint32_t ticks;
