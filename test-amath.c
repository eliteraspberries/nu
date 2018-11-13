/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#undef NDEBUG

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "nu.h"
#include "test.h"

static float
_map_domain(float x, float domain[2])
{
    return x * (domain[1] - domain[0]) + domain[0];
}

static int
compare(float (*a)(float), float (*b)(float), float x, float e)
{
    float y = a(x);
    float z = b(x);
    if (nu_eq(y, z, e) == 0) {
        fprintf(
            stderr,
            "%.8f %.8f %.8f\n",
            (double) x,
            (double) y,
            (double) z
        );
        return 0;
    }
    return 1;
}

int
main(void)
{
    float error = 1e-5f;
    size_t n = 1000000;
    struct nu_random_state r;
    nu_random_seed(&r, 0);
    for (size_t i = 0; i < n; i++) {
        float x = _map_domain(nu_random_float(&r), (float [2]) {-10.f, 10.f});
        assert(compare(cosf, nu_cos, x, error) != 0);
        assert(compare(sinf, nu_sin, x, error) != 0);
    }
    for (size_t i = 0; i < n; i++) {
        float x = _map_domain(nu_random_float(&r), (float [2]) {-3.f, 3.f});
        assert(compare(expf, nu_exp, x, error) != 0);
    }
    for (size_t i = 0; i < n; i++) {
        float x = _map_domain(nu_random_float(&r), (float [2]) {0.f, 100.f});
        assert(compare(logf, nu_log, x, error) != 0);
    }
    return 0;
}
