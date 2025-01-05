/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <time.h>
#include <stddef.h>
#include <sys/time.h>

time_t time(time_t *tloc) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if(tloc) *tloc = tv.tv_sec;
    return tv.tv_sec;
}
