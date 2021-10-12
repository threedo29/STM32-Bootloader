#ifndef __UART_H__
#define __UART_H__

#include "common.h"

#define __UART1_ENABLE
// #define __UART2_ENABLE
// #define __UART3_ENABLE
// #define __UART4_ENABLE
// #define __UART5_ENABLE

#ifdef __UART1_ENABLE
#define PIN_UART1_TX                                GPIO_Pin_9
#define PIN_UART1_RX                                GPIO_Pin_10
#define PORT_UART1                                  GPIOA
#endif

#ifdef __UART2_ENABLE
#endif

#ifdef __UART3_ENABLE
#endif

#ifdef __UART4_ENABLE
#endif

#ifdef __UART5_ENABLE
#endif

void uart_config(void);
void uart1_config(void);
void usart_send_hex(USART_TypeDef* usart, u8 data);

#endif /* __UART_H__ */
