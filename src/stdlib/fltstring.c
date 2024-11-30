/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

long double strtold(const char *nptr, char **endptr) {
    const char *s = nptr;
    long double result = 0.0;
    int polarity = 1;
    int expSign = 1;
    bool hasFrac = false, hasExp = false;

    while(isspace(*s)) s++;

    if(*s == '-') {
        polarity = -1;
        s++;
    } else if(*s == '+') {
        s++;
    }

    if(isdigit(*s)) {
        while(isdigit(*s)) {
            result = result * 10.0 + (*s - '0');
            s++;
        }
    }

    if(*s == '.') {
        s++;
        long double frac = 0.0;
        long double divisor = 10.0;
        while(isdigit(*s)) {
            frac += (*s - '0') / divisor;
            divisor *= 10.0;
            s++;
        }

        result += frac;
        hasExp = true;
    }

    if (*s == 'e' || *s == 'E') {
        s++;
        if(*s == '-') {
            expSign = -1;
            s++;
        } else if(*s == '+') {
            s++;
        }

        int exponent = 0;
        if(isdigit(*s)) {
            while(isdigit(*s)) {
                exponent = exponent * 10 + (*s - '0');
                s++;
            }

            result *= powl(10.0, expSign * exponent);
            hasExp = true;
        } else {
            errno = EINVAL;
            if (endptr) *endptr = (char *)s;
            return 0.0;
        }
    }

    if(s == nptr || (!hasFrac && !hasExp && !isdigit(*s))) {
        errno = EINVAL;
        if (endptr) *endptr = (char *)nptr;
        return 0.0;
    }

    if (endptr) *endptr = (char *)s;    // ptr to first invalid character
    return result * polarity;
}

double strtod(const char *nptr, char **endptr) {
    return (double) strtold(nptr, endptr);
}

float strtof(const char *nptr, char **endptr) {
    return (float) strtold(nptr, endptr);
}

double atof(const char *str) {
    return strtod(str, NULL);
}