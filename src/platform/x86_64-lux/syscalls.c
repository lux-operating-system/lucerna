/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 *
 * Platform-Specific Code for x86_64 lux
 */

/* these functions must be defined for every platform lucerna is to be ported to */

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
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

/* Group 3: Interprocess Communication */

int socket(int domain, int type, int protocol) {
    int status = (int)luxSyscall(SYSCALL_SOCKET, domain, type, protocol, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    } else {
        return status;
    }
}

int connect(int sd, const struct sockaddr *addr, socklen_t len) {
    int status = (int)luxSyscall(SYSCALL_CONNECT, sd, (uint64_t)addr, len, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    } else {
        return status;
    }
}

int bind(int sd, const struct sockaddr *addr, socklen_t len) {
    int status = (int)luxSyscall(SYSCALL_BIND, sd, (uint64_t)addr, len, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    } else {
        return status;
    }
}

int listen(int sd, int backlog) {
    int status = (int)luxSyscall(SYSCALL_LISTEN, sd, backlog, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    } else {
        return status;
    }
}

int accept(int sd, struct sockaddr *addr, socklen_t *len) {
    int status = (int)luxSyscall(SYSCALL_ACCEPT, sd, (uint64_t)addr, (uint64_t)len, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    } else {
        return status;
    }
}