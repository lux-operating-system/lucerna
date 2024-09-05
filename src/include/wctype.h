/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <stdint.h>

typedef uint32_t wchar_t;
typedef uint64_t wint_t;
typedef int wctype_t;
typedef int wctrans_t;

#define WCHAR_MIN           0
#define WCHAR_MAX           99999
