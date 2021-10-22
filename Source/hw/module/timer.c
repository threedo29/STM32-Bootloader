#include "timer.h"

void timer_init(void)
{
    timer2_init();
}

void timer2_init(void)
{
    TIM_TimeBaseInitTypeDef tim_time_base_structure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    tim_time_base_structure.TIM_Prescaler = 7200;
    tim_time_base_structure.TIM_Period = 1000;
    tim_time_base_structure.TIM_ClockDivision = TIM_CKD_DIV1;
    tim_time_base_structure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &tim_time_base_structure);
    TIM_Cmd(TIM2, ENABLE);
    
    timer2_nvic_init();

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void timer2_nvic_init(void)
{
    NVIC_InitTypeDef nvic_init_structure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    nvic_init_structure.NVIC_IRQChannel = TIM2_IRQn;
    nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init_structure.NVIC_IRQChannelSubPriority = 1;
    nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init_structure);
}