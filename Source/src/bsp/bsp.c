#include "bsp.h"

void chip_config(void)
{
    SystemInit();
    SystemCoreClockUpdate();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_GPIOD |
                           RCC_APB2Periph_GPIOE |
                           RCC_APB2Periph_GPIOF |
                           RCC_APB2Periph_GPIOG, ENABLE);
}