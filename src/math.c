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
