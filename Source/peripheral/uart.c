#include "uart.h"

void uart_config(void)
{
#ifdef __UART1_ENABLE
    uart1_config();
#endif
#ifdef __UART2_ENABLE
#endif
#ifdef __UART3_ENABLE
#endif
#ifdef __UART4_ENABLE
#endif
#ifdef __UART5_ENABLE
#endif
}

#ifdef __UART1_ENABLE

void uart1_config(void)
{
    GPIO_InitTypeDef gpio_init_structure;
    USART_InitTypeDef usart_init_structure;
    // NVIC_InitTypeDef nvic_init_structure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);

    gpio_init_structure.GPIO_Pin = PIN_UART1_TX;
    gpio_init_structure.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORT_UART1, &gpio_init_structure);

    gpio_init_structure.GPIO_Pin = PIN_UART1_RX;
    gpio_init_structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORT_UART1, &gpio_init_structure);

    usart_init_structure.USART_BaudRate = 115200;
    usart_init_structure.USART_WordLength = USART_WordLength_8b;
    usart_init_structure.USART_StopBits = USART_StopBits_1;
    usart_init_structure.USART_Parity = USART_Parity_No;
    usart_init_structure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    usart_init_structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &usart_init_structure);
    USART_Cmd(USART1, ENABLE);

    // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
#endif

void uart_send_hex(USART_TypeDef* uart, u8 data)
{
    uart->DR = (data & (u16)0x01FF);
    while (!(uart->SR & USART_SR_TC));
}

void uart_send_char(USART_TypeDef* uart, char data)
{
    uart->DR = (data & (u16)0x01FF);
    while (!(uart->SR & USART_FLAG_TXE));
}

void uart_send_str(USART_TypeDef* uart, char* buffer)
{
    while (*buffer != '\0')
    {
        uart_send_char(uart, *buffer);
        buffer++;
    }
}

void uart_send_int(USART_TypeDef* uart, u16 num)
{
    char str[10];
    u16 i = 0;

    do
    {
        str[i++] = (num % 10) + '0';
    } while ((num /= 10) > 0);

    while (i)
    {
        uart_send_char(uart, str[--i]);
    }
}

u16 uart_receive(USART_TypeDef* uart)
{
    u16 result = 0;
    while ((uart->SR & USART_FLAG_RXNE) == (u16)RESET);
    result = (u16)(uart->DR & (u16)0x01FF);

    return result;
}
