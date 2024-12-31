/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

/* POSIX mandates 32 minimum atexit() handlers */
#define MAX_ATEXIT          32

static void (*__atexit[MAX_ATEXIT])(void);
static int __atexitCount = 0;

int atexit(void (*func)(void)) {
    if(__atexitCount >= MAX_ATEXIT) {
        errno = ENOBUFS;
        return errno;
    }

    __atexit[__atexitCount] = func;
    __atexitCount++;
    return 0;
}

void exit(int status) {
    fflush(NULL);

    for(int i = __atexitCount-1; __atexitCount && i >= 0; i--)
        __atexit[i]();

    _exit(status);
}

void _Exit(int status) {
    _exit(status);
}