/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#ifndef NU_COS_H
#define NU_COS_H

#include <assert.h>
#include <math.h>

#pragma STDC FP_CONTRACT ON

static inline float
__nu_cos(float x)
{
    static const float pi = (float) M_PI;
    const float  a2 = -0.4999999963f;
    const float  a4 =  0.0416666418f;
    const float  a6 = -0.0013888397f;
    const float  a8 =  0.0000247609f;
    const float a10 = -0.0000002605f;
    assert(x >= 0.f && x <= 0.5f * pi);
    float x2 = x * x;
    float x4 = x2 * x2;
    float x6 = x4 * x2;
    float x8 = x6 * x2;
    float x10 = x8 * x2;
    return 1.f + a2 * x2 + a4 * x4 + a6 * x6 + a8 * x8 + a10 * x10;
}

static inline float
_nu_cos(float x)
{
    static const float pi = (float) M_PI;
    if (x >= 0.f) {
        if (x <= 0.5f * pi) {
            return __nu_cos(x);
        } else if (x <= pi) {
            return -1.f * _nu_cos(pi - x);
        } else if (x <= 2.f * pi) {
            return -1.f * _nu_cos(x - pi);
        } else {
            int k = (int) (x / (2.f * pi));
            return _nu_cos(x - (float) k * 2.f * pi);
        }
    } else {
        return _nu_cos(-1.f * x);
    }
}

#endif
