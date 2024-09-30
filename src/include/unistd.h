/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <sys/types.h>

#define _POSIX_VERSION              200112L
#define _POSIX2_VERSION             200112L

#define SEEK_SET                1
#define SEEK_CUR                2
#define SEEK_END                3

extern char **environ;
void exit(int);
void _exit(int);
pid_t fork(void);
pid_t vfork(void);
int sched_yield(void);

int execl(const char *, const char *, ...);
int execv(const char *, const char **);
int execle(const char *, const char *, ...);
int execve(const char *, const char **, const char **);
int execlp(const char *, const char *, ...);
int execvp(const char *, const char **);

pid_t getpid(void);
pid_t gettid(void);
uid_t getuid(void);
uid_t getgid(void);
int close(int);
ssize_t read(int, void *, size_t);
ssize_t write(int, const void *, size_t);
void *sbrk(intptr_t);
off_t lseek(int, off_t, int);

