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
#define SYSCALL_CHDIR           30
#define SYSCALL_GETCWD          31
#define SYSCALL_MOUNT           32
#define SYSCALL_UMOUNT          33
#define SYSCALL_FNCTL           34
#define SYSCALL_OPENDIR         35
#define SYSCALL_CLOSEDIR        36
#define SYSCALL_READDIR_R       37
#define SYSCALL_SEEKDIR         38
#define SYSCALL_TELLDIR         39

/* Group 3: Interprocess Communication */

#define SYSCALL_SOCKET          40
#define SYSCALL_CONNECT         41
#define SYSCALL_BIND            42
#define SYSCALL_LISTEN          43
#define SYSCALL_ACCEPT          44
#define SYSCALL_RECV            45
#define SYSCALL_SEND            46
#define SYSCALL_KILL            47

/* Group 4: Memory Management */

#define SYSCALL_SBRK            48
#define SYSCALL_MMAP            49
#define SYSCALL_MUNMAP          50

/* Group 5: Driver I/O Functions */

#define SYSCALL_IOPERM          51
#define SYSCALL_IRQ             52
#define SYSCALL_IOCTL           53
#define SYSCALL_MMIO            54
#define SYSCALL_PCONTIG         55
#define SYSCALL_VTOP            56

uint64_t luxSyscall(uint64_t request, uint64_t p0, uint64_t p1, uint64_t p2, uint64_t p3);
