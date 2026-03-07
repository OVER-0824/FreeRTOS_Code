#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

void LED1_Triggle(void)
{
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_2);
	vTaskDelay(500);
}

void LED2_Triggle(void)
{
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_3);
	vTaskDelay(500);
}
