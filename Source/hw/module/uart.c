#include "uart.h"

void uart_init(void)
{
    uart1_init();
}

static void uart1_init(void)
{
    USART_InitTypeDef uart_init_structure;
    uart1_gpio_init();
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    uart_init_structure.USART_BaudRate = 115200;
    uart_init_structure.USART_WordLength = USART_WordLength_8b;
    uart_init_structure.USART_StopBits = USART_StopBits_1;
    uart_init_structure.USART_Parity = USART_Parity_No;
    uart_init_structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uart_init_structure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &uart_init_structure);
    USART_Cmd(USART1, ENABLE);
}

static void uart1_gpio_init(void)
{
    GPIO_InitTypeDef gpio_init_structure;
    gpio_init_structure.GPIO_Pin = PIN_UART1_TX;
    gpio_init_structure.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORT_UART1, &gpio_init_structure);

    gpio_init_structure.GPIO_Pin = PIN_UART1_RX;
    gpio_init_structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(PORT_UART1, &gpio_init_structure);
}

void uart_send_hex(USART_TypeDef* uart, u8 data)
{
    uart->DR = (data & (u16)0x01FF);
    while ((uart->SR & USART_SR_TXE) == RESET);
}

void uart_send_char(USART_TypeDef* uart, char data)
{
    uart->DR = (data & (u16)0x01FF);
    while ((uart->SR & USART_SR_TXE) == RESET);
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
    char str[10]; /* int로 표현가능한 수의 길이 */
    u16 i;
    do
    {
        str[i++] = (num % 10) + '0';
    } while ((num /= 10) > 0);

    while (i)
    {
        uart_send_char(uart, str[--i]);
    }
}