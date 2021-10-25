#include "boot_switch.h"

void boot_switch_init(void)
{
    GPIO_InitTypeDef gpio_init_structure;
    gpio_init_structure.GPIO_Pin = PIN_SWITCH1 | PIN_SWITCH2 | PIN_SWITCH3 | PIN_SWITCH4;
    gpio_init_structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(PORT_SWITCH, &gpio_init_structure);
}

bool is_switch_on(u16 switch_pin)
{
    return ((u16)(PORT_SWITCH->IDR) & switch_pin) ? false : true;
}

bool is_switch_off(u16 switch_pin)
{
    return ((u16)(PORT_SWITCH->IDR) & switch_pin) ? true : false;
}