// header file for libc.c

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char val);

char *strcat(char *dest, const char *src);
char *strcpy(char *dest, const char *src);
char* itoa(int value, char* str, int base);