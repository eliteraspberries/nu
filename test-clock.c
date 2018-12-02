/* Copyright 2018, Mansour Moufid <mansourmoufid@gmail.com> */

#undef NDEBUG

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include "nu.h"

int
main(void)
{
    uint64_t t;
    for (size_t i = 0; i < 10; i++) {
        nu_clock_tick(&t);
        sleep(1);
        nu_clock_tock(&t);
        assert(t >= 1000000000UL);
    }
    return 0;
}
