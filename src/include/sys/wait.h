/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <sys/types.h>

// exit status
#define _EXIT_NORMAL            0x100
#define _EXIT_SIGNALED          0x200

// waitpid() flags
#define WNOHANG                 0x02
#define WUNTRACED               0x04

// waitpid() status macros
#define WIFEXITED(status)       (status & _EXIT_NORMAL)
#define WEXITSTATUS(status)     ((int8_t) status & 0xFF)
#define WIFSIGNALED(status)     (status & _EXIT_SIGNALED)
#define WTERMSIG(status)        (0)     /* todo */
#define WIFSTOPPED(status)      (0)
#define WSTOPSIG(status)        (0)

pid_t wait(int *);
pid_t waitpid(pid_t, int *, int);
