/*
 * stm32f407_i2c_driver.h
 *
 *  Created on: 20 Jul 2022
 *      Author: abdo
 */

#ifndef INC_STM32F407_I2C_DRIVER_H_
#define INC_STM32F407_I2C_DRIVER_H_

#include "stm32f407xx.h"

/*
 * Configuration structure for I2Cx peripheral
 */
typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint8_t  I2C_DeviceAddress;
	uint8_t  I2C_ACKControl;
	uint16_t I2C_FMDutyCycle;
}I2C_Config_t;


/*
 * Handle structure for I2Cx peripheral
 */
typedef struct
{
	I2C_RegDef_t *pI2Cx;
	I2C_Config_t  I2C_Config;
	uint8_t *pTxBuffer;		/* < To store the app. TX buffer address >*/
	uint8_t *pRxBuffer;		/* < To store the app. RX buffer address >*/
	uint32_t TXLen;			/* < To store the TX len >*/
	uint32_t RXLen;			/* < To store the RX len >*/
	uint8_t  TxRxState;		/* < To store the communication state >*/
	uint8_t  DevAddr;		/* < To store the slave/device address >*/
	uint32_t RxSize;		/* < To store the RX size >*/
	uint8_t  Sr;			/* < To store repeated start value >*/
}I2C_Handle_t;

/*
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM		100000
#define I2C_SCL_SPEED_FM4K		400000
#define I2C_SCL_SPEED_FM2K		200000

/*
 * @I2C_ACKControl
 */
#define I2C_ACK_ENABLE			1
#define I2C_ACK_DISABLE			0

/*
 * @I2C_FMDutyCycle
 */
#define I2C_FM_DUTY_2			0
#define I2C_FM_DUTY_16_9		1

/*
 * I2C application status
 */
#define I2C_READY							0
#define I2C_BUSY_IN_RX						1
#define I2C_BUSY_IN_TX						2

/*
 * I2C related status flags definitions
 */
#define I2C_SB_FLAG							(1 << I2C_SR1_SB)
#define I2C_ADDR_FLAG						(1 << I2C_SR1_ADDR)
#define I2C_BTF_FLAG						(1 << I2C_SR1_BTF)
#define I2C_ADD10_FLAG						(1 << I2C_SR1_ADD10)
#define I2C_STOPF_FLAG						(1 << I2C_SR1_STOPF)
#define I2C_RxNE_FLAG						(1 << I2C_SR1_RxNE)
#define I2C_TxE_FLAG						(1 << I2C_SR1_TxE)
#define I2C_BERR_FLAG						(1 << I2C_SR1_BERR)
#define I2C_ARLO_FLAG						(1 << I2C_SR1_ARLO)
#define I2C_AF_FLAG							(1 << I2C_SR1_AF)
#define I2C_OVR_FLAG						(1 << I2C_SR1_OVR)
#define I2C_PECERR_FLAG						(1 << I2C_SR1_PECERR)
#define I2C_TIMEOUT_FLAG					(1 << I2C_SR1_TIMEOUT)
#define I2C_SMBALERT_FLAG					(1 << I2C_SR1_SMBALERT)

#define I2C_MSL_FLAG						(1 << I2C_SR2_MSL)
#define I2C_BUSY_FLAG						(1 << I2C_SR2_BUSY)
#define I2C_TRA_FLAG						(1 << I2C_SR2_TRA)
#define I2C_GENCALL_FLAG					(1 << I2C_SR2_GENCALL)
#define I2C_SMBDEFAULT_FLAG					(1 << I2C_SR2_SMBDEFAUL)
#define I2C_SMBHOST_FLAG					(1 << I2C_SR2_SMBHOST)
#define I2C_DUALF_FLAG						(1 << I2C_SR2_DUALF)
#define I2C_PEC_FLAG						(0xFF << I2C_SR2_PEC)

#define I2C_DISABLE_SR						RESET
#define I2C_ENABLE_SR						SET


/*
 * I2C application event macros
 */
#define I2C_EV_TX_CMPLT						0
#define I2C_EV_STOP							2
#define I2C_EV_RX_CMPLT						1


/*
 * I2C application error macros
 */
#define I2C_ERROR_BERR  					3
#define I2C_ERROR_ARLO  					4
#define I2C_ERROR_AF    					5
#define I2C_ERROR_OVR   					6
#define I2C_ERROR_TIMEOUT 					7

/*********************************************************************************************************************
 * 									APIs supported by this driver
 * 				for more information about the APIs check the function definitions
**********************************************************************************************************************/

/*
 * Peripheral Clock setup
 */
void I2C_PeripheralClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/*
 * Data Send and Receive
 */
void I2C_MAsterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint8_t len, uint8_t slaveAddr, uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint8_t len, uint8_t slaveAddr, uint8_t Sr);

uint8_t I2C_MAsterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint8_t len, uint8_t slaveAddr, uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint8_t len, uint8_t slaveAddr, uint8_t Sr);

void I2C_closeReceiveData(I2C_Handle_t *pI2CHandle);
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);

/*
 * IRQ configuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint8_t IRQPriority);
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);

/*
 * other Peripheral Control APIs
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);
void I2C_generateStopCondition(I2C_RegDef_t *pI2Cx);

/*
 * Application Callback
 */
__weak void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);


#endif /* INC_STM32F407_I2C_DRIVER_H_ */
