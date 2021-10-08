#include "gpio.h"

void gpio_init(void)
{
    GPIO_InitTypeDef gpio_init_structure;
    
    gpio_init_structure.GPIO_Pin = TEST_PIN;
    gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TEST_PORT, &gpio_init_structure);

    GPIO_ResetBits(TEST_PORT, TEST_PIN);
}

void gpio_set_bit(u8 data)
{

}

void gpio_reset_bit(u8 data)
{

}

void gpio_toggle_bit(u8 data)
{

}
