// header file for libc.c

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char val);

char *strcat(char *dest, const char *src);
char *strcpy(char *dest, const char *src);