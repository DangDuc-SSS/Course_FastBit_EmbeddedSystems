/*
 * 010i2c_master_tx_testing.c
 *
 *  Created on: 21 Jul 2022
 *      Author: abdo
 */


#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include "stm32f407xx.h"

#define MY_ADDR			0x61
#define SLAVE_ADDR		0x68

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i++);
}

I2C_Handle_t I2C1Handle;

// some data
uint8_t some_data[] = "We are testing I2C master Tx\n";

/*
 * PB6 --> SCL
 * PB9 --> SDA
 * ALT function mode : 4
 */

void I2C1_GPIOInits(void)
{
	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OP;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// scl
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);

	// scl
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	GPIO_Init(&I2CPins);


}

void I2C1_Inits(void)
{
	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);
}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIOBtn, GpioLed;

	// this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeripheralClockControl(GPIOA, ENABLE);
	GPIO_Init(&GPIOBtn);

	// this is led gpio configuration
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeripheralClockControl(GPIOD, ENABLE);
	GPIO_Init(&GpioLed);
}

int main(void)
{
	//initiate button
	GPIO_ButtonInit();

	// i2c pin inits
	I2C1_GPIOInits();

	//i2c peripheral configuration
	I2C1_Inits();

	//enable the peripheral
	I2C_PeripheralControl(I2C1, ENABLE);

	while(1)
	{

		// wait for button pressed
		while(! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

		// to avoid button de-bouncing related issues 200ms of delay
		delay();

		//send some data to the salve
		I2C_MAsterSendData(&I2C1Handle, some_data, strlen((char*)some_data), SLAVE_ADDR);

	}

	return 0;
}
