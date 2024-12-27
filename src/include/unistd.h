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

#define STDIN_FILENO            0
#define STDOUT_FILENO           1
#define STDERR_FILENO           2

extern char **environ;
void _exit(int);
pid_t fork(void);
pid_t vfork(void);
int sched_yield(void);

int execl(const char *, const char *, ...);
int execv(const char *, char *const[]);
int execle(const char *, const char *, ...);
int execve(const char *, char *const[], char *const[]);
int execlp(const char *, const char *, ...);
int execvp(const char *, char *const[]);

pid_t getpid(void);
pid_t gettid(void);
uid_t getuid(void);
uid_t getgid(void);
int close(int);
ssize_t read(int, void *, size_t);
ssize_t write(int, const void *, size_t);
int brk(void *);
void *sbrk(intptr_t);
off_t lseek(int, off_t, int);

int chdir(const char *);
char *getcwd(char *, size_t);
char *getwd(char *);

int setenv(const char *, const char *, int overwrite);
int unsetenv(const char *);
char *getenv(const char *);
char *secure_getenv(const char *);

char *ttyname(int);
int ttyname_r(int, char *, size_t);

extern char *optarg;
extern int optind, opterr, optopt;
int getopt(int, char * const[], const char *);
