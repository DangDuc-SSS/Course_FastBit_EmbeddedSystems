/*
 * it.c
 *
 *  Created on: 27 Jul 2022
 *      Author: abdo
 */

#include "main.h"

extern TIM_HandleTypeDef htimer2;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer2);
}
