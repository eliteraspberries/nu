/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "nu.h"
#include "boxmuller.h"

static inline nu_tuple32
split64(uint64_t x)
{
    uint32_t a = (uint32_t) (x);
    uint32_t b = (uint32_t) (x >> 32);
    return (nu_tuple32) {a, b};
}

#define C0 3141592653589793238ULL
#define C1 4626433832795028841ULL
#define C2 9716939937510582097ULL
#define C3 4944592307816406286ULL

void
nu_random_seed256(struct nu_random_state *state, uint64_t seed[4])
{
    state->s0 = C0;
    state->s1 = C1;
    state->s2 = C2;
    state->s3 = C3;
    state->s0 ^= seed[0];
    state->s1 ^= seed[1];
    state->s2 ^= seed[2];
    state->s3 ^= seed[3];
}

void
nu_random_seed(struct nu_random_state *state, uint64_t seed)
{
    nu_random_seed256(state, (uint64_t [4]) {seed, 0, 0, 0});
}

static inline uint64_t
rotl64(const uint64_t x, int k)
{
    return (x << k) | (x >> (64 - k));
}

/* Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org) */
static inline uint64_t
next(struct nu_random_state *state)
{
    uint64_t s0 = state->s0;
    uint64_t s1 = state->s1;
    uint64_t s2 = state->s2;
    uint64_t s3 = state->s3;
    const uint64_t result = rotl64(s1 * 5, 7) * 9;
    const uint64_t t = s1 << 17;
    s2 ^= s0;
    s3 ^= s1;
    s1 ^= s2;
    s0 ^= s3;
    s2 ^= t;
    s3 = rotl64(s3, 45);
    state->s0 = s0;
    state->s1 = s1;
    state->s2 = s2;
    state->s3 = s3;
    return result;
}

static inline void
jump(struct nu_random_state *state)
{
    const uint64_t JUMP[] = {
        0x180ec6d33cfd0aba,
        0xd5a61266f0c9392c,
        0xa9582618e03fc9aa,
        0x39abdc4529b1661c,
    };
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    for (size_t i = 0; i < 4; i++) {
        for (int b = 0; b < 64; b++) {
            if (JUMP[i] & (uint64_t) 1 << b) {
                s0 ^= state->s0;
                s1 ^= state->s1;
                s2 ^= state->s2;
                s3 ^= state->s3;
            }
            next(state);
        }
    }
    state->s0 = s0;
    state->s1 = s1;
    state->s2 = s2;
    state->s3 = s3;
}

void
nu_random_jump(struct nu_random_state *state)
{
    jump(state);
}

uint64_t
nu_random(struct nu_random_state *state)
{
    return next(state);
}

static inline float
uint32tofloat(uint32_t x)
{
    union {
        uint32_t i;
        float f;
    } u;
    u.i = x;
    u.i &= 0x007fffff;
    u.i |= 0x3f800000;
    u.f -= 1.f;
    return u.f;
}

static inline nu_tuplefloat
uint64totuplefloat(uint64_t x)
{
    nu_tuple32 u = split64(x);
    return (nu_tuplefloat) {uint32tofloat(u.a), uint32tofloat(u.b)};
}

float
nu_random_float(struct nu_random_state *state)
{
    nu_tuplefloat x = uint64totuplefloat(next(state));
    return x.a;
}

void
nu_random_array(struct nu_random_state *state, uint64_t x[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        x[i] = next(state);
    }
}

void
nu_random_array_float(struct nu_random_state *state, float x[], size_t n)
{
    for (size_t i = 0; i < n - 1; i += 2) {
        assert(i < n);
        assert(i + 1 < n);
        nu_tuplefloat y = uint64totuplefloat(next(state));
        x[i] = y.a;
        x[i + 1] = y.b;
    }
    if (n % 2 != 0) {
        nu_tuplefloat y = uint64totuplefloat(next(state));
        x[n - 1] = y.a;
    }
}

static inline nu_tuplefloat
_nu_gauss(nu_tuplefloat x)
{
    assert(x.a >= 0.f && x.a < 1.f);
    assert(x.b >= 0.f && x.b < 1.f);
    x.a += 0x1p-127f;
    x.b += 0x1p-127f;
    return _nu_boxmuller(x);
}

float
nu_random_gauss(struct nu_random_state *state)
{
    nu_tuplefloat x = uint64totuplefloat(next(state));
    nu_tuplefloat y = _nu_gauss(x);
    return y.a;
}

void
nu_random_array_gauss(struct nu_random_state *state, float x[], size_t n)
{
    for (size_t i = 0; i < n - 1; i += 2) {
        assert(i < n);
        assert(i + 1 < n);
        nu_tuplefloat y = uint64totuplefloat(next(state));
        nu_tuplefloat z = _nu_gauss(y);
        x[i] = z.a;
        x[i + 1] = z.b;
    }
    if (n % 2 != 0) {
        nu_tuplefloat y = uint64totuplefloat(next(state));
        nu_tuplefloat z = _nu_gauss(y);
        x[n - 1] = z.a;
    }
}
