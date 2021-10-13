#include "app.h"

void app_main(void)
{
    // char* str = "";
    // FLASH_Unlock();

    // sprintf(str, "BOOT VER : %d.%d.%d\r\n", 2, 0, 1);
    // uart_send_str(USART1, str);

    while (true)
    {
        uart_send_str(USART1, "test\r\n");
        delay(500);
    }
}
