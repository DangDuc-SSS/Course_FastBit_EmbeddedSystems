/*
 * stm32f407_spi_driver.h
 *
 *  Created on: 18 Jul 2022
 *      Author: abdo
 */

#include "stm32f407_spi_driver.h"


static void spi_txe_interrupt_handle(SPI_Handle_t *pHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pHandle);
/************************************************************
 *@fn					- SPI_PeripheralClockControl
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
void SPI_PeripheralClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}

	}
	else
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}
	}
}

/************************************************************
 *@fn					- SPI_PeripheralControl
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
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	}
	else
	{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
	}
}

/************************************************************
 *@fn					- SPI_Init
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
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	// peripheral clock enable
	SPI_PeripheralClockControl(pSPIHandle->pSPIx, ENABLE);

	// first lets configure the SPI_CR1 register

	uint32_t tempreg = 0;

	// 1. configure the device mode
	tempreg |= (pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR);

	// 2. configure the bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//bidi mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//bidi mode should be set
		tempreg |= (1 << SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//bidi mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
		//RXONLY bit must be set
		tempreg |= (1 << SPI_CR1_RXONLY);
	}

	// 3.configure the spi serial clock speed (baud rate)
	tempreg |= (pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR);

	// 4. configure the DFF
	tempreg |= (pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF);

	// 5. configure the CPOL
	tempreg |= (pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL);

	// 6. configure the CPHA
	tempreg |= (pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA);

	// 7. configure the SSM
	tempreg |= (pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM);

	// storing the value in the register
	pSPIHandle->pSPIx->CR1 = tempreg;
}

/************************************************************
 *@fn					- SPI_DeInit
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
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if(pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}
	else if(pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}
	else if(pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
		return FLAG_SET;
	else
		return FLAG_RESET;
}
/************************************************************
 *@fn					- SPI_SendData
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
 *@Note					- this is a blocking call
 *
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len)
{
	while(len > 0)
	{
		// 1. wait until TXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		// 2. check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			// 16 BIT DFF
			// 1. load the data in to the DR
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			(uint16_t*)pTxBuffer++;
			len -= 2;
		}
		else
		{
			// 8 BIT DFF
			pSPIx->DR = *pTxBuffer;
			pTxBuffer++;
			len--;
		}
	}
}

uint8_t SPI_SendDataWithIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len)
{
	uint8_t state = pSPIHandle->TXState;

	if(state != SPI_BUSY_IN_TX)
	{
		// 1. Save the TX buffer address and Len information in some global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TXLen = len;

		// 2. Mark the SPI state as busy in transmission so that no ther code can take over the same SPI peripheral
		//    until transmission is over
		pSPIHandle->TXState = SPI_BUSY_IN_TX;

		// 3. Enable the TXEIE control bit to get interrupt whenever TXE flag is in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);
	}

	// 4. Data transmission will be handled by the ISR code (will implement later)

	return state;
}

/************************************************************
 *@fn					- SPI_RecieveData
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
void SPI_RecieveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len)
{
	while(len > 0)
	{
		// 1. wait until RXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		// 2. check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			// 16 BIT DFF
			// 1. load the data from DR to Rxbuffer address
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			(uint16_t*)pRxBuffer++;
			len -= 2;
		}
		else
		{
			// 8 BIT DFF
			*pRxBuffer = pSPIx->DR;
			pRxBuffer++;
			len--;
		}
	}
}

uint8_t SPI_RecieveDataWithIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len)
{
	uint8_t state = pSPIHandle->RXState;

	if(state != SPI_BUSY_IN_RX)
	{
		// 1. Save the TX buffer address and Len information in some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RXLen = len;

		// 2. Mark the SPI state as busy in transmission so that no ther code can take over the same SPI peripheral
		//    until transmission is over
		pSPIHandle->RXState = SPI_BUSY_IN_RX;

		// 3. Enable the TXEIE control bit to get interrupt whenever TXE flag is in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);
	}

	// 4. Data transmission will be handled by the ISR code (will implement later)

	return state;
}
/************************************************************
 *@fn					- SPI_IRQInterruptConfig
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
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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
 *@fn					- SPI_IRQPriorityConfig
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
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint8_t IRQPriority)
{
	// 1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + iprx) &= ~(0xFF << shift_amount);
	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

/************************************************************
 *@fn					- SPI_IRQHandling
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

static void spi_txe_interrupt_handle(SPI_Handle_t *pHandle)
{
	//  check the DFF bit in CR1
	if(pHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
	{
		// 16 BIT DFF
		// 1. load the data in to the DR
		pHandle->pSPIx->DR = *((uint16_t*)pHandle->pTxBuffer);
		(uint16_t*)pHandle->pTxBuffer++;
		pHandle->TXLen -= 2;
	}
	else
	{
		// 8 BIT DFF
		pHandle->pSPIx->DR = *pHandle->pTxBuffer;
		pHandle->pTxBuffer++;
		pHandle->TXLen--;
	}

	if(! pHandle->TXLen)
	{
		//TXLen is zero, so close the spi transmission and inform the application that TX is over

		// this prevents interrupts from setting up of the TXE flag
		SPI_CLoseTransmission(pHandle);

		SPI_ApplicationEventCallback(pHandle, SPI_EVENT_TX_CMPLT);
	}
}
static void spi_rxne_interrupt_handle(SPI_Handle_t *pHandle)
{
	//  check the DFF bit in CR1
	if(pHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
	{
		// 16 BIT DFF
		// 1. load the data from DR to buffer
		*((uint16_t*)pHandle->pRxBuffer) = pHandle->pSPIx->DR;
		(uint16_t*)pHandle->pRxBuffer++;
		pHandle->RXLen -= 2;
	}
	else
	{
		// 8 BIT DFF
		*pHandle->pRxBuffer = pHandle->pSPIx->DR;
		pHandle->pRxBuffer++;
		pHandle->RXLen--;
	}

	if(! pHandle->RXLen)
	{
		//RXLen is zero, so close the spi reception and inform the application that TX is over

		// this prevents interrupts from setting up of the RXNE flag
		SPI_CLoseReception(pHandle);

		SPI_ApplicationEventCallback(pHandle, SPI_EVENT_RX_CMPLT);
	}
}
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pHandle)
{
	uint8_t temp;
	// 1. Clear the ove flag
	if(pHandle->TXState!= SPI_BUSY_IN_TX)
	{
		temp = pHandle->pSPIx->DR;
		temp = pHandle->pSPIx->SR;
	}
	(void)temp;

	// 2. inform the application
	SPI_ApplicationEventCallback(pHandle, SPI_EVENT_OVR_ERR);
}

void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
	uint8_t temp1, temp2;
	// first lets check for TXE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 && temp2)
	{
		// handle TXE
		spi_txe_interrupt_handle(pHandle);
	}

	// check for RXNE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);
	if(temp1 && temp2)
	{
		// handle RXNE
		spi_rxne_interrupt_handle(pHandle);
	}

	// check for ovr flag
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);
	if(temp1 && temp2)
	{
		// handle RXNE
		spi_ovr_err_interrupt_handle(pHandle);
	}
}



void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}

void SPI_CLoseTransmission(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TXLen = 0;
	pSPIHandle->TXState = SPI_READY;
}

void SPI_CLoseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RXLen = 0;
	pSPIHandle->RXState = SPI_READY;
}


__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{
	// this is a weak implementation, the application may override this function
}
