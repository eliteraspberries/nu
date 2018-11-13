/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#undef NDEBUG

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "nu.h"
#include "test.h"

int
main(void)
{
    float *x = NULL;
    float *y = NULL;
    size_t n;
    n = 1000;
    x = calloc(n, sizeof(float));
    y = calloc(n, sizeof(float));
    assert(x != NULL);
    assert(y != NULL);
    nu_array_linspace(x, 0.f, 1.f, n);
    nu_array_linspace(y, 1.f, 0.f, n);
    for (size_t i = 0; i < n; i++) {
        assert(nu_eq(x[i] + y[i], 1.f, 1e-8f));
    }
    free(x);
    free(y);
    return 0;
}
