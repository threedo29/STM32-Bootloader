#include "watchdog.h"

void watchdog_init(void)
{
    watchdog_gpio_init();
}

void watchdog_gpio_init(void)
{
    GPIO_InitTypeDef gpio_init_structure;
    gpio_init_structure.GPIO_Pin = PIN_WATCHDOG_TIMER;
    gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &gpio_init_structure);
}

void watchdog(void)
{
    watchdog_gpio_reset();
    watchdog_gpio_set();
}