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

#endif
