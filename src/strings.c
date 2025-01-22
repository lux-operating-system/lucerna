/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <ctype.h>
#include <stddef.h>
#include <string.h>

int strncasecmp(const char *s1, const char *s2, size_t n) {
    if(!n) return 0;

    for(size_t i = 0; i < n; i++) {
        if(tolower(s1[i]) != tolower(s2[i]))
            return s1[i] - s2[i];
        else if(!s1[i] && !s2[i])
            return 0;
    }

    return 0;
}

int strcasecmp(const char *s1, const char *s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    if(len1 < len2) return s1[len1-1] - s2[len1-1];
    else if(len1 > len2) return s1[len2-1] - s2[len2-1];

    return strncasecmp(s1, s2, len1);
}