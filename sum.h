/* Copyright 2019, Mansour Moufid <mansourmoufid@gmail.com> */

#ifndef NU_SUM_H
#define NU_SUM_H

#include <stddef.h>

#include "nu.h"

static inline float
_nu_sum(const float x[], size_t n)
{
    struct nu_sum_state state;
    if (n == 0) {
        return 0.f;
    }
    nu_sum_init(&state);
    for (size_t i = 0; i < n; i++) {
        nu_sum_add(&state, x[i]);
    }
    return nu_sum_sum(&state);
}

#endif
