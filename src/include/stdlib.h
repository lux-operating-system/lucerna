/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <stddef.h>

#define OCTAL           8
#define DECIMAL         10
#define HEX             16

#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1

void *malloc(size_t);
void free(void *);
void *calloc(size_t, size_t);
void *realloc(void *, size_t);

char *itoa(int, char *, int);
char *uitoa(unsigned int, char *, int);
int atoi(const char *);
char *ltoa(long, char *, int);
char *ultoa(unsigned long, char *, int);
long atol(const char *);
long long atoll(const char *);

double strtod(const char *, char **);
float strtof(const char *, char **);
long double strtold(const char *, char **);

double atof(const char *);

void exit(int);
void _Exit(int);
int atexit(void (*)(void));
void abort(void);

int posix_openpt(int);
char *ptsname(int);
int ptsname_r(int, char *, size_t);
int grantpt(int);
int unlockpt(int);

int abs(int);

int system(const char *);