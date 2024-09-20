/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <stddef.h>

#define OCTAL       8
#define DECIMAL     10
#define HEX         16

void *malloc(size_t);
void free(void *);
void *calloc(size_t, size_t);
void *realloc(void *, size_t);

char *itoa(int, char *, int);
int atoi(const char *);
char *ltoa(long, char *, int);
long atol(const char *);

void exit(int);