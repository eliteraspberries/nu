/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#undef NDEBUG

#include <assert.h>
#include <stdlib.h>

#include "nu.h"
#include "test.h"

int
main(void)
{
    float error;
    nu_tuplefloat meanvar;
    float mean, var;
    size_t n;
    struct nu_random_state s;
    float *x = NULL;
    error = 1e-2f;
    n = 100000;
    x = calloc(n, sizeof(float));
    assert(x != NULL);
    nu_random_array_float(&s, x, n);
    meanvar = nu_meanvar(x, n);
    mean = meanvar.a;
    var = meanvar.b;
    assert(nu_eq(mean, 0.50000f, error));
    assert(nu_eq(var, 0.08333f, error));
    nu_random_array_gauss(&s, x, n);
    meanvar = nu_meanvar(x, n);
    mean = meanvar.a;
    var = meanvar.b;
    assert(nu_eq(mean, 0.f, error));
    assert(nu_eq(var, 1.f, error));
    free(x);
    return 0;
}
