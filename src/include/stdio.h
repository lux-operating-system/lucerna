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
int vsnprintf(char *, size_t, const char *, va_list);
int snprintf(char *, size_t, const char *, ...);
int vfprintf(FILE *, const char *, va_list);
int fprintf(FILE *, const char *, ...);
int vprintf(const char *, va_list);
int printf(const char *, ...);
int puts(const char *);
int fputs(const char *, FILE *);
int fputc(int, FILE *);
int putc(int, FILE *);
int putchar(int);
int getc(FILE *);
int fgetc(FILE *);
size_t fwrite(const void *, size_t, size_t, FILE *);
size_t fread(void *, size_t, size_t, FILE *);
ssize_t getdelim(char **, size_t *, int, FILE *);
ssize_t getline(char **, size_t *, FILE *);

FILE *fopen(const char *, const char *);
int fclose(FILE *);
