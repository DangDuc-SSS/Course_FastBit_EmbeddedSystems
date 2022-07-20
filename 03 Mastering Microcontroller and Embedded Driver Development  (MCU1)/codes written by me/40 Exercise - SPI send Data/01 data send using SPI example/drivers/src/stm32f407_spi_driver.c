/*
 * stm32f407_spi_driver.h
 *
 *  Created on: 18 Jul 2022
 *      Author: abdo
 */

#include "stm32f407_spi_driver.h"

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
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{

}

