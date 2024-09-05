/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <sys/types.h>

#define _POSIX_VERSION              200112L
#define _POSIX2_VERSION             200112L

void exit(int);
void _exit(int);
pid_t fork(void);
pid_t vfork(void);
int sched_yield(void);
pid_t getpid(void);
pid_t gettid(void);
uid_t getuid(void);
uid_t getgid(void);
