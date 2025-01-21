/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <sys/types.h>

#define PROT_READ               0x01
#define PROT_WRITE              0x02
#define PROT_EXEC               0x04
#define PROT_NONE               0x00

#define MAP_SHARED              0x01
#define MAP_PRIVATE             0x02
#define MAP_FIXED               0x04
#define MAP_ANONYMOUS           0x08
#define MAP_ANON                (MAP_ANONYMOUS)

#define MS_ASYNC                0x01
#define MS_SYNC                 0x02
#define MS_INVALIDATE           0x04

#define MAP_FAILED              ((void *) -1)

void *mmap(void *, size_t, int, int, int, off_t);
int munmap(void *, size_t);
int msync(void *, size_t, int);
