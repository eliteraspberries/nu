/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#undef NDEBUG

#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "nu.h"
#include "test.h"

static const float pi = (float) M_PI;

int
main(void)
{
    float error;
    float *x;
    float *y;
    float *z;

    error = 1e-5f;
    for (size_t k = 3; k <= nu_diff_kmax; k++) {
        for (size_t n = k + 1; n <= 10; n++) {
            x = nu_array_alloc(n, sizeof(float));
            y = nu_array_alloc(n, sizeof(float));
            z = nu_array_alloc(n, sizeof(float));
            assert(x != NULL);
            assert(y != NULL);
            assert(z != NULL);
            for (size_t i = 0; i < n; i++) {
                x[i] = (float) i;
                z[i] = 1.f;
            }
            nu_diff(1.f, k, y, x, n);
            assert(nu_array_eq(y, z, n, error));
            free(x);
            free(y);
            free(z);
        }
    }

    error = 1e-4f;
    size_t k = 3;
    size_t n = 1000;
    float h = (2.f * pi) / n;
    x = nu_array_alloc(n, sizeof(float));
    y = nu_array_alloc(n, sizeof(float));
    z = nu_array_alloc(n, sizeof(float));
    nu_array_linspace(x, 0.f, 2.f * pi, n);
    nu_array_cos(y, x, n);
    nu_array_sin(x, x, n);
    nu_diff(h, k, z, x, n);
    assert(nu_array_eq(y, z, n, error));
    free(x);
    free(y);
    free(z);

    return 0;
}
