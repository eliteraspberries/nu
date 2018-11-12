/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#include <assert.h>
#include <stddef.h>

#include "nu.h"
#include "cos.h"
#include "exp.h"
#include "log.h"
#include "sin.h"

float
nu_cos(float x)
{
    return _nu_cos(x);
}

float
nu_exp(float x)
{
    return _nu_exp(x);
}

float
nu_log(float x)
{
    return _nu_log(x);
}

float
nu_sin(float x)
{
    return _nu_sin(x);
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
