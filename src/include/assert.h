/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#ifdef NDEBUG
#define assert(expr) ((void) 0)
#else
#define assert(expr) __assert((long) expr, #expr, __FILE__, __LINE__, __func__)
#endif

void __assert(long, const char *, const char *, int, const char *);
