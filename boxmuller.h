/* Copyright 2018, 2019, Mansour Moufid <mansourmoufid@gmail.com> */

#ifndef NU_BOXMULLER_H
#define NU_BOXMULLER_H

#include <assert.h>
#include <math.h>

#include "nu.h"

#pragma STDC FP_CONTRACT ON

static inline float
__nu_cos(float x)
{
    static const float pi = (float) M_PI;
    const float a2 = -0.49670f;
    const float a4 =  0.03705f;
    assert(x >= 0.f && x <= 0.5f * pi);
    float x2 = x * x;
    float x4 = x2 * x2;
    return 1.f + a2 * x2 + a4 * x4;
}

static inline float
_nu_cos(float x)
{
    static const float pi = (float) M_PI;
    assert(x >= 0.f && x <= 2.f * pi);
    if (x <= 0.5f * pi) {
        return __nu_cos(x);
    } else if (x <= pi) {
        return -1.f * _nu_cos(pi - x);
    } else {
        return _nu_cos(2.f * pi - x);
    }
}

static inline float
__nu_exp(float x)
{
    static const float ln2 = (float) M_LN2;
    const float a1 = -0.9998684f;
    const float a2 =  0.4982926f;
    const float a3 = -0.1595332f;
    const float a4 =  0.0293641f;
    assert(x >= 0.f && x <= ln2);
    float x2 = x * x;
    float x3 = x2 * x;
    float x4 = x3 * x;
    float e = 1.f + a1 * x + a2 * x2 + a3 * x3 + a4 * x4;
    assert(e > 0.f);
    return 1.f / e;
}

static inline float
_nu_exp(float x)
{
    static const float ln2 = (float) M_LN2;
    if (x >= 0.f) {
        if (x <= ln2) {
            return __nu_exp(x);
        } else {
            return 2.f * _nu_exp(x - ln2);
        }
    } else {
        return 1.f / _nu_exp(-x);
    }
}

static inline float
__nu_log(float x)
{
    const float a1 =  0.99949556f;
    const float a2 = -0.49190896f;
    const float a3 =  0.28947478f;
    const float a4 = -0.13606275f;
    const float a5 =  0.03215845f;
    assert(x >= 1.f && x <= 2.f);
    float x1 = x - 1.f;
    float x2 = x1 * x1;
    float x3 = x2 * x1;
    float x4 = x3 * x1;
    float x5 = x4 * x1;
    return a1 * x1 + a2 * x2 + a3 * x3 + a4 * x4 + a5 * x5;
}

static inline float
_nu_log(float x)
{
    static const float log2 = (float) M_LN2;
    assert(x >= 0.f);
    if (x > 0.f) {
        if (x < 1.f) {
            return -1.f * _nu_log(1.f / x);
        } else if (x <= 2.f) {
            return __nu_log(x);
        } else {
            return log2 + _nu_log(0.5f * x);
        }
    } else {
        return -INFINITY;
    }
}

static inline float
__nu_sin(float x)
{
    static const float pi = (float) M_PI;
    const float a2 = -0.16605f;
    const float a4 =  0.00761f;
    assert(x >= 0.f && x <= 0.5f * pi);
    float x2 = x * x;
    float x4 = x2 * x2;
    return 1.f + a2 * x2 + a4 * x4;
}

static inline float
_nu_sin(float x)
{
    static const float pi = (float) M_PI;
    assert(x >= 0.f && x <= 2.f * pi);
    if (x <= 0.5f * pi) {
        return x * __nu_sin(x);
    } else if (x <= pi) {
        return _nu_sin(pi - x);
    } else {
        return -1.f * _nu_sin(x - pi);
    }
}

static inline float
_nu_sqrt(float x)
{
    if (x == 0.f) {
        return 0.f;
    }
    float z = _nu_exp(0.5f * _nu_log(x));
    assert(z > 0.f);
    z = 0.5f * (z + x / z);
    z = 0.5f * (z + x / z);
    return z;
}

static inline nu_tuplefloat
_nu_boxmuller(nu_tuplefloat x)
{
    static const float pi = (float) M_PI;
    const float u1 = x.a;
    const float u2 = x.b;
    assert(u1 > 0.f && u1 <= 1.f);
    assert(u2 > 0.f && u2 <= 1.f);
    float R = _nu_sqrt(-2.f * _nu_log(u1));
    float theta = 2.f * pi * u2;
    float z1 = R * _nu_cos(theta);
    float z2 = R * _nu_sin(theta);
    return (nu_tuplefloat) {z1, z2};
}

#endif
