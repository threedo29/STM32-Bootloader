#include "peripheral.h"

void gpio_config(void)
{
    GPIO_InitTypeDef gpio_init_structure;

    gpio_init_structure.GPIO_Pin = PIN_BOOT_SWITCH1 | PIN_BOOT_SWITCH2 | PIN_BOOT_SWITCH3 | PIN_BOOT_SWITCH4;
    gpio_init_structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORT_BOOT_SWITCH, &gpio_init_structure);
}

void switch_is_on(u16 gpio_pin)
{
    if (GPIO_ReadInputDataBit(PORT_BOOT_SWITCH, gpio_pin))
    {
    }
}
