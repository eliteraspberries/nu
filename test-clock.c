/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#undef NDEBUG

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include "nu.h"

static const uint64_t billion = 1000UL * 1000 * 1000;

int
main(void)
{
    uint64_t t = 0;
    nu_clock_tick(&t);
    unsigned int n = 4;
    unsigned int x = n;
    do {
        x = sleep(x);
    } while (x > 0);
    nu_clock_tock(&t);
    assert(t / billion * billion == n * billion);
    return 0;
}
