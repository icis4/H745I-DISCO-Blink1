/*
 * retarget.c
 *
 *  Created on: Jan 23, 2023
 *      Author: iic
 */

#include "main.h"
#include <stdio.h>
#include "uart_irq/uart_irq_hal_x7.h"

int _write(int file, char *ptr, int len)
{
    return UartIrq_Transmit((uint8_t*)ptr, len);
}

_ssize_t _read(int fd, void *ptr, size_t len)
{
    return UartIrq_Receive(ptr, len);
}
