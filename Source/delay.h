#ifndef __DELAY_H__
#define __DELAY_H__

#include "common.h"

static void _delay_tick(u32 count);
void delay_us(u32 us);
void delay(u32 ms);
void delay_750ns(void);

#endif /* __DELAY_H__ */
