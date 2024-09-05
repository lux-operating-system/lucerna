/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <stdint.h>
#include <stddef.h>         // size_t and ssize_t

// Unix system types

typedef uint64_t dev_t;     // device
typedef uint64_t ino_t;     // inode
typedef uint32_t mode_t;
typedef uint64_t nlink_t;
typedef uint32_t uid_t;
typedef uint32_t gid_t;
typedef int32_t pid_t;
typedef uint32_t id_t;
typedef int64_t off_t;
typedef uint64_t time_t;
typedef uint16_t blksize_t;
typedef int64_t blkcnt_t;
typedef uint64_t clock_t;
typedef int16_t clockid_t;
typedef uint64_t fsblkcnt_t;
typedef uint64_t fsfilcnt_t;
typedef int16_t timer_t;
typedef uint64_t useconds_t;
typedef int64_t suseconds_t;
