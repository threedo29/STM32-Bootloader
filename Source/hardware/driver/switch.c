#include "switch.h"

void switch_init(void)
{
    GPIO_InitTypeDef gpio_init_structure;
    gpio_init_structure.GPIO_Pin = SWITCH1_PIN | SWITCH2_PIN | SWITCH3_PIN | SWITCH4_PIN;
    gpio_init_structure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(SWITCH_PORT, &gpio_init_structure);
}

/**
 * @brief   Off - 3.3V
 *          On  - 0V
 * 
 * @param   data    Switch Pin
 * @return  true    On
 * @return  false   Off
 */
bool switch_is_on(u8 data)
{
    bool result = false;

    if ((GPIO_ReadInputData(SWITCH1_PORT) & data) == true)
    {
        result = false;
    }
    else
    {
        result = true;
    }

    return result;
}
