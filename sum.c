/* Copyright 2018, 2019, Mansour Moufid <mansourmoufid@gmail.com> */

#include <assert.h>
#include <stddef.h>

#include "nu.h"
#include "sum.h"

static const float ws[] = {
    0x1p-128, 0x1p-127, 0x1p-126, 0x1p-125, 
    0x1p-124, 0x1p-123, 0x1p-122, 0x1p-121, 
    0x1p-120, 0x1p-119, 0x1p-118, 0x1p-117, 
    0x1p-116, 0x1p-115, 0x1p-114, 0x1p-113, 
    0x1p-112, 0x1p-111, 0x1p-110, 0x1p-109, 
    0x1p-108, 0x1p-107, 0x1p-106, 0x1p-105, 
    0x1p-104, 0x1p-103, 0x1p-102, 0x1p-101, 
    0x1p-100, 0x1p-099, 0x1p-098, 0x1p-097, 
    0x1p-096, 0x1p-095, 0x1p-094, 0x1p-093, 
    0x1p-092, 0x1p-091, 0x1p-090, 0x1p-089, 
    0x1p-088, 0x1p-087, 0x1p-086, 0x1p-085, 
    0x1p-084, 0x1p-083, 0x1p-082, 0x1p-081, 
    0x1p-080, 0x1p-079, 0x1p-078, 0x1p-077, 
    0x1p-076, 0x1p-075, 0x1p-074, 0x1p-073, 
    0x1p-072, 0x1p-071, 0x1p-070, 0x1p-069, 
    0x1p-068, 0x1p-067, 0x1p-066, 0x1p-065, 
    0x1p-064, 0x1p-063, 0x1p-062, 0x1p-061, 
    0x1p-060, 0x1p-059, 0x1p-058, 0x1p-057, 
    0x1p-056, 0x1p-055, 0x1p-054, 0x1p-053, 
    0x1p-052, 0x1p-051, 0x1p-050, 0x1p-049, 
    0x1p-048, 0x1p-047, 0x1p-046, 0x1p-045, 
    0x1p-044, 0x1p-043, 0x1p-042, 0x1p-041, 
    0x1p-040, 0x1p-039, 0x1p-038, 0x1p-037, 
    0x1p-036, 0x1p-035, 0x1p-034, 0x1p-033, 
    0x1p-032, 0x1p-031, 0x1p-030, 0x1p-029, 
    0x1p-028, 0x1p-027, 0x1p-026, 0x1p-025, 
    0x1p-024, 0x1p-023, 0x1p-022, 0x1p-021, 
    0x1p-020, 0x1p-019, 0x1p-018, 0x1p-017, 
    0x1p-016, 0x1p-015, 0x1p-014, 0x1p-013, 
    0x1p-012, 0x1p-011, 0x1p-010, 0x1p-009, 
    0x1p-008, 0x1p-007, 0x1p-006, 0x1p-005, 
    0x1p-004, 0x1p-003, 0x1p-002, 0x1p-001, 
    0x1p+000, 0x1p+001, 0x1p+002, 0x1p+003, 
    0x1p+004, 0x1p+005, 0x1p+006, 0x1p+007, 
    0x1p+008, 0x1p+009, 0x1p+010, 0x1p+011, 
    0x1p+012, 0x1p+013, 0x1p+014, 0x1p+015, 
    0x1p+016, 0x1p+017, 0x1p+018, 0x1p+019, 
    0x1p+020, 0x1p+021, 0x1p+022, 0x1p+023, 
    0x1p+024, 0x1p+025, 0x1p+026, 0x1p+027, 
    0x1p+028, 0x1p+029, 0x1p+030, 0x1p+031, 
    0x1p+032, 0x1p+033, 0x1p+034, 0x1p+035, 
    0x1p+036, 0x1p+037, 0x1p+038, 0x1p+039, 
    0x1p+040, 0x1p+041, 0x1p+042, 0x1p+043, 
    0x1p+044, 0x1p+045, 0x1p+046, 0x1p+047, 
    0x1p+048, 0x1p+049, 0x1p+050, 0x1p+051, 
    0x1p+052, 0x1p+053, 0x1p+054, 0x1p+055, 
    0x1p+056, 0x1p+057, 0x1p+058, 0x1p+059, 
    0x1p+060, 0x1p+061, 0x1p+062, 0x1p+063, 
    0x1p+064, 0x1p+065, 0x1p+066, 0x1p+067, 
    0x1p+068, 0x1p+069, 0x1p+070, 0x1p+071, 
    0x1p+072, 0x1p+073, 0x1p+074, 0x1p+075, 
    0x1p+076, 0x1p+077, 0x1p+078, 0x1p+079, 
    0x1p+080, 0x1p+081, 0x1p+082, 0x1p+083, 
    0x1p+084, 0x1p+085, 0x1p+086, 0x1p+087, 
    0x1p+088, 0x1p+089, 0x1p+090, 0x1p+091, 
    0x1p+092, 0x1p+093, 0x1p+094, 0x1p+095, 
    0x1p+096, 0x1p+097, 0x1p+098, 0x1p+099, 
    0x1p+100, 0x1p+101, 0x1p+102, 0x1p+103, 
    0x1p+104, 0x1p+105, 0x1p+106, 0x1p+107, 
    0x1p+108, 0x1p+109, 0x1p+110, 0x1p+111, 
    0x1p+112, 0x1p+113, 0x1p+114, 0x1p+115, 
    0x1p+116, 0x1p+117, 0x1p+118, 0x1p+119, 
    0x1p+120, 0x1p+121, 0x1p+122, 0x1p+123, 
    0x1p+124, 0x1p+125, 0x1p+126, 0x1p+127, 
};

void
nu_sum_init(struct nu_sum_state *state)
{
    for (size_t i = 0; i < 256; i++) {
        state->sp[i] = 0.f;
        state->sn[i] = 0.f;
    }
}

static inline float
_nu_add(float a, float b, float *e)
{
    float s = a + b;
    *e = (a - s) + b;
    return s;
}

void
nu_sum_add(struct nu_sum_state *state, float x)
{
    assert(state != NULL);
    float *const sp = state->sp;
    float *const sn = state->sn;
    if (x > 0.f) {
        for (size_t i = 0; i < 256; i++) {
            if (x < ws[i]) {
                float e;
                sp[i] = _nu_add(sp[i], x, &e);
                if (e != 0.f) {
                    nu_sum_add(state, e);
                }
                break;
            }
        }
        for (size_t i = 0; i < 255; i++) {
            if (sp[i] > ws[i]) {
                float e;
                sp[i + 1] = _nu_add(sp[i + 1], sp[i], &e);
                sp[i] = 0.f;
                if (e != 0.f) {
                    nu_sum_add(state, e);
                }
            } else {
                break;
            }
        }
    } else {
        for (size_t i = 0; i < 256; i++) {
            if (x > -ws[i]) {
                float e;
                sn[i] = _nu_add(sn[i], x, &e);
                if (e != 0.f) {
                    nu_sum_add(state, e);
                }
                break;
            }
        }
        for (size_t i = 0; i < 255; i++) {
            if (sn[i] < -ws[i]) {
                float e;
                sn[i + 1] = _nu_add(sn[i + 1], sn[i], &e);
                sn[i] = 0.f;
                if (e != 0.f) {
                    nu_sum_add(state, e);
                }
            } else {
                break;
            }
        }
    }
}

float
nu_sum_sum(struct nu_sum_state *state)
{
    float s = 0.f;
    for (size_t i = 0; i < 256; i++) {
        s += state->sp[i] + state->sn[i];
    }
    return s;
}

float
nu_sum(const float x[], size_t n)
{
    return _nu_sum(x, n);
}

nu_tuplefloat
nu_meanvar(const float x[], size_t n)
{
    struct nu_sum_state sumx2_state;
    nu_sum_init(&sumx2_state);
    for (size_t i = 0; i < n; i++) {
        nu_sum_add(&sumx2_state, x[i] * x[i]);
    }
    float sumx = _nu_sum(x, n);
    float sumx2 = nu_sum_sum(&sumx2_state);
    float mean = sumx / n;
    float var = (sumx2 - mean * sumx) / n;
    return (nu_tuplefloat) {mean, var};
}
