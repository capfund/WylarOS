#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void log(const char* message);
void log_c(const char* message, uint8_t fgColor, uint8_t bgColor);