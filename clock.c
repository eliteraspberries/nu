/* Copyright 2014-2018, Mansour Moufid <mansourmoufid@gmail.com> */

#include <assert.h>
#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach_time.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>

#include "nu.h"

#if defined(CLOCK_REALTIME)
static const clockid_t clock_ids[] = {
#if defined(CLOCK_MONOTONIC_RAW)
    CLOCK_MONOTONIC_RAW,
#endif
#if defined(CLOCK_MONOTONIC)
    CLOCK_MONOTONIC,
#endif
    CLOCK_REALTIME,
};
static const size_t n_clock_ids = sizeof(clock_ids) / sizeof(clockid_t);
static int
clock_init(uint64_t *timer)
{
    struct timespec ts;
    assert(timer != NULL);
    for (size_t i = 0; i < n_clock_ids; i++) {
        if (clock_gettime(clock_ids[i], &ts) == 0) {
            goto ok;
        }
    }
    return 1;
ok:
    *timer = (uint64_t) ts.tv_sec;
    *timer *= 1000000000;
    *timer += (uint64_t) ts.tv_nsec;
    return 0;
}
#endif

#if defined(__APPLE__) && defined(__MACH__)
static int
clock_init_mach(uint64_t *timer)
{
    static mach_timebase_info_data_t tb_info = {
        .numer = 0,
        .denom = 0,
    };
    assert(timer != NULL);
    uint64_t abs_time = mach_absolute_time();
    if (tb_info.denom == 0) {
        (void) mach_timebase_info(&tb_info);
    }
    *timer = abs_time * tb_info.numer / tb_info.denom;
    return 0;
}
#endif

int
nu_clock_tick(uint64_t *timer)
{
#if defined(__APPLE__) && defined(__MACH__)
    return clock_init_mach(timer);
#endif
#if defined(CLOCK_REALTIME)
    return clock_init(timer);
#endif
    return 1;
}

int
nu_clock_tock(uint64_t *timer)
{
    uint64_t now;
    assert(timer != NULL);
    if (nu_clock_tick(&now) != 0) {
        return 1;
    }
    assert(now >= *timer);
    *timer = now - *timer;
    return 0;
}
