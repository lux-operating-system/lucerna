/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#define MNT_FORCE               0x01
#define MNT_DETACH              0x02
#define MNT_EXPIRE              0x04
#define UMOUNT_NOFOLLOW         0x08

int mount(const char *, const char *, const char *, int, void *);
int umount(const char *);
int umount2(const char *, int);
