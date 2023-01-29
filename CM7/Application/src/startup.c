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
#include "ff.h"
#include "files/file_utils.h"

extern char SDRAMDISKPath[4];   /* SDRAMDISK logical drive path */

void startup(void)
{
	int result = 0;
	DWORD total_kb = 0, free_kb = 0;

	UartIrq_Init(&huart3);
	printf("\n" \
			"=== STM32H745I-DISCO ===\n" \
			"\n");
	/* FATfs RAM Disk*/
	//result = diskfree(SDRAMDISKPath, &total_kb, &free_kb);
	if (!result)
		printf("%s - %lu KiB total drive space. %lu KiB available.\n", SDRAMDISKPath, total_kb, free_kb);
	else
		printf("%s - Error:%x\n", SDRAMDISKPath, errno);

}
