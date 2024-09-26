/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 *
 * Platform-Specific Code for x86_64 lux
 */

#pragma once

#include <sys/types.h>

typedef struct {
    char name[256];     // device name
    char driver[256];   // name of server/driver prefixed with "lux:///ks"
    int kernel;         // 1 for kernel-level IRQ handler, 0 for user space
    uintptr_t khandler; // entry point for kernel IRQ handlers
    int level, high;    // trigger mode
} IRQHandler;

int execrdv(const char *, const char **);
int irq(int, IRQHandler *);
uintptr_t mmio(uintptr_t, off_t, int);
uintptr_t pcontig(uintptr_t, off_t, int);
