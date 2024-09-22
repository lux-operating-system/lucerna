/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <stdlib.h>
#include <errno.h>

/* Default empty functions for systems that do not implement Unix-style
 * pseudo-terminals, in which case these functions are irrelevant */

#ifndef __LUCERNA_PTY

char *ptsname(int fd) {
    errno = ENOSYS;
    return NULL;
}

int ptsname_r(int fd, char *buf, size_t bufsz) {
    errno = ENOSYS;
    return ENOSYS;
}

int grantpt(int fd) {
    errno = ENOSYS;
    return -1;
}

int unlockpt(int fd) {
    errno = ENOSYS;
    return -1;
}

#endif
