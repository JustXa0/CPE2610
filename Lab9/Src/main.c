/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab9
 * Dependencies: stdbool.h, stdlib.h, stm32f411.h, temp.h, keypad.h, lcd.h, piezo.h, accel.h
 * Description: Program main entry point
 */

#define F_CPU 16000000UL

#include <stdbool.h>
#include <stdlib.h>

#include "stm32f411.h"
#include "temp.h"
#include "keypad.h"
#include "lcd.h"
#include "piezo.h"
#include "accel.h"

// flag to show error event occurred
static volatile bool watchdog = false;

// secondary flags to describe error event
static volatile bool lower = false;
static volatile bool higher = false;
static volatile bool movement = false;

// flag to trigger LCD update event
static volatile bool updateLcd = false;

// flag to prevent incorrect comparisons with accelerometer
static volatile bool secondRead = false;

// flag to prevent multiple piezo notes at same time
static volatile bool newNote = false;

// variables used to store ADC threshold values for temperature sensor
static volatile uint16_t lowerAdc = 0;
static volatile uint16_t higherAdc = 0;

// variable to store ADC data register for processing
volatile uint16_t data = 0;

// array used to convert uint to chars for numbers
static const char deci[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

int main() {
	// main initialization
	LcdSetup();
	LcdInit();
	KeypadInit();
	PiezoInit();
	ADXL345Init();

	uint8_t key = 0;
	uint16_t passcode = 0;

	// variables to store accelerometer position
	int16_t x = 0, y = 0, z = 0;

	// variables for comparison
	int16_t lastX = 0, lastY = 0, lastZ = 0;

	/**Lower Temperature Threshold**/

	LcdWriteStr("ENT lower temp:");
	LcdSetPosition(1, 0);
	LcdWriteStr("___");
	LcdSetPosition(1, 0);
	LcdSetDisplay(true, false, false);
	for (uint8_t i = 0; i < 3; i++) {
		while((key = KeypadGetKey()) == 0);
		LcdWriteChar(key);
		lowerAdc = (10 * lowerAdc) + (key - '0');
	}

	/**Higher Temperature Threshold**/

	LcdClear();
	LcdWriteStr("ENT higher temp:");
	LcdSetPosition(1,0);
	LcdWriteStr("___");
	LcdSetPosition(1, 0);
	LcdSetDisplay(true, false, false);
	for (uint8_t i = 0; i < 3; i++) {
		while ((key = KeypadGetKey()) == 0);
		LcdWriteChar(key);
		higherAdc = (10 * higherAdc) + (key - '0');
	}

	// converting given temperatures to ADC values
	lowerAdc = ConvertFAdc(lowerAdc);
	higherAdc = ConvertFAdc(higherAdc);

	/**Setting Passcode**/

	LcdClear();
	LcdWriteStr("ENT CODE: ____");
	LcdSetPosition(0, 10);
	LcdSetDisplay(true, false, false);
	for (uint8_t i = 0; i < 4; i++) {
		while ((key = KeypadGetKey()) == 0);
		LcdWriteChar(key);
		passcode = (10 * passcode) + (key - '0');
	}

	LcdClear();
	TempInit();

	// start TIM5 for LCD updates
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	NVIC->ISER[1] |= 1 << 18;

	LcdWriteStr("Temp:");

	/**TIM SETUP**/

	TIM5->CNT = 0;
	TIM5->PSC = 15999;
	TIM5->ARR = 499;
	TIM5->EGR = 1;
	TIM5->DIER |= TIM_DIER_UIE;
	TIM5->CR1 |= TIM_CR1_CEN;

	while (1) {
 		if (watchdog) { // something was detected, stop timer and handle here
 			TIM5->CR1 &= ~TIM_CR1_CEN;
 			LcdClear();

 			if (higher) { // temperature exceeded upper threshold, handle event
 				LcdWriteStr("TEMP TOO HIGH");
 				Play(2000, 1000);
 				newNote = true;
 				int16_t input = -1;	// -1 so if user gives code of 0000, it doesn't skip user inputting that

 				while (passcode != input) {
 					LcdSetPosition(1, 0);
 					LcdWriteStr("CODE: ____");
 					LcdSetPosition(1, 6);
 					LcdSetDisplay(true, false, false);
 					input = -1;
 					for (uint8_t i = 0; i < 4; i++) {
 						while((key = KeypadGetKey()) == 0);
 						LcdWriteChar('*');
 						if (i == 0) {
 							input = key - '0';
 						} else {
 							input = (10 * input) + (key - '0');
 						}
  					}
 				}
 				higher = false;		// clear event flag

 			} else if (lower) { // temperature less than lower threshold, handle event
 				LcdWriteStr("TEMP TOO LOW");
 				Play(2000, 1000);
 				newNote = true;
 				int16_t input = -1;	// -1 so if user gives code of 0000, it doesn't skip user inputting that

 				// loop while user doesn't supply the correct code
 				while (passcode != input) {
 					LcdSetPosition(1, 0);
 					LcdWriteStr("CODE: ____");
 					LcdSetPosition(1, 6);
 					LcdSetDisplay(true, false, false);
 					input = -1;
 					for (uint8_t i = 0; i < 4; i++) {
 						while ((key = KeypadGetKey()) == 0);
 						LcdWriteChar('*');
 						if (i == 0) {
 							input = key - '0';
 						} else {
 							input = (10 * input) + (key - '0');
 						}
 					}
 				}
 				lower = false;	// clear event flag

 			} else if (movement) { // accelerometer detected movement change >10 in 500ms, handle event
 				LcdWriteStr("MOVE DETECT");
 				Play(2000, 1000);
 				newNote = true;
 				int16_t input = -1;	// -1 so if user gives code of 0000, it doesn't skip user inputting that

 				while (passcode != input) {
 					LcdSetPosition(1, 0);
 				 	LcdWriteStr("CODE: ____");
 				 	LcdSetPosition(1, 6);
 				 	LcdSetDisplay(true, false, false);
 				 	input = -1;
 				 	for (uint8_t i = 0; i < 4; i++) {
 				 		while ((key = KeypadGetKey()) == 0);
 				 		LcdWriteChar('*');
 				 		if (i == 0) {
 				 			input = key - '0';
 				 		} else {
 				 			input = (10 * input) + (key - '0');
 				 		}
 				 	}
 				}

 				movement = false;	// clear event flag
 				secondRead = false;
 			}

 			// system was reset
 			watchdog = false;
 			newNote = false;
 			LcdClear();
 			LcdWriteStr("SYS RST");

 			// set up timer for next LCD update
 			TIM5->CNT = 0;
 			TIM5->CR1 |= TIM_CR1_CEN;

		} else if (updateLcd) { // LCD update event triggered, handle here
			LcdSetPosition(0, 0);
			LcdWriteStr("Temp:");
			LcdSetDisplay(true, false, false);
			uint16_t f = ConvertF(data);
			if (f >= 10) {
				uint8_t temp = f, divisor = 1;
				while (temp >= 10) {
					temp /= 10;
					divisor *= 10;
				}
				temp = f;
				while (divisor > 0) {
					LcdWriteChar(deci[temp / divisor]);
					temp %= divisor;
					divisor /= 10;
				}
			} else {
				LcdWriteChar(deci[f]);
			}

			// checking accelerometer while also updating LCD in case of change
			if (secondRead) {
				lastX = x;
				lastY = y;
				lastZ = z;
			}
			ADXL345Read(&x, &y, &z);
			if (secondRead) { // if movement change in any direction is greater than 10, generic event flag and movement flag will be set
				if ( (abs(lastX) >= abs(x) + 10 || abs(lastX) <= abs(x) - 10) ||
						(abs(lastY) >= abs(y) + 10 || abs(lastY) <= abs(y) - 10) ||
						(abs(lastZ) >= abs(z) + 10 || abs(lastZ) <= abs(z) - 10) ) {
					watchdog = true;
					movement = true;
				}
			}
			secondRead = true;

			updateLcd = false;	// clear update flag

			// set up timer for next LCD update
			TIM5->CNT = 0;
			TIM5->CR1 |= TIM_CR1_CEN;
		}
	}

	// never return
	for(;;){}

	return 0;
}

void ADC_IRQHandler() {
	// checking if end of conversion set off interrupt
	if (ADC1->SR & ADC_SR_EOC) {
		data = ADC1->DR;

		// checking to see if data read is higher or lower than thresholds, will set off event if true
		if (data > higherAdc || data < lowerAdc) {
			if (data > higherAdc) {
				higher = true;
			} else {
				lower = true;
			}
			watchdog = true;
		}

		// clearing end of conversion flag
		ADC1->SR &= ~(ADC_SR_EOC);
	}
}

// handles time of piezo speaker
void TIM4_IRQHandler() {
	// stop clocks
	TIM4->CR1 = 0;
	TIM3->CR1 &= ~(0b1);

	// clear update flag on tim4
	TIM4->SR &= ~(0b1);

	// if note has ended but event hasn't cleared, start another note on piezo
	if (newNote) {
		Play(2000, 1000);
	}
}

void TIM5_IRQHandler() {
	// stop clock
	TIM5->CR1 = 0;

	// read data from ADC to be used to write to LCD
	data = ADC1->DR;

	// clear update flag on tim5 & create update flag
	TIM5->SR &= ~TIM_SR_UIF;
	updateLcd = true;

	// start next conversion on ADC
	ADC1->CR2 |= ADC_CR2_SWSTART;
}
