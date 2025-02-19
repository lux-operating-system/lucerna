/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <stddef.h>

void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
int memcmp(const void *, const void *, size_t);
void *memset(void *, int, size_t);
size_t strlen(const char *);
char *strcpy(char *, const char *);
char *strncpy(char *, const char *, size_t);
int strcmp(const char *, const char *);
int strncmp(const char *, const char *, size_t);
char *strtok(char *, const char *);
char *strtok_r(char *, const char *, char **);
char *strerror(int);
char *strchr(const char *, int);
char *strrchr(const char *, int);
char *strdup(const char *);
char *strstr(const char *, const char *);
char *strncat(char *, const char *, size_t);
char *strcat(char *, const char *);
