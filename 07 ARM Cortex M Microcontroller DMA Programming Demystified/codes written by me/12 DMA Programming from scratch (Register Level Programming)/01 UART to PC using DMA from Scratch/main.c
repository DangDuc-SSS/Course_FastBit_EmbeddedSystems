#include <stdint.h>
#include "stm32f446xx.h"

void button_init(void);
void uart2_init(void);
void dma1_init(void);
void enable_dma1_stream6(void);
void dma1_interrupt_configuration(void);

void HT_Complete_Callback(void);
void FT_Complete_Callback(void);
void TE_error_Callback(void);
void FE_error_Callback(void);
void DME_error_Callback(void);

void send_some_data(void);

#define BASE_ADDR_OF_GPIOC_PERI	GPIOC
char data_stream[] = "Hello World\r\n";

int main(void)
{
	button_init();
	
	uart2_init();
	
	dma1_init();
	
	dma1_interrupt_configuration();
	
	enable_dma1_stream6();

	while(1);
	
}

void send_some_data(void)
{
	char somedata[] = "Hello World\r\n";
	USART_TypeDef *pUART2;
	uint32_t len = sizeof(somedata);
	
	pUART2 = USART2;
	
	// make sure that in the status register TXE bit is set (TDR is empty)
	// if TXE is 1, put the byte
	for(uint32_t i = 0; i < len; i++)
	{
		// we are waiting for TXE to become 1
		while(!(pUART2->SR & (1 << 7)));
			
		pUART2->DR = somedata[i];
	}

	
}

void button_init(void)
{
	// button is connected to PC13, GPIOC pin number 13.
	GPIO_TypeDef *pGPIOC;
	RCC_TypeDef *pRCC;
	EXTI_TypeDef *pEXTI;
	SYSCFG_TypeDef *pSYSCFG;
	
	pGPIOC = BASE_ADDR_OF_GPIOC_PERI;
	pRCC = RCC;
	pEXTI = EXTI;
	pSYSCFG = SYSCFG;
	
	// 1. enable the clock for the GPIOC peripheral
	pRCC->AHB1ENR |= (1 << 2);
	
	// 2. Keep the GPIO pin in input mode
	pGPIOC->MODER &= (uint32_t)~(0x3 << 26);
	
	// 3. Enable the interrupt over that gpio pin
	pEXTI->IMR |= (1 << 13);
	
	// 4. Enable the clock for SYSCFG
	pRCC->APB2ENR |= (1 << 14);
	
	// 5. configuring the SYSCFG CR4 register
	pSYSCFG->EXTICR[3] &= (uint32_t)~(0xF << 4); // clearing
	pSYSCFG->EXTICR[3] |= (0x2 << 4);	// setting
	
	// 6. Configure the edge detection on that gpio pin
	pEXTI->FTSR |= (1 << 13);
	
	
	// 7. Enable the IRQ related to that gpio pin in NVIC of the processor
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
}

void uart2_init(void)
{
	GPIO_TypeDef *pGPIOA;
	RCC_TypeDef *pRCC;
	USART_TypeDef *pUART2;
	
	pRCC = RCC;
	pGPIOA = GPIOA;
	pUART2 = USART2;
	
	// 1. Enable the periheral clock for the uart2 peripheral
	pRCC->APB1ENR |= (1 << 17);
	
	// 2. Configure the gpio pins for the uart_tx and uart_rx functionality
	// PA2 as TX, PA3 as RX
	
	// first lets configure PA2 as UART2 TX
	
	//2.1 Enable the clock for the GPIOA peripheral
	pRCC->AHB1ENR |= (1 << 0);
	
	//2.2 Change the mode of the PA2 to alternate function mode
	pGPIOA->MODER &= (uint32_t)~(0x3 << 4);
	pGPIOA->MODER |= (0x2 << 4);
	pGPIOA->AFR[0] &= (uint32_t)~(0xF << 8);
	pGPIOA->AFR[0] |= (0x7 << 8);
	
	//2.3 Enable or disable pull-up resistor if requires
	pGPIOA->PUPDR &= (uint32_t)~(0x3 << 4);
	pGPIOA->PUPDR |= (0x1 << 4); // enable the pull-up resistor
	
	// second lets configure PA3 as UART2 RX
	
	//2.4 Change the mode of the PA2 to alternate function mode
	pGPIOA->MODER &= (uint32_t)~(0x3 << 6);
	pGPIOA->MODER |= (0x2 << 6);
	pGPIOA->AFR[0] &= (uint32_t)~(0xF << 12);
	pGPIOA->AFR[0] |= (0x7 << 12);
	
	//2.5 Enable or disable pull-up resistor if requires
	pGPIOA->PUPDR &= (uint32_t)~(0x3 << 6);
	pGPIOA->PUPDR |= (0x1 << 6); // enable the pull-up resistor
	
	// 3. configure the baudrate
	pUART2->BRR = 0x8B;
	
	// 4. configure the data width, no. of stop bits. etc
	// <no configuration required here, we will use default values>
	
	// 5. Enable the TX engine of the uart peripheral
	pUART2->CR1 |= (1 << 3);
	
	// 6. Enable the uart peripheral
	pUART2->CR1 |= (1 << 13);
	
}
void dma1_init(void)
{
	RCC_TypeDef *pRCC;
	DMA_TypeDef *pDMA;
	DMA_Stream_TypeDef *pSTREAM6;
	USART_TypeDef *pUART2;
	uint32_t len = sizeof(data_stream);

	pRCC = RCC;
	pDMA = DMA1;
	pSTREAM6 = DMA1_Stream6;
	pUART2 = USART2;
	
	// 1. enable the peripheral clock for the dma1
	pRCC->AHB1ENR |= (1 << 21);
	
	// 2. identify the stream which is suitable for your peripheral
	// stream 6, channel 4
	
	// 3. identify the channel number on which uart2 peripheral sends dma request
	// channel 4
	pSTREAM6->CR &= (uint32_t)~(0x7 << 25);
	pSTREAM6->CR |= (0x4 << 25);
	
	// 4. Program the source address (memory)
	pSTREAM6->M0AR = (uint32_t)data_stream;
	
	// 5. program the destination address (peripheral)
	pSTREAM6->PAR = (uint32_t)&pUART2->DR;
	
	// 6. program the number of data items to send
	pSTREAM6->NDTR = len;
	
	// 7. the direction of data transfer . m2p, p2m, m2m
	pSTREAM6->CR |= (0x1 << 6);
	
	// 8. program the source and destination data width
	pSTREAM6->CR &= (uint32_t)~(0x3 << 13);
	pSTREAM6->CR &= (uint32_t)~(0x3 << 11);
	
	// 8a. enable memory auto increment
	pSTREAM6->CR |= (1 << 10);
	
	// 9. direct mode or fifo mode
	pSTREAM6->FCR |= (1 << 2); 
	
	// 10. select the fifo threshold
	pSTREAM6->FCR &= (uint32_t)~(0x3 << 0);	// clearing 
	pSTREAM6->FCR |= (0x3 << 0);	// setting
	
	// 11. enable th circular mode if required
	
	// 12. single transfer or burst transfer
	
	// 13. configure the stream priority
	

	
	
}

void enable_dma1_stream6(void)
{
	DMA_Stream_TypeDef *pSTREAM6;
	pSTREAM6 = DMA1_Stream6;
	
	// enable the stream
	pSTREAM6->CR |= (1 << 0);
}

void dma1_interrupt_configuration(void)
{
	DMA_Stream_TypeDef *pSTREAM6;
	pSTREAM6 = DMA1_Stream6;
	
	// 1. let's do Half-transfer (HTIE)
	pSTREAM6->CR |= (1 << 3);
	
	// 2. Transfer Complete (TCIE)
	pSTREAM6->CR |= (1 << 4);
	
	// 3. transfer error (TEIE)
	pSTREAM6->CR |= (1 << 2);
	
	// 4. FIFO overrun/underrun (FEIE)
	pSTREAM6->FCR |= (1 << 7);
	
	// 5. Direct mode error (DMEIE)
	pSTREAM6->CR |= (1 << 1);
	
	// 6. Enable the IRQ for DMA1 stream6 gobal interrupt in NVIC
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
	
}

void HT_Complete_Callback(void)
{
	
}

void FT_Complete_Callback(void)
{
	USART_TypeDef *pUART2;	
	DMA_Stream_TypeDef *pSTREAM6;
	uint32_t len = sizeof(data_stream);
	
	pUART2 = USART2;
	pSTREAM6 = DMA1_Stream6;
	
	pSTREAM6->NDTR = len;
	
	pUART2->CR3 &= (uint32_t)~(1 << 7);
	
	enable_dma1_stream6();
}

__attribute__((noreturn)) void TE_error_Callback(void)
{
	while(1);
}

__attribute__((noreturn)) void FE_error_Callback(void)
{
	while(1);
}
__attribute__((noreturn)) void DME_error_Callback(void)
{
	while(1);
}

