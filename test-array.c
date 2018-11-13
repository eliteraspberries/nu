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
    float *x = NULL;
    float *y = NULL;
    error = 1e-8f;
    n = 1000;
    x = calloc(n, sizeof(float));
    y = calloc(n, sizeof(float));
    assert(x != NULL);
    assert(y != NULL);
    nu_array_linspace(x, 0.f, 1.f, n);
    nu_array_linspace(y, 1.f, 0.f, n);
    for (size_t i = 0; i < n; i++) {
        assert(nu_eq(x[i] + y[i], 1.f, error));
    }
    free(x);
    free(y);
    const float pi = (float) M_PI;
    float *theta = NULL;
    error = 1e-6f;
    n = 10000;
    theta = calloc(n, sizeof(float));
    assert(theta != NULL);
    nu_array_linspace(theta, 0.f, 2.f * pi, n);
    x = calloc(n, sizeof(float));
    y = calloc(n, sizeof(float));
    assert(x != NULL);
    assert(y != NULL);
    nu_array_cos(x, theta, n);
    nu_array_sin(y, theta, n);
    assert(nu_eq(nu_array_min(x, n), -1.f, error));
    assert(nu_eq(nu_array_max(x, n), 1.f, error));
    for (size_t i = 0; i < n; i++) {
        assert(nu_eq(x[i] * x[i] + y[i] * y[i], 1.f, error));
    }
    free(theta);
    free(x);
    free(y);
    return 0;
}
