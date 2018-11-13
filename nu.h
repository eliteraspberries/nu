/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#ifndef NU_H
#define NU_H

#include <stddef.h>
#include <stdint.h>

float nu_cos(float);
float nu_exp(float);
float nu_log(float);
float nu_sin(float);
void nu_array_cos(float [], float [], size_t);
void nu_array_exp(float [], float [], size_t);
void nu_array_log(float [], float [], size_t);
void nu_array_sin(float [], float [], size_t);

typedef struct {uint32_t a, b;} nu_tuple32;
typedef struct {float a, b;} nu_tuplefloat;

struct nu_random_state {
    uint64_t s0;
    uint64_t s1;
    uint64_t s2;
    uint64_t s3;
};
void nu_random_seed256(struct nu_random_state *, uint64_t [4]);
void nu_random_seed(struct nu_random_state *, uint64_t);
uint64_t nu_random(struct nu_random_state *);
float nu_random_float(struct nu_random_state *);
float nu_random_gauss(struct nu_random_state *);
void nu_random_array(struct nu_random_state *, uint64_t [], size_t);
void nu_random_array_float(struct nu_random_state *, float [], size_t);
void nu_random_array_gauss(struct nu_random_state *, float [], size_t);

struct nu_sum_state {
    float sp[256];
    float sn[256];
};
void nu_sum_init(struct nu_sum_state *);
void nu_sum_add(struct nu_sum_state *, float);
float nu_sum_sum(struct nu_sum_state *);
float nu_sum(float [], size_t);
nu_tuplefloat nu_meanvar(float [], size_t);

#endif
