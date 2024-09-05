/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 *
 * Platform-Specific Code for x86_64 lux
 */

#pragma once

// lux microkernel system call indexes

#include <stdint.h>

/* Group 1: Processes, Threads, and Users */

#define SYSCALL_EXIT            0
#define SYSCALL_FORK            1
#define SYSCALL_YIELD           2
#define SYSCALL_WAITPID         3
#define SYSCALL_EXECVE          4
#define SYSCALL_EXECRDV         5
#define SYSCALL_GETPID          6
#define SYSCALL_GETTID          7
#define SYSCALL_GETUID          8
#define SYSCALL_GETGID          9
#define SYSCALL_SETUID          10
#define SYSCALL_SETGID          11
#define SYSCALL_MSLEEP          12

uint64_t luxSyscall(uint64_t request, uint64_t p0, uint64_t p1, uint64_t p2, uint64_t p3);
