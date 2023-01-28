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
			start_ld1 = osKernelSysTick();
			LL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		}
		if (osKernelSysTick() - start_ld2 > 300) {
			start_ld2 = osKernelSysTick();
			LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		}
		osDelay(1);
	}
}


