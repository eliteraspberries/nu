/* Copyright 2019, Mansour Moufid <mansourmoufid@gmail.com> */

#include <assert.h>
#include <stddef.h>

#include "nu.h"
#include "sum.h"

#define co nu_diff_co

static const float nu_diff_co[][5][5] = {
    [3] = {
        { -1.500000f,   2.000000f,  -0.500000f, },
        { -0.500000f,  -0.000000f,   0.500000f, },
        {  0.500000f,  -2.000000f,   1.500000f, },
    },
    [4] = {
        { -1.833333f,   3.000000f,  -1.500000f,   0.333333f, },
        { -0.333333f,  -0.500000f,   1.000000f,  -0.166667f, },
        {  0.166667f,  -1.000000f,   0.500000f,   0.333333f, },
        { -0.333333f,   1.500000f,  -3.000000f,   1.833333f, },
    },
    [5] = {
        { -2.083333f,   4.000000f,  -3.000000f,   1.333333f,  -0.250000f, },
        { -0.250000f,  -0.833333f,   1.500000f,  -0.500000f,   0.083333f, },
        {  0.083333f,  -0.666667f,   0.000000f,   0.666667f,  -0.083333f, },
        { -0.083333f,   0.500000f,  -1.500000f,   0.833333f,   0.250000f, },
        {  0.250000f,  -1.333333f,   3.000000f,  -4.000000f,   2.083333f, },
    },
};

const size_t nu_diff_kmax = 5;

void
nu_diff(float h, size_t k, float z[], const float x[], size_t n)
{
    assert(h > 0.f);
    assert(k > 1 && k <= nu_diff_kmax);
    float w = 1.f / h;
    for (size_t i = 0; i < k / 2; i++) {
        size_t j = 0;
        assert(j <= n - k);
        z[i] = w * _nu_dot(co[k][i], &(x[j]), k);
    }
    for (size_t i = k / 2; i < n - k / 2; i++) {
        size_t j = i - k / 2;
        assert(j <= n - k);
        z[i] = w * _nu_dot(co[k][k / 2], &(x[j]), k);
    }
    for (size_t i = n - k / 2; i < n; i++) {
        size_t j = n - k;
        assert(j <= n - k);
        z[i] = w * _nu_dot(co[k][k - (n - i)], &(x[j]), k);
    }
}
