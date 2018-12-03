/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#include <assert.h>
#include <stddef.h>

#include "nu.h"
#include "cos.h"
#include "exp.h"
#include "log.h"
#include "sin.h"

size_t
nu_array_argmax(float x[], size_t n)
{
    assert(n > 0);
    float max = x[0];
    size_t j = 0;
    for (size_t i = 1; i < n; i++) {
        if (x[i] > max) {
            max = x[i];
            j = i;
        }
    }
    return j;
}

size_t
nu_array_argmin(float x[], size_t n)
{
    assert(n > 0);
    float min = x[0];
    size_t j = 0;
    for (size_t i = 1; i < n; i++) {
        if (x[i] < min) {
            min = x[i];
            j = i;
        }
    }
    return j;
}

float
nu_array_max(float x[], size_t n)
{
    size_t i = nu_array_argmax(x, n);
    return x[i];
}

float
nu_array_min(float x[], size_t n)
{
    size_t i = nu_array_argmin(x, n);
    return x[i];
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

static inline nu_complex
_nu_cadd(nu_complex x, nu_complex y)
{
    return (nu_complex) {x.r + y.r, x.i + y.i};
}

void
nu_array_cadd(nu_complex z[], nu_complex x[], nu_complex y[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = _nu_cadd(x[i], y[i]);
    }
}

static inline nu_complex
_nu_cmul(nu_complex x, nu_complex y)
{
    return (nu_complex) {
        x.r * y.r - x.i * y.i,
        x.r * y.i + x.i * y.r,
    };
}

void
nu_array_cmul(nu_complex z[], nu_complex x[], nu_complex y[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = _nu_cmul(x[i], y[i]);
    }
}

static inline nu_complex
_nu_conj(nu_complex x)
{
    return (nu_complex) {x.r, -x.i};
}

void
nu_array_conj(nu_complex z[], nu_complex x[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = _nu_conj(x[i]);
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
    assert(n > 1);
    const float m = (float) n;
    float j = 0.f;
    for (size_t i = 0; i < n; i++) {
        z[i] = (start * m + (stop - start) * j) / m;
        j += 1.f;
    }
}
