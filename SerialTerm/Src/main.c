/**
 * Name: Dr. Livingston
 * Course: CPE2610
 * Assignment: Lab1
 * Dependencies: uart_driver.h
 * Description: C terminal testing program
 */



#include <stdio.h>
#include <stdint.h>
#include "uart_driver.h"

#define F_CPU 16000000UL

// main
int main(void){
	init_usart2(57600,F_CPU);
	printf("Hello World!!\n");

	uint32_t value = 0;

	printf("Enter a value:");
	scanf("%lu", &value);
	printf("Value is %lu\n", value);

	printf("Enter a value:");
	scanf("%lu", &value);
	printf("Value is %lu\n", value);

	// never return
	for(;;){}

	return 0;
}

