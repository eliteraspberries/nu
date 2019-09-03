/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#ifndef NU_LOG_H
#define NU_LOG_H

#include <assert.h>
#include <float.h>
#include <math.h>

#pragma STDC FP_CONTRACT ON

static inline float
__nu_log(float x)
{
    const float a1 =  0.9999964239f;
    const float a2 = -0.4998741238f;
    const float a3 =  0.3317990258f;
    const float a4 = -0.2407338084f;
    const float a5 =  0.1676540711f;
    const float a6 = -0.0953293897f;
    const float a7 =  0.0360884937f;
    const float a8 = -0.0064535442f;
    assert(x >= 1.f && x <= 2.f);
    float x1 = x - 1.f;
    float x2 = x1 * x1;
    float x3 = x2 * x1;
    float x4 = x3 * x1;
    float x5 = x4 * x1;
    float x6 = x5 * x1;
    float x7 = x6 * x1;
    float x8 = x7 * x1;
    return (a1 * x1 + a2 * x2 + a3 * x3 + a4 * x4 +
            a5 * x5 + a6 * x6 + a7 * x7 + a8 * x8);
}

static inline float
_nu_log(float x)
{
    const float log2 = (float) M_LN2;
    if (x >= 1.f) {
        if (x <= 2.f) {
            return __nu_log(x);
        } else {
            return log2 + _nu_log(0.5f * x);
        }
    } else if (x > 0.f) {
        return -1.f * _nu_log(1.f / x);
    } else {
        return -INFINITY;
    }
}

#endif
