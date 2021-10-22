#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include "hw_common.h"

#define PIN_WATCHDOG_TIMER                          GPIO_Pin_8
#define PORT_WATCHDOG_TIMER                         GPIOG

static __inline void watchdog_gpio_set(void)
{
    PORT_WATCHDOG_TIMER->BSRR = PIN_WATCHDOG_TIMER;
}

static __inline void watchdog_gpio_reset(void)
{
    PORT_WATCHDOG_TIMER->BRR = PIN_WATCHDOG_TIMER;
}

void watchdog_init(void);
void watchdog_gpio_init(void);
void watchdog(void);

#endif /* __WATCHDOG_H__ */
