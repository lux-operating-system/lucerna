/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 *
 * Platform-Specific Code for x86_64 lux
 */

#pragma once

#include <sys/types.h>

#define MMIO_R          0x01    // read perms
#define MMIO_W          0x02    // write perms
#define MMIO_X          0x04    // execute perms
#define MMIO_CD         0x08    // cache disable
#define MMIO_ENABLE     0x80    // create mapping; clear to unmap

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
