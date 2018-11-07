/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#include <assert.h>
#include <stddef.h>

#include "nu.h"
#include "cos.h"
#include "sin.h"

float
nu_cos(float x)
{
    return _cos(x);
}

float
nu_sin(float x)
{
    return _sin(x);
}

void
nu_array_cos(float z[], float x[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = _cos(x[i]);
    }
}

void
nu_array_sin(float z[], float x[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = _sin(x[i]);
    }
}
