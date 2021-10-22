#include "app.h"

void app_init(void)
{
}

void app_main(void)
{
    char arr[20];
    uart_send_str(USART1, "Program Start\r\n\r\n");

    sprintf(arr, "- %d\r\n", SystemCoreClock);
    uart_send_str(USART1, arr);

    while (true)
    {
        uart_send_str(USART1, "test\r\n");
        delay(500);
    }
}