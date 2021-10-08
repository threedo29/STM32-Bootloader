#ifndef __GPIO_H__
#define __GPIO_H__

#include "hw_common.h"

#define TEST_PORT                                   GPIOF
#define TEST_PIN                                    GPIO_Pin_9

void gpio_init(void);
void gpio_set_bit(u8 data);
void gpio_reset_bit(u8 data);
void gpio_toggle_bit(u8 data);

#endif
