/*
 * stm32f407xx_rcc_driver.h
 *
 *  Created on: 23 Jul 2022
 *      Author: abdo
 */

#ifndef INC_STM32F407XX_RCC_DRIVER_H_
#define INC_STM32F407XX_RCC_DRIVER_H_

#include "stm32f407xx.h"

// this returns the APB1 clock value
uint32_t RCC_GetPCLK1Value(void);

// this returns the APB2 clock value
uint32_t RCC_GetPCLK2Value(void);

uint32_t RCC_GETPLLOutputClock(void);



#endif /* INC_STM32F407XX_RCC_DRIVER_H_ */
