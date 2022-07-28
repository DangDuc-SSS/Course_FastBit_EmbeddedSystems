/*
 * main.c
 *
 *  Created on: 27 Jul 2022
 *      Author: abdo
 */


#include "stm32f4xx_hal.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

#define TRUE 			1
#define FALSE 			0


void UART2_Init(void);
void Error_handler(void);

UART_HandleTypeDef huart2;



uint8_t data_buffer[100];

int main(void)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;
	uint8_t msg[100];

	HAL_Init();

	UART2_Init();

	memset(&osc_init, 0, sizeof(osc_init));
	memset(&clk_init, 0, sizeof(clk_init));

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_HCLK;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	if(HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0WS) != HAL_OK)
	{
		Error_handler();
	}

	__HAL_RCC_HSI_DISABLE(); // Saves some current

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	UART2_Init();

	memset(msg, 0, sizeof(msg));
	sprintf((char*)msg, "SYSCLK : %ld\r\n",HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2, msg, strlen((char*)msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf((char*)msg, "HCLK : %ld\r\n",HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart2, msg, strlen((char*)msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf((char*)msg, "PCLK1 : %ld\r\n",HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart2, msg, strlen((char*)msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf((char*)msg, "PCLK2 : %ld\r\n",HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart2, msg, strlen((char*)msg), HAL_MAX_DELAY);

	while(1);
	return 0;
}


void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;

	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		// there is a problem
		Error_handler();
	}
}

void Error_handler(void)
{
	while(1);
}


