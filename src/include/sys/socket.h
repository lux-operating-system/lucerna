/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

/* socket family/domain */
#define AF_UNIX                 1
#define AF_LOCAL                AF_UNIX

/* socket type */
#define SOCK_STREAM             1       // stream-oriented
#define SOCK_DGRAM              2       // datagram-oriented
#define SOCK_SEQPACKET          3       // connection-oriented

/* additional socket flags */
#define SOCK_NONBLOCK           0x100
#define SOCK_CLOEXEC            0x200

typedef uint16_t sa_family_t;
typedef size_t socklen_t;

/* generic socket */
struct sockaddr {
    sa_family_t sa_family;
    char sa_data[512];
};

struct sockaddr_storage {
    sa_family_t sa_family;
    char sa_data[512];
};

int socket(int, int, int);
int connect(int, const struct sockaddr *, socklen_t);
int bind(int, const struct sockaddr *, socklen_t);
int listen(int, int);
int accept(int, struct sockaddr *, socklen_t *);
ssize_t recv(int, void *, size_t, int);
ssize_t send(int, const void *, size_t, int);
