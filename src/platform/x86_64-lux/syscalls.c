/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 *
 * Platform-Specific Code for x86_64 lux
 */

/* these functions must be defined for every platform lucerna is to be ported to */

#include <sys/types.h>
#include "syscalls.h"

/* Group 1: Processes, Threads, and Users */

void exit(int status) {
    luxSyscall(SYSCALL_EXIT, status, 0, 0, 0);
}

void _exit(int status) {
    exit(status);
}

pid_t fork(void) {
    return (pid_t) luxSyscall(SYSCALL_FORK, 0, 0, 0, 0);
}

pid_t vfork(void) {
    return fork();
}

int sched_yield(void) {
    return (int) luxSyscall(SYSCALL_YIELD, 0, 0, 0, 0);
}

// TODO: waitpid(), execve(), execrdv()

pid_t getpid(void) {
    return (pid_t) luxSyscall(SYSCALL_GETPID, 0, 0, 0, 0);
}

pid_t gettid(void) {
    return (pid_t) luxSyscall(SYSCALL_GETTID, 0, 0, 0, 0);
}

uid_t getuid(void) {
    return (uid_t) luxSyscall(SYSCALL_GETUID, 0, 0, 0, 0);
}

gid_t getgid(void) {
    return (gid_t) luxSyscall(SYSCALL_GETGID, 0, 0, 0, 0);
}

// TODO: setuid(), setgid()

unsigned long msleep(unsigned long msec) {
    return (unsigned long) luxSyscall(SYSCALL_MSLEEP, msec, 0, 0, 0);
}
