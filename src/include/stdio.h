/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <sys/types.h>

int vsprintf(char *, const char *, va_list);
int sprintf(char *, const char *, ...);
