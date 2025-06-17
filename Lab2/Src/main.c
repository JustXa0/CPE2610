/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab2
 * Dependencies: ledbar.h, pushbutton.h, delay.h, uart_driver.h
 * Description: User LED Flashing Program
 */



#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "ledbar.h"
#include "pushbutton.h"
#include "delay.h"
#include "uart_driver.h"

// Macro for the frequency of the processor, 16MHz
#define F_CPU 16000000UL

/**
 * generateRandoms - Creates a random number for use with LED bar
 * Args - N/A
 * Return - 10 bit pattern stored in uint16_t
 */
static uint16_t generateRandoms() {
	uint16_t random = rand() % (LEDBAR_ALL - 0x0 + 1) + (0x0);
	random &= ~(0x081F);

	return random;
}

/**
 * printBinary - prints the binary value stored in the uint16_t to the console
 * Args - num - number to be printed in binary
 * Return - N\A
 */
static void printBinary(uint16_t num) {
	for (uint8_t i = 15; i > 4; i--) {
		if (i != 11) {
			printf("%d", ~(0xFFFE) & (num >> i));
		}
	}
}

// main
int main(void){
	//Initializes serial communication, LED bar, & pushbuttons
	init_usart2(57600,F_CPU);
	LedBarInit();
	PushButtonEncInit();
	PushButtonUserInit();
	PushButtonControlInit();

	while(1) {
		printf("Welcome to Fast Match! \n");
		printf("Please enter a random seed to start the game: ");

		// variable to store input speed
		uint32_t input;
		scanf("%lu", &input);

		// seed rand()
		srand(input);

		uint16_t random = generateRandoms();
		printf("Binary version corresponding to led position: ");
		printBinary(random);
		printf("\n");

		// stores solution to problem
		uint16_t solution = random;

		// stores current LED position values
		uint16_t current = 0;

		// stores current number of steps taken
		uint32_t steps = 0;

		while (solution != current) {
			if (PushButtonUserRead()) {
				current = LedBarToggle(0);
				steps++;
			} else if (PushButtonControlRead()) {
				current = LedBarShiftLeft(1, 1);
				steps++;
			} else if (PushButtonEncRead()) {
				current = LedBarShiftRight(1, 1);
				steps++;
			}
		}

		printf("You win! \n");
		printf("It took you %lu steps! \n", steps);
		BusyMilli(100);
		LedBarWin();
	}

	// never return
	for(;;){}

	return 0;
}

