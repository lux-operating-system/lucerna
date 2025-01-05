/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <sys/types.h>

struct utimbuf {
    time_t actime;
    time_t modtime;
};

int utime(const char *, const struct utimbuf *);
