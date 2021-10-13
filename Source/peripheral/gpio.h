#ifndef __GPIO_H__
#define __GPIO_H__

#include "common.h"

#define PIN_BOOT_SWITCH1                            GPIO_Pin_6
#define PIN_BOOT_SWITCH2                            GPIO_Pin_7
#define PIN_BOOT_SWITCH3                            GPIO_Pin_8
#define PIN_BOOT_SWITCH4                            GPIO_Pin_9

#define PORT_BOOT_SWITCH                            GPIOC

void gpio_config(void);
void switch_is_on(u16 gpio_pin);

#endif
