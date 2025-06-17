/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab1
 * Dependencies: uart_driver.h, delay.h, flash.h
 * Description: User LED Flashing Program
 */



#include <stdio.h>
#include <stdint.h>
#include "uart_driver.h"
#include "flash.h"
#include "delay.h"

// Macro for the frequency of the processor, 16MHz
#define F_CPU 16000000UL

// main
int main(void){
	// initializes serial communication
	init_usart2(57600,F_CPU);

	// initializes user led
	UserLedInit();

	/* variables to store the number of flashes requested,
	** as well as how long of a delay between flashes, in ms
	*/
	uint32_t numFlash = 0;
	uint32_t delayMS = 0;

	// infinite loop
	while (1) {
		// prompts user for number of flashes via serial communication, stores in numFlash
		printf("How many times should the LED flash:");
		scanf("%lu", &numFlash);

		// prompts user for delay between flashes via serial communication, stores in delayMS
		printf("How long of a delay should there be in milliseconds:");
		scanf("%lu", &delayMS);

		// loop for flashes, starts with led on for given time, before turning led off for given time
		for (uint32_t i = 0; i < numFlash; i++) {
			UserLedOn();
			BusyMilli(delayMS);
			UserLedOff();
			BusyMilli(delayMS);
		}
	}

	// never return
	for(;;){}

	return 0;
}

