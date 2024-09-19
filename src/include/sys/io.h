/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <stdint.h>

int ioperm(uintptr_t, uintptr_t, int);
void outb(uintptr_t, uint8_t);
void outw(uintptr_t, uint16_t);
void outd(uintptr_t, uint32_t);
uint8_t inb(uintptr_t);
uint16_t inw(uintptr_t);
uint32_t ind(uintptr_t);
