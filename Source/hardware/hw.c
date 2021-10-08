#include "hw.h"

void hw_init(void)
{
    bsp_init();

    gpio_init();
    switch_init();
}