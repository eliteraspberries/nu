/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#include <stddef.h>
#include <stdint.h>

#include "nu.h"

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

float
nu_random_float(struct nu_random_state *state)
{
    return uint32tofloat((uint32_t) next(state));
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
    uint64_t w;
    uint32_t a, b;
    for (size_t i = 0; i < n / 2; i++) {
        w = next(state);
        a = (uint32_t) (w);
        b = (uint32_t) (w >> 32);
        x[2 * i] = uint32tofloat(a);
        x[2 * i + 1] = uint32tofloat(b);
    }
    if (n % 2 != 0) {
        w = next(state);
        a = (uint32_t) (w);
        x[n - 1] = uint32tofloat(a);
    }
}
