/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#define IOCTL_IN_PARAM              0x0001
#define IOCTL_OUT_PARAM             0x0002

int ioctl(int, unsigned long, ...);
