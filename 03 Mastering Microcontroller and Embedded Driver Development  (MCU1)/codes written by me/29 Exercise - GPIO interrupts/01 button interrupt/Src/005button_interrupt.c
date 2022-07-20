/*
 * 002led_button.c
 *
 *  Created on: 17 Jul 2022
 *      Author: abdo
 */

#include <string.h>
#include "stm32f407xx.h"

#define LOW				0
#define BTN_PRESSED		LOW

void delay(void)
{
	for(uint32_t i = 0; i < 500000/2; i++);
}

GPIO_Handle_t GpioLed;
GPIO_Handle_t GpioBtn;
int main(void)
{
	memset(&GpioLed,0,sizeof(GpioLed));
	memset(&GpioLed,0,sizeof(GpioBtn));

	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeripheralClockControl(GPIOA, ENABLE);
	GPIO_Init(&GpioLed);


	GpioBtn.pGPIOx = GPIOD;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_PeripheralClockControl(GPIOD, ENABLE);
	GPIO_Init(&GpioBtn);

	//IRQ configuration
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5, NVIC_IRQ_PRIO15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5, ENABLE);


	return 0;
}

void EXTI9_5_IRQHandler(void)
{

	GPIO_IRQHandling(GPIO_PIN_NO_5);
	GPIO_ToggleOutputPin(GpioLed.pGPIOx, GPIO_PIN_NO_14);
	void delay(void);
}
