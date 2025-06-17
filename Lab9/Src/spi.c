/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab7
 * Dependencies: st32f411.h, spi.h
 * Description: SPI2 functions
 */

#include "stm32f411.h"

#include "spi.h"

/**
 * SpiInit - Initializes SPI2 and sets GPIO ports needed to AF (PB15 & PB13)
 * Args - N/A
 * Return - N/A
 */
void SpiInit() {
	// enable spi2 & gpiob
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	// clear and set PB15 (MOSI) and PB13 (SCK) to alternate function
	GPIOB->MODER = (GPIOB->MODER & ~((0x3 << 30) | (0x3 << 26))) | (0x2 << 30 | 0x2 << 26);
	GPIOB->AFR[1] = (GPIOB->AFR[1] & ~((0xF << 28) | (0xF << 20))) | (0x5 << 28) | (0x5 << 20);

	// configure SPI2
	SPI2->CR1 = SPI_CR1_DFF | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR;
	SPI2->CR2 = 0;
}

/**
 * SpiWrite - Writes 16 bits of data to SPI data register to be transmitted, waits until tranmission is complete
 * Args - data: 2 bytes of data to be sent
 * Return - N/A
 */
void SpiWrite(uint16_t data) {
	while (!(SPI2->SR & SPI_SR_TXE)); // busy wait until TX buffer is empty
	SPI2->CR1 |= SPI_CR1_SPE;		  // turn on SPI2
	SPI2->DR = data;				  // write data to SPI data register
	while (SPI2->SR & SPI_SR_BSY);	  // busy wait
	SPI2->CR1 &= ~SPI_CR1_SPE;		  // turn off SPI2
}
