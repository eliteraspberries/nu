/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#ifndef NU_SIN_H
#define NU_SIN_H

#include <assert.h>
#include <math.h>

static inline float
__nu_sin(float x)
{
    const float pi = (float) M_PI;
    const float  a2 = -0.1666666664f;
    const float  a4 =  0.0083333315f;
    const float  a6 = -0.0001984090f;
    const float  a8 =  0.0000027526f;
    const float a10 = -0.0000000239f;
    assert(x >= 0.f && x <= 0.5f * pi);
    float x2 = x * x;
    float x4 = x2 * x2;
    float x6 = x4 * x2;
    float x8 = x6 * x2;
    float x10 = x8 * x2;
    return 1.f + a2 * x2 + a4 * x4 + a6 * x6 + a8 * x8 + a10 * x10;
}

static inline float
_nu_sin(float x)
{
    const float pi = (float) M_PI;
    if (x >= 0.f) {
        if (x <= 0.5f * pi) {
            return x * __nu_sin(x);
        } else if (x <= pi) {
            return _nu_sin(pi - x);
        } else if (x <= 2.f * pi) {
            return -1.f * _nu_sin(x - pi);
        } else {
            int k = (int) (x / (2.f * pi));
            return _nu_sin(x - (float) k * 2.f * pi);
        }
    } else {
        return -1.f * _nu_sin(-1.f * x);
    }
}

#endif
