#ifndef __BOOT_SWITCH_H__
#define __BOOT_SWITCH_H__

#include "hw_common.h"

#define PIN_SWITCH1                                 GPIO_Pin_6
#define PIN_SWITCH2                                 GPIO_Pin_7
#define PIN_SWITCH3                                 GPIO_Pin_8
#define PIN_SWITCH4                                 GPIO_Pin_9
#define PORT_SWITCH                                 GPIOC

void boot_switch_init(void);
bool is_switch_on(u16 switch_pin);
bool is_switch_off(u16 switch_pin);

#endif /* __BOOT_SWITCH_H__ */
