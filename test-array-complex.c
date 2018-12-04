/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#undef NDEBUG

#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include "nu.h"
#include "test.h"

int
main(void)
{
    float error;
    size_t n;
    struct nu_random_state r;
    nu_complex *x = NULL;
    nu_complex *y = NULL;
    nu_complex *z = NULL;
    nu_random_seed(&r, 0);
    error = 1e-6f;
    n = 1000;
    x = nu_array_alloc(n, sizeof(nu_complex));
    y = nu_array_alloc(n, sizeof(nu_complex));
    z = nu_array_alloc(n, sizeof(nu_complex));
    assert(x != NULL);
    assert(y != NULL);
    assert(z != NULL);
    nu_random_array_float(&r, (float *) x, n * 2);
    nu_array_conj(y, x, n);
    nu_array_cmul(z, x, y, n);
    for (size_t i = 0; i < n; i++) {
        float norm = x[i].r * x[i].r + x[i].i * x[i].i;
        assert(nu_eq(z[i].r, norm, error));
        assert(nu_eq(z[i].i, 0.f, error));
    }
    free(x);
    free(y);
    free(z);
    return 0;
}
