/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#undef NDEBUG

#include <assert.h>
#include <stdlib.h>

#include "nu.h"
#include "test.h"

int
main(void)
{
    const float e = 2.71828182f;
    float error;
    size_t n;
    float sum;
    float *z = NULL;
    error = 1e-5f;
    n = 1000;
    z = nu_array_alloc(n, sizeof(float));
    assert(z != NULL);
    z[0] = 1.f;
    float d = 1.f;
    for (size_t i = 1; i < n; i++) {
        d *= (float) i;
        z[i] = 1.f / d;
    }
    sum = nu_sum(z, n);
    free(z);
    assert(nu_eq(sum, e, error));
    const float pi = 3.14159265f;
    struct nu_sum_state s;
    nu_sum_init(&s);
    n = 1000000;
    d = 1.f;
    float sgn = 1.f;
    nu_sum_add(&s, 1.f);
    for (size_t i = 1; i < n; i++) {
        sgn *= -1.f;
        d += 2.f;
        nu_sum_add(&s, sgn / d);
    }
    sum = 4.f * nu_sum_sum(&s);
    assert(nu_eq(sum, pi, error));
    return 0;
}
