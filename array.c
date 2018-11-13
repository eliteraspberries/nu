/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#include <assert.h>
#include <stddef.h>

#include "nu.h"
#include "cos.h"
#include "exp.h"
#include "log.h"
#include "sin.h"

float
nu_array_max(float x[], size_t n)
{
    assert(n > 0);
    float max = x[0];
    for (size_t i = 1; i < n; i++) {
        if (x[i] > max) {
            max = x[i];
        }
    }
    return max;
}

float
nu_array_min(float x[], size_t n)
{
    assert(n > 0);
    float min = x[0];
    for (size_t i = 1; i < n; i++) {
        if (x[i] < min) {
            min = x[i];
        }
    }
    return min;
}

void
nu_array_add(float z[], float x[], float y[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = x[i] + y[i];
    }
}

void
nu_array_mul(float z[], float x[], float y[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = x[i] * y[i];
    }
}

void
nu_array_cos(float z[], float x[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = _nu_cos(x[i]);
    }
}

void
nu_array_exp(float z[], float x[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = _nu_exp(x[i]);
    }
}

void
nu_array_log(float z[], float x[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = _nu_log(x[i]);
    }
}

void
nu_array_sin(float z[], float x[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = _nu_sin(x[i]);
    }
}

void
nu_array_linspace(float z[], float start, float stop, size_t n)
{
    float x = start;
    assert(n > 1);
    float dx = (stop - start) / ((float) (n - 1));
    for (size_t i = 0; i < n; i++) {
        z[i] = x;
        x += dx;
    }
}
