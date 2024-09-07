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

/* Group 2: File System Manipulation */

#define SYSCALL_OPEN            13
#define SYSCALL_CLOSE           14
#define SYSCALL_READ            15
#define SYSCALL_WRITE           16
#define SYSCALL_STAT            17
#define SYSCALL_LSEEK           18
#define SYSCALL_CHOWN           19
#define SYSCALL_CHMOD           20
#define SYSCALL_LINK            21
#define SYSCALL_UNLINK          22
#define SYSCALL_MKNOD           23
#define SYSCALL_MKDIR           24
#define SYSCALL_RMDIR           25
#define SYSCALL_UTIME           26
#define SYSCALL_CHROOT          27
#define SYSCALL_MOUNT           28
#define SYSCALL_UMOUNT          29
#define SYSCALL_FNCTL           30

/* Group 3: Interprocess Communication */

#define SYSCALL_SOCKET          31
#define SYSCALL_CONNECT         32
#define SYSCALL_BIND            33
#define SYSCALL_LISTEN          34
#define SYSCALL_ACCEPT          35
#define SYSCALL_RECV            36
#define SYSCALL_SEND            37

uint64_t luxSyscall(uint64_t request, uint64_t p0, uint64_t p1, uint64_t p2, uint64_t p3);
