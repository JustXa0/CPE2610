/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab4
 * Dependencies: tim.h, gpio.h, delay.h, keypad.h
 * Description: Functions to read from keypad
 */

#include "tim.h"
#include "gpio.h"
#include "delay.h"
#include "keypad.h"

static volatile gpio_s *const gpioc = (volatile gpio_s *const) GPIO_C;

/**
 * Row[4..1] = PC[7..4]
 * Col[4..1] = PC[3..0]
 */

/**
 * KeypadInit - initializes keypad interface
 * Args - N/A
 * Return - N/A
 */
void KeypadInit() {
	// enable gpioc port
	volatile uint32_t *const rccAbh1enrAddr = (volatile uint32_t *const) RCC_AHB1ENR;
	*rccAbh1enrAddr |= (1 << 3);
	// set cols and rows to input (default state)
	gpioc->moder = (gpioc->moder & ~(0xFFFF));
	// set cols and rows to pull-up resistors (active low)
	gpioc->pupdr = ((gpioc->pupdr & ~(0xFFFF)) | 0x5555);
}

/**
 * KeypadStatus - non-blocking function to read and return position of key pressed
 * Args - N/A
 * Return - 1-16 -> based on position of key pressed, 0 if no key was pressed during the time of check
 */
uint8_t KeypadStatus() {
	gpioc->moder = ((gpioc->moder & ~(0xFF)) | 0x55);
	SystemMicro(5);

	uint8_t rows = (gpioc->idr & 0xF0) >> 4;
	uint8_t row;
	uint8_t result;
	if (rows == 0b1111) {
		return 0;
	} else if (rows == 0b1110) {
		result = 0; // Row 1, pin 5
	} else if (rows == 0b1101) {
		result = 1; // Row 2, pin 6
	} else if (rows == 0b1011) {
		result = 2; // Row 3, pin 7
	} else if (rows == 0b0111) {
		result = 3; // Row 4, pin 8
	}

	result *= 4;

	// Flip inputs and outputs on rows and columns, do the same thing
	// Set Rows to output, cols to input
	gpioc->moder = ((gpioc->moder & ~(0xFFFF)) | (0x55 << 8));
	SystemMicro(5); // Pause to let hardware catch up
	uint8_t columns = (gpioc->idr & 0xF);
	uint8_t col;

	if (columns == 0b1110) {
		result += 1; // Column 1, pin 1
	} else if (columns == 0b1101) {
		result += 2; // Column 2, pin 2
	} else if (columns == 0b1011) {
		result += 3; // Column 3, pin 3
	} else  if (columns == 0b0111) {
		result += 4; // Column 4, pin 4
	}

	gpioc->moder = ((gpioc->moder & ~(0xFFFF)) | (0x55));

	return result;
}

/**
 * KeypadGetKey - blocking function to read and return position of key pressed
 * 				  handles de-bounce
 * Args - N/A
 * Return - 1-16 -> based on position of key pressed
 */
uint8_t KeypadGetKey() {
	while (KeypadStatus() == 0);
	SystemMilli(10);
	uint8_t key = KeypadStatus();
	while (KeypadStatus() != 0);
	SystemMilli(10);
	return key;
}
