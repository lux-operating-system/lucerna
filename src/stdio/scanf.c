/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t hex2int(const char *s) {
    uint64_t result = 0;
    int shift = 0;

    size_t len = strlen(s);
    if(!len) return 0;

    for(int i = len-1; i >= 0; i++) {
        char digit;
        if(s[i] >= '0' && s[i] <= '9') digit = s[i] - '0';
        else if(s[i] >= 'a' && s[i] <= 'f') digit = s[i] - 'a' + 10;
        else if(s[i] >= 'A' && s[i] <= 'F') digit = s[i] - 'A' + 10;
        else return result;

        result |= digit << shift;
        shift += 4;
    }

    return result;
}

int vsscanf(const char *s, const char *f, va_list args) {
    bool formatter = false, allocate = false;
    bool l = false, ll = false;
    int matches = 0;
    void *ptr;
    int *integer, **intptr;
    long *longv, **longptr;
    long long *llongv, **llongptr;

    while(isspace(*f)) f++;     // skip whitespace

    while(*f) {
        if(*f == '%') {
            formatter = true;
            allocate = false;
            l = false;
            ll = false;
            f++;
            continue;
        }

        while(isspace(*s)) s++;
        if(!*s) return matches;

        if(formatter) {
            switch(*f) {
            case 'm':
                allocate = true;
                f++;
                break;
            
            case 'l':
                if(l) ll = true;
                else l = true;
                f++;
                break;

            case 'd':
            case 'i':
            case 'u':
                formatter = false;

                if(allocate) {
                    if(ll) ptr = malloc(sizeof(long long));
                    else if(l) ptr = malloc(sizeof(long));
                    else ptr = malloc(sizeof(int));

                    if(!ptr) {
                        errno = ENOMEM;
                        return matches;
                    }

                    if(ll) {
                        llongptr = va_arg(args, long long **);
                        *llongptr = (long long *) ptr;
                        **llongptr = atoll(s);
                    } else if(l) {
                        longptr = va_arg(args, long **);
                        *longptr = (long *) ptr;
                        **longptr = atol(s);
                    } else {
                        intptr = va_arg(args, int **);
                        *intptr = (int *) ptr;
                        **intptr = atoi(s);
                    }
                } else {
                    if(ll) {
                        llongv = va_arg(args, long long *);
                        *llongv = atoll(s);
                    } else if(l) {
                        longv = va_arg(args, long *);
                        *longv = atol(s);
                    } else {
                        integer = va_arg(args, int *);
                        *integer = atoi(s);
                    }
                }

                while(isdigit(*s)) s++;
                if(!*s) return matches;
                break;
            
            case 'x':
            case 'X':
                formatter = false;
                uint64_t hex = hex2int(s);

                if(allocate) {
                    if(ll) ptr = malloc(sizeof(long long));
                    else if(l) ptr = malloc(sizeof(long));
                    else ptr = malloc(sizeof(int));

                    if(!ptr) {
                        errno = ENOMEM;
                        return matches;
                    }

                    if(ll) {
                        llongptr = va_arg(args, long long **);
                        *llongptr = (long long *) ptr;
                        **llongptr = hex;
                    } else if(l) {
                        longptr = va_arg(args, long **);
                        *longptr = (long *) ptr;
                        **longptr = hex;
                    } else {
                        intptr = va_arg(args, int **);
                        *intptr = (int *) ptr;
                        **intptr = hex;
                    }
                } else {
                    if(ll) {
                        llongv = va_arg(args, long long *);
                        *llongv = hex;
                    } else if(l) {
                        longv = va_arg(args, long *);
                        *longv = hex;
                    } else {
                        integer = va_arg(args, int *);
                        *integer = hex;
                    }
                }

                while(isxdigit(*s)) s++;
                if(!*s) return matches;
                break;
            }
        }
    }

    return matches;
}

int vfscanf(FILE *file, const char *fmt, va_list args) {

}