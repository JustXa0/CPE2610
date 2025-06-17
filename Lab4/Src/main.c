/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab4
 * Dependencies: ledbar.h, tim.h, delay.h, lcd.h, keypad.h
 * Description: Hex game on LCD screen
 */

#include <stdio.h>
#include <stdlib.h>
#include "tim.h"
#include "lcd.h"
#include "ledbar.h"
#include "keypad.h"
#include "delay.h"

// Macro for the frequency of the processor, 16MHz
#define F_CPU 16000000UL

#define RCC_APB1ENR 0x40023840

static const char keys[17] = {'?', '1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', 'E', '0', 'F', 'D'};
static const char hex[17] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
static const uint8_t vals[17] = {16, 1, 2, 3, 10, 4, 5, 6, 11, 7, 8, 9, 12, 14, 0, 15, 13};

// main
int main(void){
	volatile uint32_t *const rccApb1EnrAddr = (volatile uint32_t *const) RCC_APB1ENR;
	*rccApb1EnrAddr |= (0b1 | (0b1 << 3));
	LedBarInit();
	LcdSetup();
	LcdInit();
	KeypadInit();

	volatile TIMx_t *const tim5 = (volatile TIMx_t *const) TM5;
	tim5->ARR = 0xFFFFFFFF;
	tim5->CR1 |= 0b1;

	LcdSetPosition(0, 4);
	LcdWriteStr("Hex Game");
	LcdSetPosition(1,2);
	LcdWriteStr("Hit any key");

	while(KeypadStatus() == 0);

	LcdClear();
	uint16_t seed = tim5->CNT;
	tim5->CR1 = 0;
	srand(seed);

	tim5->PSC = 15999;
	tim5->ARR = 0xFFFF;
	tim5->CNT = 0;
	tim5->EGR = 1;

	uint8_t correct = 0;
	uint32_t times[5] = {0};

	for (uint8_t i = 1; i < 6; i++) {
		LcdSetPosition(0, 4);
		LcdWriteStr("Round ");
		LcdWriteChar(hex[i]);
		LcdSetPosition(1, 4);
		SystemSec(2);
		LcdWriteStr("1 ");
		SystemMilli(500);
		LcdWriteStr("2 ");
		SystemMilli(500);
		LcdWriteStr("3 ");
		SystemMilli(500);

		LcdClear();
		LcdSetPosition(0, 6);
		LcdWriteStr("Go!");

		uint8_t pattern1 = rand() & 0xF;
		uint8_t pattern2 = rand() & 0xF;

		LedBarSetPattern((pattern1 << 12) + (pattern2 << 5));
		LcdSetPosition(1, 5);
		LcdWriteStr("0x");

		tim5->CR1 |= 1;

		uint8_t val1 = KeypadGetKey();
		LcdWriteChar(keys[val1]);
		uint8_t val2 = KeypadGetKey();
		LcdWriteChar(keys[val2]);

		// after value is guessed
		tim5->CR1 = 0;

		if ((vals[val1] == pattern1) && (vals[val2] == pattern2)) {
			LcdClear();
			LcdSetPosition(0, 3);
			LcdWriteStr("Correct!");
			times[i - 1] = tim5->CNT;
			correct++;
			SystemSec(3);
			LcdClear();
		} else {
			LcdClear();
			LcdSetPosition(0, 2);
			LcdWriteStr("Wrong!");
			LcdSetPosition(1, 2);
			LcdWriteStr("Ans:0x");
			LcdWriteChar(hex[pattern1]);
			LcdWriteChar(hex[pattern2]);
			SystemSec(3);
			LcdClear();
		}

		tim5->CR1 = 0;
		tim5->CNT = 0;
	}

	LcdClear();
	if (correct == 0) {
		LcdWriteStr("None Correct!");
		LcdSetPosition(1, 0);
		LcdWriteStr("Rst to try again!");
	} else {
		LcdSetPosition(0, 2);
		LcdWriteChar(hex[correct]);
		LcdWriteStr("/5 Correct!");
		LcdSetPosition(1, 0);
		LcdWriteStr("AVG time:");

		uint32_t tot = 0;
		uint32_t lowest = 1000000000;
		for (uint8_t i = 0; i < 5; i++) {
			tot += times[i];
			if ((lowest > times[i]) && (times[i] != 0)) {
				lowest = times[i];
			}
		}

		uint32_t avg = tot / correct;

		while (avg > 0) {
		    uint32_t divisor = 1;
		    uint32_t temp = avg;

		    // Find the largest power of 10
		    while (temp >= 10) {
		        temp /= 10;
		        divisor *= 10;
		    }

		    // Extract the most significant digit
		    uint8_t digit = avg / divisor;

		    // Print the corresponding character
		    LcdWriteChar(hex[digit]);

		    // Remove the most significant digit from avg
		    avg %= divisor;
		}


		SystemSec(3);

		LcdClear();
		LcdSetPosition(0, 2);
		LcdWriteStr("Fastest Time");
		LcdSetPosition(1, 4);

		while (lowest > 0) {
			uint32_t divisor = 1;
			uint32_t temp = lowest;

			// Find the largest power of 10
			while (temp >= 10) {
				temp /= 10;
				divisor *= 10;
			}

			// Extract the most significant digit
			uint8_t digit = lowest / divisor;

			// Print the corresponding character
			LcdWriteChar(hex[digit]);

			// Remove the most significant digit from avg
			lowest %= divisor;
		}

		SystemSec(3);

		LcdClear();
		LcdWriteStr("Rst 2 Play Again");
	}

	// never return
	for(;;){}

	return 0;
}


