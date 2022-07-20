/*
 * 006spi_tx_testing.c
 *
 *  Created on: 18 Jul 2022
 *      Author: abdo
 */

#include <string.h>
#include <stdio.h>
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407_spi_driver.h"


//command codes
#define COMMAND_LED_CTRL        0x50
#define COMMAND_SENSOR_READ     0x51
#define COMMAND_LED_READ        0x52
#define COMMAND_PRINT          	0x53
#define COMMAND_ID_READ         0x54

#define LED_ON     				1
#define LED_OFF    				0

//arduino analog pins
#define ANALOG_PIN0  		 	0
#define ANALOG_PIN1   			1
#define ANALOG_PIN2   			2
#define ANALOG_PIN3   			3
#define ANALOG_PIN4   			4

// arduino led
#define LED_PIN					9

/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 --> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode : 5
 */

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	// MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	// MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	// NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
}

void SPI2_Inits()
{
	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;	// generates sclk of 8 MHZ
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_DI;	// hardware slave management is enabled for NSS pin

	SPI_Init(&SPI2Handle);
}

void delay(void)
{
	for(uint32_t i = 0; i < 500000/2; i++);
}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIOBtn;

	// this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);
}

uint8_t SPI_verifyResponse(uint8_t ackbyte)
{
	if(ackbyte == 0xF5)
	{
		// ack
		return 1;
	}
	else
	{
		// nack
		return 0;
	}
}

int main(void)
{
	uint8_t dummy_write = 0xFF;
	uint8_t dummy_read = 0xFF;

	GPIO_ButtonInit();

	// thus function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();

	// This function is used to initialize the SPI2 peripheral parameters
	SPI2_Inits();

	/*
	 * making SSOE 1 does NSS output enable.
	 * The NSS pin is automatically managed by the hardware
	 * i.e when SPE = 1, NSS will be pulled to low
	 * and NSS pin will be high when SPE = 0
	 */
	SPI_SSOEConfig(SPI2, ENABLE);

	while(1)
	{
		// wait till the button is pressed
		while(!GPIO_ReadFromInputPin(GPIOA,  GPIO_PIN_NO_0));

		// to avoid button de-bouncing related issues 200ms of delay
		delay();

		// enable the SPI2 peripheral
		SPI_PeripheralControl(SPI2, ENABLE);


		// 1. CMD_LED_CTRL	<pin no(1)>		<value(1)>
		uint8_t commandCode = COMMAND_LED_CTRL;
		uint8_t ackbyte;
		uint8_t  args[2];

		// send command
		SPI_SendData(SPI2, &commandCode, 1);

		// do dummy read to clear off RXNE
		SPI_RecieveData(SPI2, &dummy_read, 1);

		//send some dummy bits (1 byte) to fetch the response from the slave
		SPI_SendData(SPI2, &dummy_write, 1);

		// read the ack byte received
		SPI_RecieveData(SPI2, &ackbyte, 1);

		if(SPI_verifyResponse(ackbyte))
		{
			// send arguments
			args[0] = LED_PIN;
			args[1] = LED_ON;

			// send arguments
			SPI_SendData(SPI2, args, 2);
		}
		// end of COMMAND_LED_CTRL

		// 2. CMD_SENSOR_READ  <analog pin number(1)>

		// wait till the button is pressed
		while(!GPIO_ReadFromInputPin(GPIOA,  GPIO_PIN_NO_0));

		// to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandCode = COMMAND_SENSOR_READ;

		// send command
		SPI_SendData(SPI2, &commandCode, 1);

		// do dummy read to clear off RXNE
		SPI_RecieveData(SPI2, &dummy_read, 1);

		//send some dummy bits (1 byte) to fetch the response from the slave
		SPI_SendData(SPI2, &dummy_write, 1);

		// read the ack byte received
		SPI_RecieveData(SPI2, &ackbyte, 1);

		if(SPI_verifyResponse(ackbyte))
		{
			// send arguments
			args[0] = ANALOG_PIN0;

			// send arguments
			SPI_SendData(SPI2, args, 1);

			// do dummy read to clear off RXNE
			SPI_RecieveData(SPI2, &dummy_read, 1);

			// insert some delay so that slave can ready with the data
			delay();

			//send some dummy bits (1 byte) to fetch the response from the slave
			SPI_SendData(SPI2, &dummy_write, 1);

			uint8_t analog_read;
			SPI_RecieveData(SPI2, &analog_read, 1);

			printf("sensor read = %d\n",analog_read);
		}
		// end of COMMAND_SENSOR_READ


		// 3. CMD_LED_READ  <pin number(1)>

		// wait till the button is pressed
		while(!GPIO_ReadFromInputPin(GPIOA,  GPIO_PIN_NO_0));

		// to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandCode = COMMAND_LED_READ;

		// send command
		SPI_SendData(SPI2, &commandCode, 1);

		// do dummy read to clear off RXNE
		SPI_RecieveData(SPI2, &dummy_read, 1);

		//send some dummy bits (1 byte) to fetch the response from the slave
		SPI_SendData(SPI2, &dummy_write, 1);

		// read the ack byte received
		SPI_RecieveData(SPI2, &ackbyte, 1);

		if(SPI_verifyResponse(ackbyte))
		{
			// send arguments
			args[0] = LED_PIN;

			// send arguments
			SPI_SendData(SPI2, args, 1);

			// do dummy read to clear off RXNE
			SPI_RecieveData(SPI2, &dummy_read, 1);

			// insert some delay so that slave can ready with the data
			delay();

			//send some dummy bits (1 byte) to fetch the response from the slave
			SPI_SendData(SPI2, &dummy_write, 1);

			uint8_t led_read;
			SPI_RecieveData(SPI2, &led_read, 1);

			printf("led pin read = %d\n",led_read);
		}
		// end of CMD_LED_READ

		// 4. CMD_PRINT  <len(2)>  <message(len)>

		// wait till the button is pressed
		while(!GPIO_ReadFromInputPin(GPIOA,  GPIO_PIN_NO_0));

		// to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandCode = COMMAND_PRINT;

		// send command
		SPI_SendData(SPI2, &commandCode, 1);

		// do dummy read to clear off RXNE
		SPI_RecieveData(SPI2, &dummy_read, 1);

		//send some dummy bits (1 byte) to fetch the response from the slave
		SPI_SendData(SPI2, &dummy_write, 1);

		// read the ack byte received
		SPI_RecieveData(SPI2, &ackbyte, 1);

		if(SPI_verifyResponse(ackbyte))
		{
			char message[] = "hello , this message is from stm32f407 , I wish you glad that you recieved this message";
			// send arguments
			uint8_t message_len = strlen(message);

			// send arguments
			SPI_SendData(SPI2, &message_len, 1);
			SPI_SendData(SPI2, (uint8_t*)message, strlen(message));
		}
		// end of CMD_PRINT


		// 5. COMMAND_ID_READ

		// wait till the button is pressed
		while(!GPIO_ReadFromInputPin(GPIOA,  GPIO_PIN_NO_0));

		// to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandCode = COMMAND_ID_READ;

		// send command
		SPI_SendData(SPI2, &commandCode, 1);

		// do dummy read to clear off RXNE
		SPI_RecieveData(SPI2, &dummy_read, 1);

		//send some dummy bits (1 byte) to fetch the response from the slave
		SPI_SendData(SPI2, &dummy_write, 1);

		// read the ack byte received
		SPI_RecieveData(SPI2, &ackbyte, 1);

		if(SPI_verifyResponse(ackbyte))
		{
			char ID[10];
			for(int i = 0; i < 10; i++)
			{
				//send some dummy bits (10 bytes) to fetch the response from the slave
				SPI_SendData(SPI2, &dummy_write, 1);

				// read the byte received from the ID
				SPI_RecieveData(SPI2, (uint8_t*)&ID[i], 1);
			}


			printf("ID = %s\n",ID);
		}
		// end of COMMAND_ID_READ

		// lets confirm that is not busy
		while(SPI_GetFlagStatus(SPI2, SPI_BSY_FLAG) == SET);

		//Disable the SPI2 peripheral
		SPI_PeripheralControl(SPI2, DISABLE);
	}

	return 0;
}
