/*----------------------------------------------------------------------------
 * Name:    Usart.c
 * Purpose: USART usage for STM32
 * Version: V1.00
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * Copyright (c) 2005-2007 Keil Software. All rights reserved.
 *----------------------------------------------------------------------------*/
/** @defgroup UART_IRQ UART Irq
  * @brief This file is the code for uart Rx/Tx irq handling
  * @{
  */

#include "main.h"
#include <stdio.h>
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "stream_buffer.h"
#include "usart.h"

#if defined(STM32H7)
USART_TypeDef* huart_irq = NULL;
#else
UART_HandleTypeDef* huart_irq = NULL;
#endif

StreamBufferHandle_t uartInStreamHandle = NULL;
uint8_t uartInStreamBuffer[ 4000 ] __attribute__ ((section (".noinit")));
StaticStreamBuffer_t uartInStreamControlBlock;

StreamBufferHandle_t uartOutStreamHandle = NULL;
uint8_t uartOutStreamBuffer[ 4000 ] __attribute__ ((section (".noinit")));
StaticStreamBuffer_t uartOutStreamControlBlock;

static unsigned int tx_restart = 1;               // NZ if TX restart is required

/*----------------------------------------------------------------------------
  UART_IRQHandler
  Handles UART global interrupt request.
 *----------------------------------------------------------------------------*/
void UartIrq__IRQHandler(UART_HandleTypeDef* uartHandle)
{
	uint8_t data;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

#if defined(STM32H7)
	if (LL_USART_IsActiveFlag_TXE(uartHandle))
	{
		if (xStreamBufferReceiveFromISR(uartOutStreamHandle, &data, 1, &xHigherPriorityTaskWoken)) {
			LL_USART_TransmitData8(uartHandle, d);
			tx_restart = 0;
		} else {
			tx_restart = 1;
			LL_USART_DisableIT_TXE(uartHandle); // disable TX interrupt if nothing to send
		}
	 }
#else
	uint32_t isrflags   = READ_REG(uartHandle->Instance->ISR);
	// USART_ISR_ORE OverRun Error
	// USART_ISR_PE  Parity Error
	// USART_ISR_FE  Framing Error
	// USART_ISR_NE  Noise detected Flag
	uint32_t errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE));

	if (errorflags != RESET) {
		__HAL_UART_CLEAR_IT(uartHandle, errorflags);
		return;
	}

    if (__HAL_UART_GET_IT(uartHandle, UART_IT_RXNE)) { // read interrupt
    	__HAL_UART_CLEAR_IT(uartHandle, UART_IT_RXNE);	// clear interrupt

    	data = uartHandle->Instance->RDR & 0x07F;
    	if ((data >= ' ' && data < 128u) || data == '\n' || data == '\r') {
    		xStreamBufferSendFromISR(uartInStreamHandle, &data, 1, &xHigherPriorityTaskWoken);
    	}

    	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    if (__HAL_UART_GET_IT(uartHandle, UART_IT_TXE)) {
    	__HAL_UART_CLEAR_IT(uartHandle, UART_IT_TXE); // clear interrupt

      if (xStreamBufferReceiveFromISR(uartOutStreamHandle, &data, 1, &xHigherPriorityTaskWoken)) {
    	  uartHandle->Instance->TDR = data;
    	  tx_restart = 0;
      }
      else {
        tx_restart = 1;
        __HAL_UART_DISABLE_IT(uartHandle, UART_IT_TXE); // disable TX interrupt if nothing to send
      }
    }
#endif
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  UartIrq__IRQHandler(huart_irq);
  HAL_UART_IRQHandler(huart_irq);
}

#if defined(STM32H7)
void UartIrq_Init(USART_TypeDef* uartHandle)
#else
void UartIrq_Init(UART_HandleTypeDef* uartHandle)
#endif
{
	huart_irq = uartHandle;

    /* definition and creation of uartOutStream */
    uartOutStreamHandle = xStreamBufferCreateStatic( sizeof( uartOutStreamBuffer ), 1, uartOutStreamBuffer, &uartOutStreamControlBlock);

	/* definition and creation of uartInStream */
	uartInStreamHandle = xStreamBufferCreateStatic( sizeof( uartInStreamBuffer ), 1, uartInStreamBuffer, &uartInStreamControlBlock);

#if defined(STM32H7)
	if (uartHandle == USART1) {
		//TODO:
	} else if (uartHandle == USART3) {
		/* USART3 interrupt Init */
		NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
		NVIC_EnableIRQ(USART3_IRQn);
		LL_USART_EnableIT_RXNE_RXFNE(uartHandle);
	} else {
		//TODO:
	}
#else
	if (uartHandle->Instance == USART1) {
		HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}

	__HAL_UART_ENABLE_IT(uartHandle, UART_IT_RXNE);
#endif
}

int UartIrq_Transmit(uint8_t* ptr, size_t len)
{
    if (uartOutStreamHandle == NULL) return 0;

    len = xStreamBufferSend(uartOutStreamHandle, ptr, len, portMAX_DELAY);
    if (huart_irq != NULL) {
        if (tx_restart) {                                   // If transmit interrupt is disabled, enable it
            tx_restart = 0;
#if defined(STM32H7)
            LL_USART_EnableIT_TXE(huart_irq);
#elif defined(STM32F7)
            __HAL_UART_ENABLE_IT(huart_irq, UART_IT_TXE);  // enable TX interrupt
#else
#error MCU not (yet) supported
#endif
        }
    }
    return len;
}

int UartIrq_Receive(uint8_t* ptr, size_t len)
{
    return xStreamBufferReceive(uartInStreamHandle, ptr, len, portMAX_DELAY);
}

/*------------------------------------------------------------------------------
  SenChar
  transmit a character
 *------------------------------------------------------------------------------*/
int UartIrq_SendChar(uint8_t c)
{
  if (uartOutStreamHandle == NULL) return 1;

  xStreamBufferSend(uartOutStreamHandle, &c, 1, portMAX_DELAY);

  if (huart_irq != NULL) {
      if (tx_restart) {                                // If transmit interrupt is disabled, enable it
        tx_restart = 0;
#if defined(STM32H7)
        LL_USART_EnableIT_TXE(huart_irq);
#elif defined(STM32F7)
        __HAL_UART_ENABLE_IT(huart_irq, UART_IT_TXE);  // enable TX interrupt
#else
#endif
      }
  }

  return (0);
}

/*------------------------------------------------------------------------------
  GetKey
  receive a character
 *------------------------------------------------------------------------------*/
int UartIrq_GetChar (void)
{
    uint8_t data = 0;

    if (uartInStreamHandle == NULL) return EOF;

    xStreamBufferReceive(uartInStreamHandle, &data, 1, portMAX_DELAY);

    return data;
}


/**
  * @}
  */

/**
  * @}
  */
