/*
 * stm32f407_i2c_driver.c
 *
 *  Created on: 20 Jul 2022
 *      Author: abdo
 */

#include "stm32f407_i2c_driver.h"



static void I2C_generateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);


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


	// configure the FREQ field of CR2
	tempreg |= RCC_GetPCLK1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3F);

	// program the device own address
	tempreg = 0;
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
void I2C_MAsterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint8_t len, uint8_t slaveAddr, uint8_t Sr)
{
	// 1. Generate the START condition
	I2C_generateStartCondition(pI2CHandle->pI2Cx);

	// 2. Confirm that start generation is completed by checking the SB flag in the SR1
	// note: until SB is cleared SCL will be stretched (pulled to LOW)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SB_FLAG));

	// 3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits)
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, slaveAddr);

	// 4. confirm that the address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_ADDR_FLAG));

	// 5. clear the ADDR flag according to its software sequence
	// Note : Until ADDR is cleared SCL will be stretched (pulled to LOW)
	I2C_ClearADDRFlag(pI2CHandle);

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
	if(Sr == I2C_DISABLE_SR)
		I2C_generateStopCondition(pI2CHandle->pI2Cx);


}
static void I2C_generateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr)
{
	slaveAddr = slaveAddr << 1;
	slaveAddr &= ~(1 << 0);
	pI2Cx->DR = slaveAddr;
}
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr)
{
	slaveAddr = slaveAddr << 1;
	slaveAddr |= (1 << 0);
	pI2Cx->DR = slaveAddr;
}
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle)
{
	uint32_t dummyRead;
	// check for device mode
	if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
	{
		// device is in master mode
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxSize == 1)
			{
				// first disable the ack
				I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);

				// clear the ADDR flag (read SR1, read SR2)
				dummyRead = pI2CHandle->pI2Cx->SR1;
				dummyRead = pI2CHandle->pI2Cx->SR2;
				(void)dummyRead;
			}
		}
		else
		{
			// clear the ADDR flag (read SR1, read SR2)
			dummyRead = pI2CHandle->pI2Cx->SR1;
			dummyRead = pI2CHandle->pI2Cx->SR2;
			(void)dummyRead;
		}
	}
	else
	{
		// device is in slave mode

		// clear the ADDR flag (read SR1, read SR2)
		dummyRead = pI2CHandle->pI2Cx->SR1;
		dummyRead = pI2CHandle->pI2Cx->SR2;
		(void)dummyRead;
	}


}
void I2C_generateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}

/************************************************************
 *@fn					- I2C_MasterReceiveData
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
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint8_t len, uint8_t slaveAddr, uint8_t Sr)
{
	// 1. Generate the START condition
	I2C_generateStartCondition(pI2CHandle->pI2Cx);

	// 2. confirm that start generation is completed by checking the SB flag in the SR1
	// 	Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SB_FLAG));

	// 3. Send the address of the slave with r/nw bit set to R(1) (total 8 bits)
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, slaveAddr);

	// 4. wait until address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_ADDR_FLAG));

	// procedure to read only 1 byte from slave
	if(len == 1)
	{
		// Disable Acking
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

		// clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		// wait until RXNE becomes 1
		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_RxNE_FLAG));

		// generate STOP condition
		if(Sr == I2C_DISABLE_SR)
			I2C_generateStopCondition(pI2CHandle->pI2Cx);

		// read data in to buffer
		*pRxBuffer = pI2CHandle->pI2Cx->DR;

	}

	// procedure to read data from slave when len > 1
	if(len > 1)
	{
		// clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		// read the data until len becomes zero
		for(uint32_t i = len; i > 0; i--)
		{
			// wait until RXNE becomes 1
			while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_RxNE_FLAG));

			if(i == 2) // if last 2 bytes are remaining
			{
				// clear the ack bit
				I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

				// generate STOP condition
				if(Sr == I2C_DISABLE_SR)
					I2C_generateStopCondition(pI2CHandle->pI2Cx);
			}

			// read the data from data register in to buffer
			*pRxBuffer = pI2CHandle->pI2Cx->DR;

			// increment the buffer address
			pRxBuffer++;
		}
	}

	// re-enable ACKings
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
}




/************************************************************
 *@fn					- I2C_MAsterSendDataIT
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
uint8_t I2C_MAsterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint8_t len, uint8_t slaveAddr, uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TXLen = len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = slaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_generateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);

	}

	return busystate;
}


/************************************************************
 *@fn					- I2C_MasterReceiveDataIT
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
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint8_t len, uint8_t slaveAddr, uint8_t Sr)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RXLen = len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = len; //Rxsize is used in the ISR code to manage the data reception
		pI2CHandle->DevAddr = slaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_generateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);

	}

	return busystate;
}

void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx, uint8_t data)
{
	pI2Cx->DR = data;
}

uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx)
{
	return (uint8_t)pI2Cx->DR;
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
  *@fn					- I2C_ManageAcking
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
 void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
 {
	 if(EnorDi == I2C_ACK_ENABLE)
	 {
		 // enable the ack
		 pI2Cx->CR1 |= (1 << I2C_CR1_ACK);
	 }
	 else
	 {
		 // disable the ack
		 pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);
	 }

 }

 /*********************************************************************
  * @fn      		  - I2C_EV_IRQHandling
  *
  * @brief             -
  *
  * @param[in]         -
  * @param[in]         -
  * @param[in]         -
  *
  * @return            -
  *
  * @Note              -  Interrupt handling for different I2C events (refer SR1)

  */
 void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
 {

 	//Interrupt handling for both master and slave mode of a device
	 uint32_t temp1, temp2, temp3;

	 temp1 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN);
	 temp2 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN);

	 temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_SB);

 	//1. Handle For interrupt generated by SB event
 	//	Note : SB flag is only applicable in Master mode
	 if(temp1 && temp3)
	 {
		 // interrupt is generated because of SB event
		 // this block will not be executed in slave mode because for slave SB is always zero
		 // in this block lets execute the address phase
		 if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		 {
			 I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		 }
		 else if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		 {
			 I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		 }
	 }

	 temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ADDR);
 	//2. Handle For interrupt generated by ADDR event
 	//Note : When master mode : Address is sent
 	//		 When Slave mode   : Address matched with own address
	 if(temp1 && temp3)
	 {
		 // interrupt is generated because of ADDR event
		 I2C_ClearADDRFlag(pI2CHandle);
	 }

	 temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BTF);
 	//3. Handle For interrupt generated by BTF(Byte Transfer Finished) event
	 if(temp1 && temp3)
	 {
		 // interrupt is generated because of BTF event
		 if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		 {
			 // make sure that TXE is also set
			 if(pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TxE))
			 {
				 // BTF, TXE = 1 , close the transmission

				 if(pI2CHandle->TXLen == 0)
				 {
					 // 1. generate the STOP condition
					 if(pI2CHandle->Sr == I2C_DISABLE_SR)
						 I2C_generateStartCondition(pI2CHandle->pI2Cx);

					 // 2. reset all the member elements of the handle structure.
					 I2C_CloseSendData(pI2CHandle);

					 // 3. notify the application about the transmission complete
					 I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);
				 }

			 }
		 }
		 else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		 {
			 // nothing to do
		 }
	 }

	 temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_STOPF);
 	//4. Handle For interrupt generated by STOPF event
 	// Note : Stop detection flag is applicable only slave mode . For master this flag will never be set
	 if(temp1 && temp3)
	 {
		 // STOPF flag is set

		 // clear the STOP (i.e -> 1) read SR1 2) write to CR1)
		 pI2CHandle->pI2Cx->CR1 |= 0x0000;

		 // notify the application that STOP is detected
		 I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);
	 }

	 temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TxE);
 	//5. Handle For interrupt generated by TXE event
	 if(temp1 && temp2 && temp3)
	 {
		 // check for device mode
		 if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		 {
			 // TXE flag is set
			 // we have to do data transmission
			 if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
			 {
				 I2C_MasterHandleTXEInterrupt(pI2CHandle);
			 }
		 }
		 else
		 {
			 //slave
			 // make sure that the slave is really in transmitter mode
			 if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA))
				 I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_REQ);
		 }

	 }

	 temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RxNE);
 	//6. Handle For interrupt generated by RXNE event
	 if(temp1 && temp2 && temp3)
	 {
		 // check for device mode
		 if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		 {
			 // the device is master

			 // RXNE flag is set
			 if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
			 {
				 I2C_MasterHandleRXNEInterrupt(pI2CHandle);
			 }
		 }
		 else
		 {
			 //slave
			 // make sure that the slave is really in receiver mode
			 if(!(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA)))
				 I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_RCV);
		 }
	 }
 }
 static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle)
 {
	 if(pI2CHandle->TXLen > 0)
	 {
		 // 1. load the data in to DR
		 pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);

		 // 2. decrement the TxLen
		 pI2CHandle->TXLen--;

		 // 3. increment the buffer address
		 pI2CHandle->pTxBuffer++;
	 }
 }
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle)
{
	 // we have to do the data reception
	 if(pI2CHandle->RxSize == 1)
	 {
		 *pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		 pI2CHandle->RXLen--;
	 }

	 if(pI2CHandle->RxSize > 1)
	 {
		 if(pI2CHandle->RXLen == 2)
		 {
			 // clear the ack bit
			 I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);
		 }
		 // read DR
		 *pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		 pI2CHandle->RXLen--;
		 pI2CHandle->pRxBuffer++;
	 }

	 if(pI2CHandle->RXLen == 0)
	 {
		 // close the I2C data reception and notify the application

		 // 1. generate the STOP condition
			if(pI2CHandle->Sr == I2C_DISABLE_SR)
				I2C_generateStopCondition(pI2CHandle->pI2Cx);

		 // 2. close the I2C rx
			I2C_closeReceiveData(pI2CHandle);

		 // 3. Notify the application
			I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
	 }
}

void I2C_closeReceiveData(I2C_Handle_t *pI2CHandle)
{
	// Implement the code to disable ITBUFEN Control Bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	// Implement the code to disable ITEVFEN Control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RXLen = 0;
	pI2CHandle->RxSize = 0;
	I2C_ManageAcking(pI2CHandle->pI2Cx, ENABLE);
}

void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
	// Implement the code to disable ITBUFEN Control Bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	// Implement the code to disable ITEVFEN Control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TXLen = 0;
}

 /*********************************************************************
  * @fn      		  - I2C_ER_IRQHandling
  *
  * @brief             -
  *
  * @param[in]         -
  * @param[in]         -
  * @param[in]         -
  *
  * @return            -
  *
  * @Note              - Complete the code also define these macros in the driver
 						header file
 						#define I2C_ERROR_BERR  3
 						#define I2C_ERROR_ARLO  4
 						#define I2C_ERROR_AF    5
 						#define I2C_ERROR_OVR   6
 						#define I2C_ERROR_TIMEOUT 7

  */
 void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle)
 {
		uint32_t temp1,temp2;

	    //Know the status of  ITERREN control bit in the CR2
		temp2 = (pI2CHandle->pI2Cx->CR2) & ( 1 << I2C_CR2_ITERREN);


	/***********************Check for Bus error************************************/
		temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1<< I2C_SR1_BERR);
		if(temp1  && temp2 )
		{
			//This is Bus error

			//Implement the code to clear the buss error flag
			pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_BERR);

			//Implement the code to notify the application about the error
		   I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
		}

	/***********************Check for arbitration lost error************************************/
		temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_ARLO );
		if(temp1  && temp2)
		{
			//This is arbitration lost error

			//Implement the code to clear the arbitration lost error flag
			pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_ARLO);

			//Implement the code to notify the application about the error
			I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_ARLO);
		}

	/***********************Check for ACK failure  error************************************/

		temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_AF);
		if(temp1  && temp2)
		{
			//This is ACK failure error

		    //Implement the code to clear the ACK failure error flag
			pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_AF);

			//Implement the code to notify the application about the error
			I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_AF);
		}

	/***********************Check for Overrun/underrun error************************************/
		temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_OVR);
		if(temp1  && temp2)
		{
			//This is Overrun/underrun

		    //Implement the code to clear the Overrun/underrun error flag
			pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_OVR);

			//Implement the code to notify the application about the error
			I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_OVR);
		}

	/***********************Check for Time out error************************************/
		temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_TIMEOUT);
		if(temp1  && temp2)
		{
			//This is Time out error

		    //Implement the code to clear the Time out error flag
			pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_TIMEOUT);

			//Implement the code to notify the application about the error
			I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_TIMEOUT);
		}
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
 __weak void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
 {

 }


 void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
 {
	 if(EnorDi == ENABLE)
	 {
		 pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);
		 pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);
		 pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	 }
	 else
	 {
		 pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);
		 pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);
		 pI2Cx->CR2 &= ~(1 << I2C_CR2_ITERREN);
	 }
 }
