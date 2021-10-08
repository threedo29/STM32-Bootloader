#ifndef __SWITCH_H__
#define __SWITCH_H__

#include "hw_common.h"

#define SWITCH1_PIN                                 GPIO_Pin_6
#define SWITCH1_PORT                                GPIOC

#define SWITCH2_PIN                                 GPIO_Pin_7
#define SWITCH2_PORT                                GPIOC

#define SWITCH3_PIN                                 GPIO_Pin_8
#define SWITCH3_PORT                                GPIOC

#define SWITCH4_PIN                                 GPIO_Pin_9
#define SWITCH4_PORT                                GPIOC

#define SWITCH_PORT                                 GPIOC

void switch_init(void);
bool switch_is_on(u8 data);

#endif
