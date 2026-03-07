#include "FreeRTOS_Demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "KEY.h"

//启动任务的配置
#define Start_Task_Stack 128
#define Start_Task_Priority 1
TaskHandle_t Start_Task_Handler;
void Start_Task(void);

//任务1的配置
#define Task1_Stack 128
#define Task1_Priority 2
TaskHandle_t Task1_Handler;
void Task1(void *pvParameters);

//任务2的配置
#define Task2_Stack 128
#define Task2_Priority 3
TaskHandle_t Task2_Handler;
void Task2(void *pvParameters);

//任务3的配置
#define Task3_Stack 128
#define Task3_Priority 4
TaskHandle_t Task3_Handler;
void Task3(void *pvParameters);

/**
 * @description:启动FreeRTOS
 * @retval:无
 */
void FreeRTOS_Start(void)
{
	//创建启动任务
	xTaskCreate((TaskFunction_t)Start_Task,				  // 任务函数地址
				(char *)"Start_Task",					  // 任务名字符串
				(configSTACK_DEPTH_TYPE)Start_Task_Stack, // 任务堆栈大小，默认最小128，单位字
				(void *)NULL,							  // 传递给任务的参数
				(UBaseType_t)Start_Task_Priority,		  // 任务优先级
				(TaskHandle_t *)&Start_Task_Handler);	  // 任务句柄的地址

	//开启调度器（开启调度器会自动创建空闲任务）
	vTaskStartScheduler();
}

/**
 * @description:启动任务：用于创建其他Task
 * @param:(void*) pvParameters
 * @retval:无
 */
void Start_Task(void)
{
	//进入临界区：保护临界区内的代码不会被打断
	taskENTER_CRITICAL();

	//任务1
	xTaskCreate((TaskFunction_t)Task1,				 // 任务函数地址
				(char *)"Task1",					 // 任务名字符串
				(configSTACK_DEPTH_TYPE)Task1_Stack, // 任务堆栈大小，默认最小128，单位字
				(void *)NULL,						 // 传递给任务的参数
				(UBaseType_t)Task1_Priority,		 // 任务优先级
				(TaskHandle_t *)&Task1_Handler);	 // 任务句柄的地址

	//任务2
	xTaskCreate((TaskFunction_t)Task2,				 // 任务函数地址
				(char *)"Task2",					 // 任务名字符串
				(configSTACK_DEPTH_TYPE)Task2_Stack, // 任务堆栈大小，默认最小128，单位字
				(void *)NULL,						 // 传递给任务的参数
				(UBaseType_t)Task2_Priority,		 // 任务优先级
				(TaskHandle_t *)&Task2_Handler);	 // 任务句柄的地址

	//任务3
	xTaskCreate((TaskFunction_t)Task3,				 // 任务函数地址
				(char *)"Task3",					 // 任务名字符串
				(configSTACK_DEPTH_TYPE)Task3_Stack, // 任务堆栈大小，默认最小128，单位字
				(void *)NULL,						 // 传递给任务的参数
				(UBaseType_t)Task3_Priority,		 // 任务优先级
				(TaskHandle_t *)&Task3_Handler);	 // 任务句柄的地址

	//退出临界区
	taskEXIT_CRITICAL();

	//启动任务只需要执行一次，用完就删
	vTaskDelete(NULL); //删除启动任务自身
}

/**
 * @description:任务1：LED1每500ms闪烁一次
 * @param:(void*) pvParameters
 * @retval:无
 */
void Task1(void *pvParameters)
{
	while(1)
	{
		LED1_Triggle();
	}
}

/**
 * @description:任务2：LED2每500ms闪烁一次
 * @param:(void*) pvParameters
 * @retval:无
 */
void Task2(void *pvParameters)
{
	while(1)
	{
		LED2_Triggle();
	}
}

/**
 * @description:任务3：判断按键KEY是否按下，按下就删除task1
 * @param:(void*) pvParameters
 * @retval:无
 */
void Task3(void *pvParameters)
{
	while (1)
	{
		if(Get_KeyNum() == 1)
		{
			//先判断Task1是否已经删除过，避免重复删除
			if(Task1_Handler != NULL)
			{
				//按键按下，删除任务1
				vTaskDelete(Task1_Handler);
				Task1_Handler = NULL;
			}
		}
		vTaskDelay(500);
	}
}
