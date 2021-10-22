#include "delay.h"

static void _delay_tick(u32 count)
{
    __asm volatile("    subs    r0, #1\n"
                   "    bne.n   _delay_tick\n"
                   "    bx      lr");
}

void delay_us(u32 us)
{
    _delay_tick(us * (72000000 / 3 / 1000000));
}

void delay(u32 ms)
{
    delay_us(ms * 1000);
}