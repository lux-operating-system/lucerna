/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>

#define EOF                 -1

#define BUFSIZ              1024
#define _IOFBF              1
#define _IOLBF              2
#define _IONBF              3

typedef struct {
    int fd;
    int eof, error;
    void *mmap;
    size_t mmapLength;
    off_t position;

    int bufferType;
    char buffer[BUFSIZ];
    size_t bufferSize;
} FILE;

extern FILE *stdin, *stdout, *stderr;

int setvbuf(FILE *, char *, int, size_t);

int vsprintf(char *, const char *, va_list);
int sprintf(char *, const char *, ...);
int vsnprintf(char *, size_t, const char *, va_list);
int snprintf(char *, size_t, const char *, ...);
int vfprintf(FILE *, const char *, va_list);
int fprintf(FILE *, const char *, ...);
int vprintf(const char *, va_list);
int printf(const char *, ...);

int vsscanf(const char *, const char *, va_list);
int vfscanf(FILE *, const char *, va_list);
int vscanf(const char *, va_list);
int sscanf(const char *, const char *, ...);
int fscanf(FILE *, const char *, ...);
int scanf(const char *, ...);

int puts(const char *);
int fputs(const char *, FILE *);
int fputc(int, FILE *);
int putc(int, FILE *);
int putchar(int);
int getc(FILE *);
int fgetc(FILE *);
int getchar(void);
int feof(FILE *);
int ferror(FILE *);
size_t fwrite(const void *, size_t, size_t, FILE *);
size_t fread(void *, size_t, size_t, FILE *);
char *fgets(char *, int, FILE *);
char *gets(char *);
ssize_t getdelim(char **, size_t *, int, FILE *);
ssize_t getline(char **, size_t *, FILE *);
int fflush(FILE *);

FILE *fopen(const char *, const char *);
FILE *fdopen(int, const char *);
int fclose(FILE *);
int fseek(FILE *, long, int);
int fseeko(FILE *, off_t, int);
long ftell(FILE *);
off_t ftello(FILE *);
int fileno(FILE *);
void clearerr(FILE *);
void rewind(FILE *);

void perror(const char *);

int remove(const char *);
int rename(const char *, const char *);