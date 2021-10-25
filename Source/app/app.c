#include "app.h"

extern pFunction Jump_To_Application;
extern u32 JumpAddress;

void app_init(void)
{
}

void app_main(void)
{
    FLASH_Unlock();

    if (check_boot_mode())
    {
        uart_send_str(USART1, "\r\n=======================================================================");
        uart_send_str(USART1, "\r\n=                   (C) COPYRIGHT 2021 UTEL Co., Ltd.                 =");
        uart_send_str(USART1, "\r\n=                                                                     =");
        uart_send_str(USART1, "\r\n=     In-Application Programming Application  (Version 1.0.0)         =");
        uart_send_str(USART1, "\r\n=                                                                     =");
        uart_send_str(USART1, "\r\n=======================================================================");
        uart_send_str(USART1, "\r\n\r\n");
        Main_Menu();
    }
    else
    {
        /* Test if user code is programmed starting from address "ApplicationAddress" */
        if (((*(__IO u32*)ApplicationAddress) & 0x2FFE0000) == 0x20000000)
        {
            /* Jump to user application */
            JumpAddress = *(__IO u32*)(ApplicationAddress + 4);
            Jump_To_Application = (pFunction)JumpAddress;
            /* Initialize user application's Stack Pointer */
            __set_MSP(*(__IO u32*)ApplicationAddress);
            Jump_To_Application();
        }
    }

    while (true) {}
}

bool check_boot_mode(void)
{
    bool result = false;

    if (is_switch_off(PIN_SWITCH1) && is_switch_off(PIN_SWITCH2) && is_switch_on(PIN_SWITCH3) && is_switch_on(PIN_SWITCH4))
    {
        result = true;
    }

    return result;
}