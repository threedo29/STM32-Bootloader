#ifndef __UART_H__
#define __UART_H__

#include "hw_common.h"

#define PIN_UART1_TX                                GPIO_Pin_9
#define PIN_UART1_RX                                GPIO_Pin_10
#define PORT_UART1                                  GPIOA

void uart_init(void);
static void uart1_init(void);
static void uart1_gpio_init(void);
void uart_send_hex(USART_TypeDef* uart, u8 data);
void uart_send_char(USART_TypeDef* uart, char data);
void uart_send_str(USART_TypeDef* uart, char* buffer);

#endif /* __UART_H__ */
