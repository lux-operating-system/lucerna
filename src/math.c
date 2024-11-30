/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <math.h>
#include <errno.h>

long double expl(long double x) {
    long double result = 1.0;
    long double term = 1.0;

    for(int i = 1; i < 20; i++) {
        term *= x/i;
        result += term;
    }

    return result;
}

double exp(double x) {
    return (double) expl((long double) x);
}

float expf(float x) {
    return (float) expl((long double) x);
}

long double logl(long double x) {
    if(x <= 0.0) {
        errno = EDOM;
        return NAN;
    }

    long double result = 0.0;
    long double term = (x-1.0) / (x+1.0);
    long double z = term;
    long double z2 = z*z;

    for(int i = 1; i < 100; i += 2) {
        result += (1.0 / i) * z;
        z *= z2;
    }

    return result;
}

float logf(float x) {
    return (float) logl((long double) x);
}

double log(double x) {
    return (double) logl((long double) x);
}