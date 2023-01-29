/*
 * default_task.c
 *
 *  Created on: Jan 28, 2023
 *      Author: iic
 */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

extern void startup(void);

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
void StartDefaultTask(void const *argument)
{
	startup();

	uint32_t start_ld2 = osKernelSysTick();
	uint32_t start_ld1 = osKernelSysTick();

	/* Infinite loop */
	for (;;) {
		if (osKernelSysTick() - start_ld1 > 1000) {
			printf("D1\n");
			start_ld1 = osKernelSysTick();
			HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		}
		if (osKernelSysTick() - start_ld2 > 300) {
			printf("D2\n");
			start_ld2 = osKernelSysTick();
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		}
		osDelay(1);
	}
}


