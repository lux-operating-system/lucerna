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
#define SYSCALL_TIMES           13

/* Group 2: File System Manipulation */

#define SYSCALL_OPEN            14
#define SYSCALL_CLOSE           15
#define SYSCALL_READ            16
#define SYSCALL_WRITE           17
#define SYSCALL_STAT            18
#define SYSCALL_FSTAT           19
#define SYSCALL_LSEEK           20
#define SYSCALL_CHOWN           21
#define SYSCALL_CHMOD           22
#define SYSCALL_LINK            23
#define SYSCALL_UNLINK          24
#define SYSCALL_MKNOD           25
#define SYSCALL_MKDIR           26
#define SYSCALL_RMDIR           27
#define SYSCALL_UTIME           28
#define SYSCALL_CHROOT          29
#define SYSCALL_MOUNT           30
#define SYSCALL_UMOUNT          31
#define SYSCALL_FNCTL           32
#define SYSCALL_ISATTY          33

/* Group 3: Interprocess Communication */

#define SYSCALL_SOCKET          34
#define SYSCALL_CONNECT         35
#define SYSCALL_BIND            36
#define SYSCALL_LISTEN          37
#define SYSCALL_ACCEPT          38
#define SYSCALL_RECV            39
#define SYSCALL_SEND            40
#define SYSCALL_KILL            41

/* Group 4: Memory Management */

#define SYSCALL_SBRK            42
#define SYSCALL_MMAP            43
#define SYSCALL_MUNMAP          44
#define SYSCALL_MLOCK           45
#define SYSCALL_MUNLOCK         46

/* Group 5: Driver I/O Functions */

#define SYSCALL_IOPERM          47
#define SYSCALL_IRQ             48
#define SYSCALL_IOCTL           49

uint64_t luxSyscall(uint64_t request, uint64_t p0, uint64_t p1, uint64_t p2, uint64_t p3);
