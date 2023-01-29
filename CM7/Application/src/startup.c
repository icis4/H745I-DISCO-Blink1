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

void startup(void)
{
	UartIrq_Init(&huart3);
	printf("\n" \
			"=== STM32H745I-DISCO ===\n" \
			"\n");
}
