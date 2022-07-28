/*
 * it.c
 *
 *  Created on: 27 Jul 2022
 *      Author: abdo
 */

#include "main.h"

UART_HandleTypeDef huart2;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
}
