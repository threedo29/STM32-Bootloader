#include "hw.h"

void hw_init(void)
{
    /* BSP */
    board_init();

    /* Module */
    watchdog_init();
    timer_init();
}

void board_init(void)
{
    /* Clock Configuration */
    SystemInit();
    SystemCoreClockUpdate();

    /* GPIO RCC Configuration */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_GPIOD |
                           RCC_APB2Periph_GPIOE |
                           RCC_APB2Periph_GPIOF |
                           RCC_APB2Periph_GPIOG, ENABLE);
}