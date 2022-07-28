/*
 * stm32f407xx_rcc_driver.c
 *
 *  Created on: 23 Jul 2022
 *      Author: abdo
 */

#include "stm32f407xx_rcc_driver.h"

uint16_t AHB_PreScalar[8] = {2, 4, 8, 16, 64, 128, 256, 512};
uint16_t APB1_PreScalar[4] = {2, 4, 8, 16};

uint32_t RCC_GETPLLOutputClock(void)
{
	return 160000000; // dummy value to be implemented
}


uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1, SystemClk;

	uint8_t clkSrc, temp, ahbp, apb1;
	clkSrc = ((RCC->CFGR >> 2) & 0x03);

	if(clkSrc == 0)
	{
		SystemClk = 16000000;
	}
	else if(clkSrc == 1)
	{
		SystemClk = 8000000;
	}
	else if(clkSrc == 2)
	{
		SystemClk = RCC_GETPLLOutputClock();
	}

	// ahbp
	temp = ((RCC->CFGR >> 4) & 0xF);
	if(temp < 8)
	{
		ahbp = 1;
	}
	else
	{
		ahbp = AHB_PreScalar[temp - 8];
	}

	// apb1
	temp = ((RCC->CFGR >> 10) & 0x7);
	if(temp < 4)
	{
		apb1 = 1;
	}
	else
	{
		apb1 = APB1_PreScalar[temp - 4];
	}

	pclk1 = (SystemClk / ahbp) / apb1;

	return pclk1;
}

uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t pclk2, SystemClk;

	uint8_t clkSrc, temp, ahbp, apb2;
	clkSrc = ((RCC->CFGR >> 2) & 0x03);

	if(clkSrc == 0)
	{
		SystemClk = 16000000;
	}
	else if(clkSrc == 1)
	{
		SystemClk = 8000000;
	}
	else if(clkSrc == 2)
	{
		SystemClk = RCC_GETPLLOutputClock();
	}

	// ahbp
	temp = ((RCC->CFGR >> 4) & 0xF);
	if(temp < 8)
	{
		ahbp = 1;
	}
	else
	{
		ahbp = AHB_PreScalar[temp - 8];
	}

	// apb2
	temp = ((RCC->CFGR >> 13) & 0x7);
	if(temp < 4)
	{
		apb2 = 1;
	}
	else
	{
		apb2 = APB1_PreScalar[temp - 4];
	}

	pclk2 = (SystemClk / ahbp) / apb2;

	return pclk2;
}
