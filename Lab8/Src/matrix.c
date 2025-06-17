/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab7
 * Dependencies: stm32f411.h, string.h, spi.h, matrix.h
 * Description: Functions to write to LED array using SPI
 */

#include <string.h>

#include "stm32f411.h"
#include "spi.h"

#include "matrix.h"

// Private 8x8 bit array used to store on and off pixel locations locally
static uint8_t frameBuffer[8] = {0};

/**
 * cs_low - Private function to set chip select signal (PB12) low
 */
static void cs_low() {
	GPIOB->ODR &= ~(1 << 12);
}

/**
 * cs_high - Private function to set chip select signal (PB12) high
 */
static void cs_high() {
	GPIOB->ODR |= 1 << 12;
}

/**
 * MatrixUpdate - Private function to write frameBuffer to LED matrix using SPI
 * Args - N/A
 * Return - N/A
 */
static void MatrixUpdate() {
	for (uint8_t row = 1; row < 9; row++) {
		cs_low();
		SpiWrite((row << 8) + frameBuffer[row-1]);
		cs_high();
	}
}

/**
 * MatrixInit - Sets up GPIO port and SPI2 for matrix use
 * Args - N/A
 * Return - N/A
 */
void MatrixInit() {
	// configure cs pin as gpio output
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER = (GPIOB->MODER & ~(0x3 << 24)) | (0x1 << 24);

	SpiInit();

	cs_low(); SpiWrite(0x0F00);	cs_high(); // Disable test mode
	cs_low(); SpiWrite(0x0900);	cs_high(); // No decode
	cs_low(); SpiWrite(0x0A0F);	cs_high(); // Brightness = max
	cs_low(); SpiWrite(0x0B07);	cs_high(); // Scan limit, all 8 rows
	cs_low(); SpiWrite(0x0C01);	cs_high(); // Exit shutdown
}

/**
 * MatrixSetPixel - turns on pixel at position (x,y)
 * Args - x: x-position of pixel
 * 		  y: y-position of pixel
 * Return - N/A
 */
void MatrixSetPixel(uint8_t x, uint8_t y) {
	// if out of bounds, exit early
	if (x > 7 || y > 7) return;

	frameBuffer[y] |= 1 << x;	// set 1 to pixel location within row
	MatrixUpdate();
}

/**
 * MatrixClearPixel - turns pixel off at position (x,y)
 * Args - x: x-position of pixel
 * 		  y: y-position of pixel
 * Return - N/A
 */
void MatrixClearPixel(uint8_t x, uint8_t y) {
	// if out of bounds, exit early
	if (x > 7 || y > 7) return;

	frameBuffer[y] &= ~1 << x;	// clear pixel location within row
	MatrixUpdate();
}

/**
 * MatrixClear - clears entire LED matrix
 * Args - N/A
 * Return - N/A
 */
void MatrixClear() {
	// sets entire array to 0, may be possible to do using *frameBuffer = 0;
	memset(frameBuffer, 0, sizeof(frameBuffer));
	MatrixUpdate();
}
