/*
 * startup.c
 *
 *  Created on: Jan 28, 2023
 *      Author: iic
 */

#include "main.h"
#include "usart.h"
#include "uart_irq/uart_irq_hal_x7.h"
#include <stdio.h>
#include <errno.h>
#include "fatfs.h"
#include "files/file_utils.h"

void startup(void)
{
	int result = 0;
	DWORD total_kb = 0, free_kb = 0;

	UartIrq_Init(&huart3);
	printf("\n" \
			"=== STM32H745I-DISCO ===\n" \
			"\n");
	/* FATfs RAM Disk*/
	if (retSDRAMDISK == FR_OK) {
		result = diskfree(SDRAMDISKPath, &total_kb, &free_kb);
		if (!result)
			printf("%s - %lu KiB total drive space. %lu KiB available.\n", SDRAMDISKPath, total_kb, free_kb);
		else
			printf("%s - Error:%x\n", SDRAMDISKPath, errno);
	} else {
		printf("%x, %s - Error\n", (uint32_t)retSDRAMDISK, SDRAMDISKPath);
	}
}

/**
  * @brief  Configure the MPU attributes
  * @param  None
  * @retval None
  */
void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}
