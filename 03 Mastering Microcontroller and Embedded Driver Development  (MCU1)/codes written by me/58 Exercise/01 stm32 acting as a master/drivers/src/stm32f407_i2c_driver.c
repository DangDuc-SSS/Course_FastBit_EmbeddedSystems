/*
 * stm32f407_i2c_driver.c
 *
 *  Created on: 20 Jul 2022
 *      Author: abdo
 */

#include "stm32f407_i2c_driver.h"

uint16_t AHB_PreScalar[8] = {2, 4, 8, 16, 64, 128, 256, 512};
uint16_t APB1_PreScalar[4] = {2, 4, 8, 16};

static void I2C_generateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr);
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx);
static void I2C_generateStopCondition(I2C_RegDef_t *pI2Cx);

/************************************************************
 *@fn					- I2C_PeripheralClockControl
 *
 *@brief  				-
 *
 *
 *@param[in]			-
 *@param[in]			-
 *@param[in]			-
 *
 *
 *@return				-
 *
 *@Note					-
 *
 */
void I2C_PeripheralClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}
		else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}
		else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}

	}
	else
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_DI();
		}
		else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_DI();
		}
		else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_DI();
		}
	}
}

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
/************************************************************
 *@fn					- I2C_Init
 *
 *@brief  				-
 *
 *
 *@param[in]			-
 *@param[in]			-
 *@param[in]			-
 *
 *
 *@return				-
 *
 *@Note					-
 *
 */
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	// enable the clock for the i2cx peripheral
	I2C_PeripheralClockControl(pI2CHandle->pI2Cx, ENABLE);

	uint32_t tempreg = 0;

	//ack control bit
	tempreg |= (pI2CHandle->I2C_Config.I2C_ACKControl << I2C_CR1_ACK);
	pI2CHandle->pI2Cx->CR1 = tempreg;

	// configure the FREQ field of CR2
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3F);

	// program the device own address
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	tempreg |= (1 << 14);
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	// CCR calculation
	uint16_t ccr_value = 0;
	tempreg = 0;

	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		// mode is standard mode
		ccr_value = RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		tempreg |= (ccr_value & 0xFFF);
	}
	else
	{
		// mode is fast mode
		tempreg |= (1 << 15);
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		}
		else
		{
			ccr_value = RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		}
		tempreg |= ccr_value;
	}
	pI2CHandle->pI2Cx->CCR = tempreg;

	// TRISE Configuration
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		// mode is standard mode
		tempreg = (RCC_GetPCLK1Value() / 1000000U) + 1;
	}
	else
	{
		// mode is fast mode
		tempreg = ((RCC_GetPCLK1Value() * 300) / 1000000000U) + 1;
	}
	pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);


}

/************************************************************
 *@fn					- I2C_DeInit
 *
 *@brief  				-
 *
 *
 *@param[in]			-
 *@param[in]			-
 *@param[in]			-
 *
 *
 *@return				-
 *
 *@Note					-
 *
 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
	if(pI2Cx == I2C1)
	{
		I2C1_REG_RESET();
	}
	else if(pI2Cx == I2C2)
	{
		I2C2_REG_RESET();
	}
	else if(pI2Cx == I2C3)
	{
		I2C3_REG_RESET();
	}
}

/************************************************************
 *@fn					- I2C_MAsterSendData
 *
 *@brief  				-
 *
 *
 *@param[in]			-
 *@param[in]			-
 *@param[in]			-
 *
 *
 *@return				-
 *
 *@Note					-
 *
 */
void I2C_MAsterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint8_t len, uint8_t slaveAddr)
{
	// 1. Generate the START condition
	I2C_generateStartCondition(pI2CHandle->pI2Cx);

	// 2. Confirm that start generation is completed by checking the SB flag in the SR1
	// note: until SB is cleared SCL will be stretched (pulled to LOW)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SB_FLAG));

	// 3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits)
	I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, slaveAddr);

	// 4. confirm that the address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_ADDR_FLAG));

	// 5. clear the ADDR flag according to its software sequence
	// Note : Until ADDR is cleared SCL will be stretched (pulled to LOW)
	I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

	// 6. Send the data until Len becomes 0
	while(len > 0)
	{
		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TxE_FLAG)); // wait till TXE is set
		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer++;
		len--;
	}

	// 7. When Len becomes zero , wait for TXE = 1 and BTF = 1 before generating the STOP condition
	// NOTE: TXE = 1 , BTF = 1, means that both SR and DR are empty and next transition should begin
	// when BTF = 1 , SCL will be stretched (pulled to LOW)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TxE_FLAG));
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_BTF_FLAG));


	// 8. generate STOP condition and master need no to wait for the completion of STOP condition.
	// Note : generating STOP, automatically clears the BTF
	I2C_generateStopCondition(pI2CHandle->pI2Cx);


}
static void I2C_generateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}
static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr)
{
	slaveAddr = slaveAddr << 1;
	slaveAddr &= ~(1 << 0);
	pI2Cx->DR = slaveAddr;
}
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx)
{
	uint32_t dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR2;
	(void)dummyRead;
}
static void I2C_generateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}
/************************************************************
 *@fn					- I2C_IRQInterruptConfig
 *
 *@brief  				-
 *
 *
 *@param[in]			-
 *@param[in]			-
 *@param[in]			-
 *
 *
 *@return				-
 *
 *@Note					-
 *
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			// program ISER0 register
			*NVIC_ISER0 |= (1 << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber < 64)
		{
			// program ISER1 register
			*NVIC_ISER1 |= (1 << (IRQNumber % 32));
		}
		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			// program ISER2 register
			*NVIC_ISER2 |= (1 << (IRQNumber % 32));
		}
	}
	else
	{
		if(IRQNumber <= 31)
		{
			// program ISER0 register
			*NVIC_ICER0 |= (1 << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber < 64)
		{
			// program ISER1 register
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		}
		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			// program ISER2 register
			*NVIC_ICER1 |= (1 << (IRQNumber % 64));
		}
	}
}

/************************************************************
 *@fn					- I2C_IRQPriorityConfig
 *
 *@brief  				-
 *
 *
 *@param[in]			-
 *@param[in]			-
 *@param[in]			-
 *
 *
 *@return				-
 *
 *@Note					-
 *
 */
 void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint8_t IRQPriority)
 {
		// 1. first lets find out the ipr register
		uint8_t iprx = IRQNumber / 4;
		uint8_t iprx_section = IRQNumber % 4;
		uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

		*(NVIC_PR_BASE_ADDR + iprx) &= ~(0xFF << shift_amount);
		*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
 }

 /************************************************************
  *@fn					- I2C_PeripheralControl
  *
  *@brief  				-
  *
  *
  *@param[in]			-
  *@param[in]			-
  *@param[in]			-
  *
  *
  *@return				-
  *
  *@Note					-
  *
  */
 void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
 {
		if(EnorDi == ENABLE)
		{
			pI2Cx->CR1 |= (1 << I2C_CR1_PE);
		}
		else
		{
			pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
		}
 }


 /************************************************************
  *@fn					- I2C_GetFlagStatus
  *
  *@brief  				-
  *
  *
  *@param[in]			-
  *@param[in]			-
  *@param[in]			-
  *
  *
  *@return				-
  *
  *@Note					-
  *
  */
 uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
 {
		if(pI2Cx->SR1 & FlagName)
			return FLAG_SET;
		else
			return FLAG_RESET;
 }

 /************************************************************
  *@fn					- I2C_ApplicationEventCallback
  *
  *@brief  				-
  *
  *
  *@param[in]			-
  *@param[in]			-
  *@param[in]			-
  *
  *
  *@return				-
  *
  *@Note					-
  *
  */
 void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
 {

 }
