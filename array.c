/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#include <stddef.h>

#include "nu.h"
#include "cos.h"
#include "exp.h"
#include "log.h"
#include "sin.h"

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
