/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

void perror(const char *s) {
    if(!s || !s[0]) fprintf(stderr, "%s\n", strerror(errno));
    else fprintf(stderr, "%s: %s\n", s, strerror(errno));
}