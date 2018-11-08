/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#ifndef NU_EXP_H
#define NU_EXP_H

#include <assert.h>
#include <math.h>

static inline float
__exp(float x)
{
    const float ln2 = (float) M_LN2;
    const float a1 = -0.9999999995f;
    const float a2 =  0.4999999206f;
    const float a3 = -0.1666653019f;
    const float a4 =  0.0416573475f;
    const float a5 = -0.0083013598f;
    const float a6 =  0.0013298820f;
    const float a7 = -0.0001413161f;
    assert(x >= 0.f && x <= ln2);
    float x2 = x * x;
    float x3 = x2 * x;
    float x4 = x3 * x;
    float x5 = x4 * x;
    float x6 = x5 * x;
    float x7 = x6 * x;
    float e = 1.f + a1 * x  + a2 * x2 + a3 * x3 +
                    a4 * x4 + a5 * x5 + a6 * x6 + a7 * x7;
    return 1.f / e;
}

static inline float
_exp(float x)
{
    const float ln2 = (float) M_LN2;
    if (x >= 0.f) {
        if (x <= ln2) {
            return __exp(x);
        } else {
            return 2.f * _exp(x - ln2);
        }
    } else {
        return 1.f / _exp(-x);
    }
}

#endif
