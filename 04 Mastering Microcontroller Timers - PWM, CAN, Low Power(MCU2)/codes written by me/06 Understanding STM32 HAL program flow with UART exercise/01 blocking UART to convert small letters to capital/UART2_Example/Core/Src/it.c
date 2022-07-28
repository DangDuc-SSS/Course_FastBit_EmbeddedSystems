/*
 * it.c
 *
 *  Created on: 27 Jul 2022
 *      Author: abdo
 */

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
