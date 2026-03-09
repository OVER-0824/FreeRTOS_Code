#include "FreeRTOS_Demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "KEY.h"

// 启动任务的配置
#define Start_Task_StackDepth 128
#define Start_Task_Priority 1
StackType_t Start_Task_StackBuffer[Start_Task_StackDepth];
StaticTask_t Start_Task_TCB;
TaskHandle_t Start_Task_Handler;
void Start_Task(void);

// 任务1的配置
#define Task1_StackDepth 128
#define Task1_Priority 2
StackType_t Task1_StackBuffer[Task1_StackDepth];
StaticTask_t Task1_TCB;
TaskHandle_t Task1_Handler;
void Task1(void *pvParameters);

// 任务2的配置
#define Task2_StackDepth 128
#define Task2_Priority 3
StackType_t Task2_StackBuffer[Task2_StackDepth];
StaticTask_t Task2_TCB;
TaskHandle_t Task2_Handler;
void Task2(void *pvParameters);

// 任务3的配置
#define Task3_StackDepth 128
#define Task3_Priority 4
StackType_t Task3_StackBuffer[Task3_StackDepth];
StaticTask_t Task3_TCB;
TaskHandle_t Task3_Handler;
void Task3(void *pvParameters);

// 空闲任务资源的配置
#define Idle_Task_StackSize configMINIMAL_STACK_SIZE
StaticTask_t Idle_Task_TCB;								// 空闲任务的TCB
StackType_t Idle_Task_StackBuffer[Idle_Task_StackSize]; // 空闲任务的任务堆栈，以数组方式存储

// 软件定时器资源的配置
StaticTask_t Timer_Task_TCB;							  // 软件定时器的TCB
StackType_t Timer_Task_StackBuffer[configTIMER_TASK_STACK_DEPTH]; // 软件定时器的任务堆栈，以数组方式存储

/*========静态创建任务需要分配的两个接口资源========*/
// 空闲任务资源
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
								   StackType_t **ppxIdleTaskStackBuffer,
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer = &Idle_Task_TCB;
	*ppxIdleTaskStackBuffer = Idle_Task_StackBuffer;
	*pulIdleTaskStackSize = Idle_Task_StackSize;
}

// 软件定时器资源
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
									StackType_t **ppxTimerTaskStackBuffer,
									uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer = &Timer_Task_TCB;
	*ppxTimerTaskStackBuffer = Timer_Task_StackBuffer;
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/**
 * @description:启动FreeRTOS
 * @retval:无
 */
void FreeRTOS_Start(void)
{
	// 创建启动任务
	Start_Task_Handler = xTaskCreateStatic((TaskFunction_t)Start_Task,			  // 指向任务函数入口的指针
										   (char *)"Start_Task",				  // 任务的描述性名称
										   (uint32_t)Start_Task_StackDepth,		  // 任务栈中的元素个数
										   (void *)NULL,						  // 任务参数
										   (UBaseType_t)Start_Task_Priority,	  // 任务优先级
										   (StackType_t *)Start_Task_StackBuffer, // 创建数组的地址，任务栈地址
										   (StaticTask_t *)&Start_Task_TCB);	  // 任务控制块地址

	/*启动调度器：会自动创建空闲任务和软件定时器（如果开启软件定时器的话），
	  如果是采用静态创建任务的方式，还需要实现两个分配资源的接口函数*/
	vTaskStartScheduler();
}

/**
 * @description:启动任务：用于创建其他Task
 * @param:(void*) pvParameters
 * @retval:无
 */
void Start_Task(void)
{
	// 进入临界区：保护临界区内的代码不会被打断
	taskENTER_CRITICAL();

	// 任务1
	Task1_Handler = xTaskCreateStatic((TaskFunction_t)Task1,			// 指向任务函数入口的指针
									  (char *)"Task1",					// 任务的描述性名称
									  (uint32_t)Task1_StackDepth,		// 任务栈中的元素个数
									  (void *)NULL,						// 任务参数
									  (UBaseType_t)Task1_Priority,		// 任务优先级
									  (StackType_t *)Task1_StackBuffer, // 创建数组的地址，任务栈地址
									  (StaticTask_t *)&Task1_TCB);		// 任务控制块地址

	// 任务2
	Task2_Handler = xTaskCreateStatic((TaskFunction_t)Task2,			// 指向任务函数入口的指针
									  (char *)"Task2",					// 任务的描述性名称
									  (uint32_t)Task2_StackDepth,		// 任务栈中的元素个数
									  (void *)NULL,						// 任务参数
									  (UBaseType_t)Task2_Priority,		// 任务优先级
									  (StackType_t *)Task2_StackBuffer, // 创建数组的地址，任务栈地址
									  (StaticTask_t *)&Task2_TCB);		// 任务控制块地址

	// 任务3
	Task3_Handler = xTaskCreateStatic((TaskFunction_t)Task3,			// 指向任务函数入口的指针
									  (char *)"Task3",					// 任务的描述性名称
									  (uint32_t)Task3_StackDepth,		// 任务栈中的元素个数
									  (void *)NULL,						// 任务参数
									  (UBaseType_t)Task3_Priority,		// 任务优先级
									  (StackType_t *)Task3_StackBuffer, // 创建数组的地址，任务栈地址
									  (StaticTask_t *)&Task3_TCB);		// 任务控制块地址

	// 退出临界区
	taskEXIT_CRITICAL();

	// 启动任务只需要执行一次，用完就删
	vTaskDelete(NULL); // 删除启动任务自身
}

/**
 * @description:任务1：LED1每500ms闪烁一次
 * @param:(void*) pvParameters
 * @retval:无
 */
void Task1(void *pvParameters)
{
	while (1)
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
	while (1)
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
		if (Get_KeyNum() == 1)
		{
			//按下按键1，挂起Task1
			vTaskSuspend(Task1_Handler);
		}
		else if(Get_KeyNum() == 6)
		{
			//按下按键2，恢复Task1
			vTaskResume(Task1_Handler);
		}
		vTaskDelay(500);
	}
}
