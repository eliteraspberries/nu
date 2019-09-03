/* Copyright 2014-2019, Mansour Moufid <mansourmoufid@gmail.com> */

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#if defined(__APPLE__) && defined(__MACH__)
#include <sys/sysctl.h>
#endif
#include <unistd.h>

#include "nu.h"
#include "cos.h"
#include "exp.h"
#include "log.h"
#include "sin.h"

#include "nu.h"

#if defined(__linux__)
static int
cache_line_size_linux(size_t *cls)
{
    long int _cls;
    if ((_cls = sysconf(_SC_LEVEL1_DCACHE_LINESIZE)) <= 0) {
        return 1;
    }
    *cls = (size_t) _cls;
    return 0;
}
#endif

#if defined(__APPLE__) && defined(__MACH__)
static int
cache_line_size_darwin(size_t *cls)
{
    long int _cls;
    size_t _cls_size = sizeof(_cls) * 8;
    if (sysctlbyname("hw.cachelinesize", &_cls, &_cls_size, NULL, 0) != 0) {
        return 1;
    }
    *cls = (size_t) _cls;
    return 0;
}
#endif

static int (*const get_cache_line_size)(size_t *) =
#if defined(__linux__)
    cache_line_size_linux;
#elif defined(__APPLE__) && defined(__MACH__)
    cache_line_size_darwin;
#else
    NULL;
#endif

static size_t
cache_line_size(void)
{
    static size_t cls = 0;
    if (cls == 0) {
        if (get_cache_line_size == NULL ||
            (*get_cache_line_size)(&cls) != 0) {
            cls = 64;
        }
    }
    return cls;
}

#if defined(_POSIX_C_SOURCE)
static int
page_size_posix(size_t *ps)
{
    long int _ps;
    if ((_ps = sysconf(_SC_PAGESIZE)) < 0) {
        return 1;
    }
    *ps = (size_t) _ps;
    return 0;
}
#endif

static int (*const get_page_size)(size_t *) =
#if defined(_POSIX_C_SOURCE)
    page_size_posix;
#else
    NULL;
#endif

static size_t
page_size(void)
{
    static size_t ps = 0;
    if (ps == 0) {
        if (get_page_size == NULL ||
            (*get_page_size)(&ps) != 0) {
            ps = 4096;
        }
    }
    return ps;
}

static size_t
gcd(size_t a, size_t b)
{
    assert(a != 0);
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

static size_t
lcm(size_t a, size_t b)
{
    assert(a != 0);
    assert(b != 0);
    return a * b / gcd(a, b);
}

static inline size_t
optimal_alignment(size_t stride)
{
    return lcm(lcm(stride, sizeof(void *)), cache_line_size());
}

static size_t
next(size_t a, size_t b)
{
    assert(b != 0);
    return ((a + b - 1) / b) * b;
}

static inline size_t
optimal_size(size_t size)
{
    return next(size, page_size());
}

void *
nu_array_alloc(size_t nelem, size_t stride)
{
    void *ptr = NULL;
    if (nelem == 0 || stride == 0) {
        errno = EINVAL;
        goto error;
    }
    size_t alignment = optimal_alignment(stride);
    size_t size = optimal_size(nelem * stride);
    if (posix_memalign(&ptr, alignment, size) != 0) {
        goto error;
    }
    return ptr;
error:
    return NULL;
}

void
nu_array_free(void *ptr)
{
    free(ptr);
}

size_t
nu_array_argmax(const float x[], size_t n)
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
nu_array_argmin(const float x[], size_t n)
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
nu_array_max(const float x[], size_t n)
{
    size_t i = nu_array_argmax(x, n);
    return x[i];
}

float
nu_array_min(const float x[], size_t n)
{
    size_t i = nu_array_argmin(x, n);
    return x[i];
}

void
nu_array_add(float z[], const float x[], const float y[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = x[i] + y[i];
    }
}

void
nu_array_mul(float z[], const float x[], const float y[], size_t n)
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
nu_array_cadd(
    nu_complex z[],
    const nu_complex x[],
    const nu_complex y[],
    size_t n
)
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
nu_array_cmul(
    nu_complex z[],
    const nu_complex x[],
    const nu_complex y[],
    size_t n
)
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
nu_array_conj(nu_complex z[], const nu_complex x[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = _nu_conj(x[i]);
    }
}

void
nu_array_cos(float z[], const float x[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = _nu_cos(x[i]);
    }
}

void
nu_array_exp(float z[], const float x[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = _nu_exp(x[i]);
    }
}

void
nu_array_log(float z[], const float x[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        z[i] = _nu_log(x[i]);
    }
}

void
nu_array_sin(float z[], const float x[], size_t n)
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
