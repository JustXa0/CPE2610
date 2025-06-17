/**
 * Name: Dr. Livingston
 * Course: CPE2610
 * Assignment: Lab1
 * Dependencies: flash.h
 * Description: Initializes and controls the User Led on the STM32F411
 */

#include "flash.h"
#include <stdint.h>

void UserLedInit(){
	volatile uint32_t* rcc = (uint32_t*) RCC_AHB1ENR;
	*rcc |= (1<<RCC_GPIOA_EN);

	volatile uint32_t* moder = (uint32_t*) GPIOA_MODER;

	*moder &= ~(0b11<<(2*GPIOA_USER));
	*moder |= 0b01<<(2*GPIOA_USER);
}

void UserLedOn(){
	volatile uint32_t* odr = (uint32_t*) GPIOA_ODR;
	*odr |= 1<<GPIOA_USER;
}

void UserLedOff(){
	volatile uint32_t* odr = (uint32_t*) GPIOA_ODR;
	*odr &=  ~(1<<GPIOA_USER);
}
