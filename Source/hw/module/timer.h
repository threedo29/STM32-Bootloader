#ifndef __TIMER_H__
#define __TIMER_H__

#include "hw_common.h"

void timer_init(void);
static void timer2_init(void);
static void timer2_nvic_init(void);

#endif /* __TIMER_H__ */
