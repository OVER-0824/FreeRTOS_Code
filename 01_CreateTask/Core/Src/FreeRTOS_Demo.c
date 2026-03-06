#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "KEY.h"

void Start_Task(void* pvParameters);
#define Start_Task_Stack 128
#define Start_Task_Priority 1
TaskHandle_t Start_Task_Handler;

void LED1_Task(void* pvParameters);
#define LED1_Task_Stack 128
#define LED1_Task_Priority 2
TaskHandle_t LED1_Task_Handler;

void LED2_Task(void* pvParameters);
#define LED2_Task_Stack 128
#define LED2_Task_Priority 3
TaskHandle_t LED2_Task_Handler;

void KEY_Task(void* pvParameters);
#define KEY_Task_Stack 128
#define KEY_Task_Priority 4
TaskHandle_t KEY_Task_Handler;

void FreeRTOS_Start(void)
{
	//创建启动任务
	xTaskCreate(Start_Task,"Start_Task",Start_Task_Stack,NULL,Start_Task_Priority,&Start_Task_Handler);
	//启动调度器
	vTaskStartScheduler();
}

void Start_Task(void* pvParameters)
{
	//进入临界区：保护临界区内的代码不会被打断
	taskENTER_CRITICAL();

	xTaskCreate(LED1_Task,"LED1_Task",LED1_Task_Stack,NULL,LED1_Task_Priority,&LED1_Task_Handler);
	xTaskCreate(LED2_Task,"LED2_Task",LED2_Task_Stack,NULL,LED2_Task_Priority,&LED2_Task_Handler);
	xTaskCreate(KEY_Task,"KEY_Task",KEY_Task_Stack,NULL,KEY_Task_Priority,&KEY_Task_Handler);

	//退出临界区
	taskEXIT_CRITICAL();

	//删除启动任务
	vTaskDelete(NULL);
}

void LED1_Task(void* pvParameters)
{
	while(1)
	{
		LED1_Triggle();
	}
}

void LED2_Task(void* pvParameters)
{
	while(1)
	{
		LED2_Triggle();
	}
}

void KEY_Task(void* pvParameters)
{
	while(1)
	{
		if(Get_KeyNum() == 1)
		{
			if(LED1_Task_Handler != NULL)
			{
				vTaskDelete(LED1_Task_Handler);
				LED1_Task_Handler = NULL;
			}
		}
		vTaskDelay(500);
	}
	
}
