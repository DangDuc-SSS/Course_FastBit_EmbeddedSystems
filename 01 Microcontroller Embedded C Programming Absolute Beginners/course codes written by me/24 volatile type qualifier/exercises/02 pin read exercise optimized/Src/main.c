/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/**** Address of the Clock control register (AHB1ENR) ****/
#define	AHB1ENR 0x40023830 // base address of RCC (register control clock) register (0x40023800) + offset of AHB1ENR reg (0x30) Address of the clock control register in which GIOD D enable bit is in it

/**** Address of the GPID mode register (used to control mode) ****/
#define GPIOD_MODE 0x40020C00 // base address of GPIO D registers (0x40020C00) + offset of mode register (0x00) -> make the mode of pin 12 (bit 24 to 1 & 25 to 0) so that we can use them as general purpose output

/**** Address of the GPIOD output data register (used to write) ****/
#define GPIOD_OUTPUT 0x40020C14 // base address of GPIO D registers (0x40020C00) + offset of output register (0x14) -> make the mode of pin 12 (bit 24 to 1 & 25 to 0) so that we can use them as general purpose output

/**** Address of the GPIA mode register (used to control mode) ****/
#define GPIOA_MODE 0x40020000 // base address of GPIO A registers (0x4002 0000) + offset of mode register (0x00) -> make the mode of pin 0 (bit 0 to 0 & 1 to 0) so that we can use them as general input

/**** Address of the GPIOA input data register (used to read) ****/
#define GPIOA_OUTPUT 0x40020010 // base address of GPIO D registers (0x4002 0000) + offset of input register (0x10) -> read the value of bit 0



#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	uint32_t volatile *pClkCtrlReg = (uint32_t*)AHB1ENR;
	uint32_t volatile *pPortDModeReg = (uint32_t*)GPIOD_MODE;
	uint32_t volatile *pPortDOutputReg = (uint32_t*)GPIOD_OUTPUT;

	uint32_t volatile *pPortAModeReg = (uint32_t*)GPIOA_MODE;
	uint32_t volatile *pPortAInputtReg = (uint32_t*)GPIOA_OUTPUT;

	// 1.enable the clock for GPIOD and GPIOA peripheral in the AHB1ENR (Set the 3rd bit position)
	*pClkCtrlReg |= (1 << 3);
	*pClkCtrlReg |= (1 << 0);


	// 2.configure 12th mode of the portD IO pin as output
	*pPortDModeReg &= ~(3 << 24);
	*pPortDModeReg |= (1 << 24);

	// 3.configure 0th mode of the portA IO pin as input
	*pPortAModeReg &= ~(3 << 0);

	while(1){
		// get the value from port A
		uint8_t val = (uint8_t)(*pPortAInputtReg & 1);

		if(val == 1)
			*pPortDOutputReg |= (1 << 12);
		else
			*pPortDOutputReg &= ~(1 << 12);

	}
    /* Loop forever */
	for(;;);
}
