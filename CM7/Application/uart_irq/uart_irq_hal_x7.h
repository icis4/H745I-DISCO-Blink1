/*
 * usart_irq.h
 *
 *  Created on: 23.11.2017
 *      Author: Ivaylo Ilchev iic@melexis.com
 */

#ifndef UART_IRQ_H_
#define UART_IRQ_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void UartIrq_Init(UART_HandleTypeDef* uartHandle);
extern void UartIrq_IRQHandler(UART_HandleTypeDef* uartHandle);
extern int  UartIrq_Transmit(uint8_t* ptr, size_t len);
extern int  UartIrq_Receive(uint8_t* ptr, size_t len);
extern int  UartIrq_GetChar(void);
extern int  UartIrq_SendChar(uint8_t c);

#ifdef __cplusplus
}
#endif

#endif /* UART_IRQ_H_ */
