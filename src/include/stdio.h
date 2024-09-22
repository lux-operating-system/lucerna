/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <sys/types.h>

#define EOF                 -1

typedef struct {
    int fd;
} FILE;

extern FILE *stdin, *stdout, *stderr;

int vsprintf(char *, const char *, va_list);
int sprintf(char *, const char *, ...);
int vfprintf(FILE *, const char *, va_list);
int fprintf(FILE *, const char *, ...);
int vprintf(const char *, va_list);
int printf(const char *, ...);

FILE *fopen(const char *, const char *);
int fclose(FILE *);
