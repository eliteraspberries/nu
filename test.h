/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#ifndef NU_TEST_H
#define NU_TEST_H

#include <math.h>

static inline int
nu_eq(float a, float b, float e)
{
    if (isfinite(a) && isfinite(b)) {
        return fabsf(a - b) < e;
    } else {
        return fpclassify(a) == fpclassify(b);
    }
}

static inline int
nu_array_eq(const float x[], const float y[], size_t n, float e)
{
    for (size_t i = 0; i < n; i++) {
        if (!nu_eq(x[i], y[i], e)) {
            return 0;
        }
    }
    return 1;
}

#endif
