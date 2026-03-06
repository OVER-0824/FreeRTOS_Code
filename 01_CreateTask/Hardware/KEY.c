#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

uint8_t Get_KeyNum(void)
{
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == RESET)
    {
        HAL_Delay(20);
        while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == RESET);
        HAL_Delay(20);
        return 1;
    }
    return 0;
}
