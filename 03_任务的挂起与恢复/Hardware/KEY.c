#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

uint8_t Get_KeyNum(void)
{
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == RESET)
    {
        vTaskDelay(10);
        return 1;
    }
    else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == RESET)
    {
        vTaskDelay(10);
        return 6;
    }
	return 0;
}
