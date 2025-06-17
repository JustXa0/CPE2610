/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab6
 * Dependencies: uart_driver.h, stm32f411.h, stdint.h, stdio.h
 * Description: Server program
 */

// Macro for the frequency of the processor, 16MHz
#define F_CPU 16000000UL
#define USART1_NVIC 5

#include "uart_driver.h"
#include "stm32f411.h"
#include <stdio.h>
#include <stdint.h>

static volatile uint8_t correct = 0;

// main
int main(void){
	// enable usart2 for console
	init_usart2(57600, F_CPU);
	// turn on gpiob
	RCC->AHB1ENR |= 1 << RCC_AHB1ENR_GPIOBEN_Pos;

	// set gpioc 7..6 to alternate
	GPIOB->MODER |= (0b10 << 14) | (0b10 << 12);

	// set alternate mapping to af7
	GPIOB->AFR[0] |= (0b111 << 28) | (0b111 << 24);

	// turn on usart2
	RCC->APB2ENR |= 1 << RCC_APB2ENR_USART1EN_Pos;

	// setup usart1
	USART1->CR1 |= (1 << 13) | (1 << 5) | (1 << 3) | (1 << 2);
	USART1->BRR = (0x68 << 4) | (0b1 << 1);
	USART1->DR = 0;


	//NVIC Enable for USART
	NVIC->ISER[1] = 1 << (USART1_NVIC);

	printf("Please enter a number (0-255) for person to guess:");
	scanf("%d", &correct);

	// never return
	for(;;){}

	return 0;
}

void USART1_IRQHandler() {
	// print client guess
	printf("Guess: %d \n", USART1->DR);
	char send = 0;
	// logic to determine what character to send
	if (USART1->DR == correct) {
		send = '=';
	} else if (USART1->DR > correct) {
		send = '<';
	} else if (USART1->DR < correct) {
		send = '>';
	}

	// send character to other board
	USART1->DR = send;

	// busy wait to ensure character is sent
	while ((USART1->SR & ~(0xFFFFFFBF)) >> 6 != 1);
}
