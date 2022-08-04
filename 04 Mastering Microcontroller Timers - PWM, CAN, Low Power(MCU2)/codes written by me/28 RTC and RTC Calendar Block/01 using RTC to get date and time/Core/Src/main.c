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
#include<stdarg.h>

#define TRUE 			1
#define FALSE 			0


void UART2_Init(void);
void SystemClock_Config_HSE(uint8_t clock_freq);
void GPIO_Init(void);
void printmsg(char *format,...);
void RTC_Init(void);
void RTC_CalendarConfig(void);
char* getDayOfWeek(uint8_t number);

UART_HandleTypeDef huart2;
RTC_HandleTypeDef hrtc;

int main(void)
{
	HAL_Init();

	GPIO_Init();

	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);

	UART2_Init();

	printmsg("This is RTC calenday Test program\r\n");

	RTC_Init();

	RTC_CalendarConfig();


	while(1);
	return 0;
}

void SystemClock_Config_HSE(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	uint32_t FLatency = 0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;

	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(clock_freq)
	{
		case SYS_CLOCK_FREQ_50_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 100;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ = 2;
			osc_init.PLL.PLLR = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_1WS;

			break;
		}

		case SYS_CLOCK_FREQ_84_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 168;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ = 2;
			osc_init.PLL.PLLR = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_2WS;

			break;
		}

		case SYS_CLOCK_FREQ_120_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 240;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ = 2;
			osc_init.PLL.PLLR = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_3WS;

			break;
		}

		case SYS_CLOCK_FREQ_180_MHZ:
		{
			// Enable the clock for the power controller
			__HAL_RCC_PWR_CLK_ENABLE();

			// set regulator voltage scale as 1
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

			// turn on the over drive mode of the voltage regulator
			__HAL_PWR_OVERDRIVE_ENABLE();


			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 360;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ = 2;
			osc_init.PLL.PLLR = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_5WS;

			break;
		}
		default:
			return;

	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_RCC_ClockConfig(&clk_init, FLatency)!= HAL_OK)
	{
		Error_handler();
	}

	//Systick configuration
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

void printmsg(char *format,...)
{
	char str[80];

	/* Extract the argument list using VA apis */
	va_list args;
	va_start(args, format);
	vsprintf(str, format, args);
	HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), HAL_MAX_DELAY);
	va_end(args);

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

void GPIO_Init(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef buttongpio, ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &ledgpio);

	buttongpio.Pin = GPIO_PIN_13;
	buttongpio.Mode = GPIO_MODE_IT_FALLING;
	buttongpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &buttongpio);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void Error_handler(void)
{
	while(1);
}

void RTC_Init(void)
{
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
	hrtc.Init.AsynchPrediv = 0x7F;
	hrtc.Init.SynchPrediv = 0xFF;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	if(HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_handler();
	}
}

void RTC_CalendarConfig(void)
{
	RTC_TimeTypeDef RTC_TimeInit;
	RTC_DateTypeDef RTC_DateInit;
	// This function does RTC calendar Config
	// Lets configure the calendar for Time :  12:11:10 PM Date : 12 june 2018 Friday

	RTC_TimeInit.Hours = 12;
	RTC_TimeInit.Minutes = 11;
	RTC_TimeInit.Seconds = 10;
	RTC_TimeInit.TimeFormat = RTC_HOURFORMAT12_PM;
	if(HAL_RTC_SetTime(&hrtc, &RTC_TimeInit, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}

	RTC_DateInit.Date = 12;
	RTC_DateInit.Month = RTC_MONTH_JUNE;
	RTC_DateInit.Year = 18;
	RTC_DateInit.WeekDay = RTC_WEEKDAY_TUESDAY;
	if(HAL_RTC_SetDate(&hrtc, &RTC_DateInit, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}
}

char* getDayOfWeek(uint8_t number)
{
	char* weekday[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	return weekday[number - 1];
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	RTC_TimeTypeDef RTC_TimeRead;
	RTC_DateTypeDef RTC_DateRead;

	HAL_RTC_GetTime(&hrtc, &RTC_TimeRead, RTC_FORMAT_BIN);

	HAL_RTC_GetDate(&hrtc, &RTC_DateRead, RTC_FORMAT_BIN);

	printmsg("Current Time is : %02d:%02d:%02d\r\n", RTC_TimeRead.Hours, RTC_TimeRead.Minutes, RTC_TimeRead.Seconds);
	printmsg("Current Date is : %02d-%02d-%02d <%s>\r\n", RTC_DateRead.Month, RTC_DateRead.Date, RTC_DateRead.Year, getDayOfWeek(RTC_DateRead.WeekDay));

}

