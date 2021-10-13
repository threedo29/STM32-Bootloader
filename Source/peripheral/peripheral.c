#include "peripheral.h"

void peripheral_init(void)
{
    gpio_config();
    uart_config();
}