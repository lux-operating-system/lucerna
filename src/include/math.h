/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <float.h>

#ifndef FLT_EVAL_METHOD
typedef float float_t;
typedef double double_t;
#else
#if FLT_EVAL_METHOD == 0
typedef float float_t;
typedef double double_t;
#else
#if FLT_EVAL_METHOD == 1
typedef double float_t;
typedef double double_t;
#else
typedef long double float_t;
typedef long double double_t;
#endif
#endif
#endif

#define isnan(x)                (x != x)
#define isunordered(x, y)       (isnan(x) || isnan(y))
#define isgreater(x, y)         (x > y)
#define isless(x, y)            (x < y)
#define islessgreater(x, y)     (isgreater(x, y) || isless(x, y))
#define isgreaterequal(x, y)    (!isless(x, y))
#define islessequal(x, y)       (!isgreater(x, y))
#define isinf(x)                ((long double) x <= LDBL_MIN || (long double) x >= LDBL_MAX)
#define isfinite(x)             (!(isinf(x) || isnan(x)))
#define isnormal(x)             (!(isfinite(x) || !x))

#define M_E                     ((double) 2.71828182845904523536)
#define M_LOG2E                 ((double) 1.44269504088896340736)
#define M_LOG10E                ((double) 0.43429448190325182765)
#define M_LN2                   ((double) 0.69314718055994530942)
#define M_LN10                  ((double) 2.30258509299404630199)
#define M_PI                    ((double) 3.14159265358979323846)
#define M_PI_2                  ((double) 1.57079632679489661923)
#define M_PI_4                  ((double) 0.78539816339744830962)
#define M_1_PI                  ((double) 0.31830988618379067154)
#define M_2_PI                  ((double) 0.63661977236758134308)
#define M_2_SQRTPI              ((double) 1.12837916709551257390)
#define M_SQRT2                 ((double) 1.41421356237309504880)
#define M_SQRT1_2               ((double) 0.70710678118654752440)

#define HUGE_VAL                ((double) 9999999999999999999999)
#define HUGE_VALF               ((float) 9999999999999)
#define HUGE_VALL               ((long double) HUGE_VAL)
#define INFINITY                HUGE_VALF
#define NAN                     INFINITY

double exp(double);
float expf(float);
long double expl(long double);

double pow(double, double);
float powf(float, float);
long double powl(long double, long double);

double log(double);
float logf(float);
long double logl(long double);

double log10(double);
float log10f(float);
long double log10l(long double);

double fabs(double);
float fabsf(float);
long double fabsl(long double);