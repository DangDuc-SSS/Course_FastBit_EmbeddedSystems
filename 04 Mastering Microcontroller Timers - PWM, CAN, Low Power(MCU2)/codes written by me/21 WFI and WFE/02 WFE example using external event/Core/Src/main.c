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

void SystemClock_Config_HSE(uint8_t clock_freq);
void Error_handler(void);
void GPIO_Init(void);
void UART2_Init(void);
void GPIO_AnalogConfig(void);

UART_HandleTypeDef huart2;
extern char some_data[];

int main(void)
{
	char msg[50];

	HAL_Init();

	GPIO_Init();

	UART2_Init();

	GPIO_AnalogConfig();

	while(1)
	{
		if(HAL_UART_Transmit(&huart2, (uint8_t*) some_data, (uint16_t)strlen((char*) some_data), HAL_MAX_DELAY) != HAL_OK)
		{
			Error_handler();
		}

		sprintf(msg, "Going to Sleep !\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, (uint16_t)strlen((char*) msg), HAL_MAX_DELAY);

		/* Systick is not required so disabled it before going to sleep */
		HAL_SuspendTick();

		// before using these lines , current consumption = 3.50 mA
		// after using these lines , current consumption = 2.4 mA
		/* going to sleep here */
		__SEV(); // Send event -> just to make sure an event is always available even if the systick didn't cause any event
		__WFE(); // Clears the event and resumes back
		__WFE(); // Goes to sleep definitely
		// MCU resumes here when it wakes up

		/* Enable the Systick*/
		HAL_ResumeTick();

		sprintf(msg, "Woke up !\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, (uint16_t)strlen((char*) msg), HAL_MAX_DELAY);
	}

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

void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_SLEEP_DISABLE();

#if 0
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &ledgpio);

	ledgpio.Pin = GPIO_PIN_12;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &ledgpio);

#endif

	GPIO_InitTypeDef buttongpio;

	buttongpio.Pin = GPIO_PIN_13;
	buttongpio.Mode = GPIO_MODE_EVT_FALLING;
	buttongpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &buttongpio);

#if 0
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
#endif

	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

}

void GPIO_AnalogConfig(void)
{
	GPIO_InitTypeDef GpioA,GpioC;

	uint32_t gpio_pins = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | \
							GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
							GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | \
							GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | \
							GPIO_PIN_14 | GPIO_PIN_15;

	GpioA.Pin = gpio_pins;
	GpioA.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA, &GpioA);


	gpio_pins = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | \
				GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | \
				GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | \
				GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
				GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;

	GpioC.Pin = gpio_pins;
	GpioC.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOC, &GpioC);
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(HAL_UART_Transmit(&huart2, (uint8_t*) some_data, (uint16_t)strlen((char*) some_data), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}
}

