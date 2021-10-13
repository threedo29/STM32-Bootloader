/**
  ******************************************************************************
  * @file    EXTI/EXTI_Example/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup EXTI_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern volatile eOperation_mode g_operation_mode;
extern volatile eActive_mode g_active_mode;
extern volatile u16 g_ad5270_value;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s/startup_stm32f429x.s).    */
/******************************************************************************/

/**
  * @brief  This function handles USART6_IRQHandler interrupt request.
  * @param  None
  * @retval None
  */
void USART6_IRQHandler(void)
{
    u8 receive_data = 0;
    double hv = 0.0;
    u16 hv1 = 0, hv2 = 0;
    u16 bias = 0;
    u16 tmp = 0;
    double temperature = 0.0;
    u16 temperature1 = 0, temperature2 = 0;
    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {
        receive_data = USART_ReceiveData(USART6);
        switch (receive_data)
        {
        case 'a':
        case 'A':
            g_operation_mode = NONE_OPERATION;
            g_ad5270_value += 5;
            ad5270_write_ready();
            delay_us(20);
            ad5270_write_data(g_ad5270_value);
            delay_us(20);
            bias = bias_adc_read();
            hv = get_highvoltage_from_adc(bias);
            hv1 = (u16)hv;
            hv2 = (u16)((hv - (u16)hv) * 100);
            usart_send_str(USART6, "AD5270 = ", sizeof("AD5270 = "));
            usart_send_int(USART6, g_ad5270_value);
            usart_send_char(USART6, '\r');
            usart_send_char(USART6, '\n');
            usart_send_str(USART6, "Highvoltage = ", sizeof("Highvoltage = "));
            usart_send_int(USART6, hv1);
            usart_send_char(USART6, '.');
            usart_send_int_zero(USART6, hv2);
            usart_send_char(USART6, '\r');
            usart_send_char(USART6, '\n');
            break;

        case 's':
        case 'S':
            g_operation_mode = NONE_OPERATION;
            g_ad5270_value -= 5;
            ad5270_write_ready();
            delay_us(20);
            ad5270_write_data(g_ad5270_value);
            delay_us(20);
            bias = bias_adc_read();
            hv = get_highvoltage_from_adc(bias);
            hv1 = (u16)hv;
            hv2 = (u16)((hv - (u16)hv) * 100);
            usart_send_str(USART6, "AD5270 = ", sizeof("AD5270 = "));
            usart_send_int(USART6, g_ad5270_value);
            usart_send_char(USART6, '\r');
            usart_send_char(USART6, '\n');
            usart_send_str(USART6, "Highvoltage = ", sizeof("Highvoltage = "));
            usart_send_int(USART6, hv1);
            usart_send_char(USART6, '.');
            usart_send_int_zero(USART6, hv2);
            usart_send_char(USART6, '\r');
            usart_send_char(USART6, '\n');
            break;

        case 'p':
        case 'P':
            g_operation_mode = NONE_OPERATION;
            tmp = tmp36_adc_read();
            temperature = get_temperature_from_adc(tmp);
            temperature1 = (u16)temperature;
            temperature2 = (u16)((temperature - (u16)temperature) * 100);
            bias = bias_adc_read();
            hv = get_highvoltage_from_adc(bias);
            hv1 = (u16)hv;
            hv2 = (u16)((hv - (u16)hv) * 100);
            usart_send_str(USART6, "AD5270 = ", sizeof("AD5270 = "));
            usart_send_int(USART6, g_ad5270_value);
            usart_send_char(USART6, '\r');
            usart_send_char(USART6, '\n');
            usart_send_str(USART6, "Temperature = ", sizeof("Temperature = "));
            usart_send_int(USART6, temperature1);
            usart_send_char(USART6, '.');
            usart_send_int_zero(USART6, temperature2);
            usart_send_char(USART6, '\r');
            usart_send_char(USART6, '\n');
            usart_send_str(USART6, "Highvoltage = ", sizeof("Highvoltage = "));
            usart_send_int(USART6, hv1);
            usart_send_char(USART6, '.');
            usart_send_int_zero(USART6, hv2);
            usart_send_char(USART6, '\r');
            usart_send_char(USART6, '\n');
            break;

        case 'm':
        case 'M':
            g_operation_mode = ACTIVE_MODE;
            break;

        default:
            break;
        }
        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
    }



    // u8 receive_data = 0x00;
    // if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    // {
    //     receive_data = usart_receive(USART6);
    //     data_receive_protocol(receive_data);
    //     USART_ClearITPendingBit(USART6, USART_IT_RXNE);
    // }
}

/**
  * @brief  This function handles TIM2_IRQHandler interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
    if (((TIM2->SR & TIM_IT_Update) != RESET) && ((TIM2->DIER & TIM_IT_Update) != RESET))
    {
        if (g_active_mode == NONE)
        {
            g_active_mode = LD_TRIGGER;
        }
        TIM2->SR = (u16)~TIM_IT_Update;
        TIM2->CNT = 0;
    }
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
