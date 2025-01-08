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
#define SYSCALL_GETTIMEOFDAY    13

/* Group 2: File System Manipulation */

#define SYSCALL_OPEN            14
#define SYSCALL_CLOSE           15
#define SYSCALL_READ            16
#define SYSCALL_WRITE           17
#define SYSCALL_LSTAT           18
#define SYSCALL_FSTAT           19
#define SYSCALL_LSEEK           20
#define SYSCALL_CHOWN           21
#define SYSCALL_CHMOD           22
#define SYSCALL_LINK            23
#define SYSCALL_UNLINK          24
#define SYSCALL_SYMLINK         25
#define SYSCALL_READLINK        26
#define SYSCALL_UMASK           27
#define SYSCALL_MKDIR           28
#define SYSCALL_UTIME           29
#define SYSCALL_CHROOT          30
#define SYSCALL_CHDIR           31
#define SYSCALL_GETCWD          32
#define SYSCALL_MOUNT           33
#define SYSCALL_UMOUNT          34
#define SYSCALL_FCNTL           35
#define SYSCALL_OPENDIR         36
#define SYSCALL_CLOSEDIR        37
#define SYSCALL_READDIR_R       38
#define SYSCALL_SEEKDIR         39
#define SYSCALL_TELLDIR         40

/* Group 3: Interprocess Communication */

#define SYSCALL_SOCKET          41
#define SYSCALL_CONNECT         42
#define SYSCALL_BIND            43
#define SYSCALL_LISTEN          44
#define SYSCALL_ACCEPT          45
#define SYSCALL_RECV            46
#define SYSCALL_SEND            47
#define SYSCALL_KILL            48
#define SYSCALL_SIGACTION       49
#define SYSCALL_SIGRETURN       50

/* Group 4: Memory Management */

#define SYSCALL_SBRK            51
#define SYSCALL_MMAP            52
#define SYSCALL_MUNMAP          53
#define SYSCALL_MSYNC           54

/* Group 5: Driver I/O Functions */

#define SYSCALL_IOPERM          55
#define SYSCALL_IRQ             56
#define SYSCALL_IOCTL           57
#define SYSCALL_MMIO            58
#define SYSCALL_PCONTIG         59
#define SYSCALL_VTOP            60

uint64_t luxSyscall(uint64_t request, uint64_t p0, uint64_t p1, uint64_t p2, uint64_t p3);
