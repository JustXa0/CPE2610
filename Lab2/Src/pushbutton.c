/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab2
 * Dependencies: pushbutton.h, delay.h
 * Description: Implementation of pushbutton functions
 */

#include "pushbutton.h"
#include "delay.h"

void PushButtonEncInit() {
	volatile uint32_t *const rccAbh1enrAddr = (volatile uint32_t *const) RCC_AHB1ENR;
	*rccAbh1enrAddr |= (1<<RCC_AHB1ENR_GPIOA);

	// configure port so pin is set to input *00*
	volatile uint32_t *const gpioaModerAddr = (volatile uint32_t *const) GPIOA_MODER;

	// sets pin 12 to pull down-resistor configuration
	volatile uint32_t *const gpioaPupdrAddr = (volatile uint32_t *const) GPIOA_PUPDR;
	*gpioaPupdrAddr = (0x64000000);
	*gpioaPupdrAddr |= (0b1 << 24);
}

void PushButtonUserInit() {
	volatile uint32_t *const rccAbh1enrAddr = (volatile uint32_t *const) RCC_AHB1ENR;
	*rccAbh1enrAddr |= (1<<RCC_AHB1ENR_GPIOC);

	// configure port so pin is set to input *00*
	volatile uint32_t *const gpiocModerAddr = (volatile uint32_t *const) GPIOC_MODER;
	*gpiocModerAddr &= ~(0xFFFFFFFF);
}

void PushButtonControlInit() {
	volatile uint32_t *const rccAbh1enrAddr = (volatile uint32_t *const) RCC_AHB1ENR;
	*rccAbh1enrAddr |= (1<<RCC_AHB1ENR_GPIOC);

	// configure port so pin is set to input *00*
	volatile uint32_t *const gpiocModerAddr = (volatile uint32_t *const) GPIOC_MODER;
	*gpiocModerAddr &= ~(0xFFFFFFFF);

	// sets pin 11 to pull down-resistor configuration
	volatile uint32_t *const gpiocPupdrAddr = (volatile uint32_t *const) GPIOC_PUPDR;
	*gpiocPupdrAddr &= ~(0xFFFFFFFF);
	*gpiocPupdrAddr |= (0b1 << 22);
}

bool PushButtonEncRead() {
	// load register value and clear all pins other than pin 12
	volatile const uint32_t *const gpioaIdrAddr = (volatile const uint32_t *const) GPIOA_IDR;
	uint32_t value = ~(0xE) & (*gpioaIdrAddr >> PB_ENC);

	// switches are active low, checking for 0
	if (value == 1) {
		return false;
	}

	// handle quick debounce
	BusyMilli(5);

	// checking for when switch is released before enabling final debounce delay
	while (value != 1) {
		value = ~(0xE) & (*gpioaIdrAddr >> PB_ENC);
	}

	BusyMilli(15);
	return true;
}

bool PushButtonUserRead() {
	// load register value and clear all pins other than pin 12
	volatile const uint32_t *const gpiocIdrAddr = (volatile const uint32_t *const) GPIOC_IDR;
	uint32_t value = (*gpiocIdrAddr >> PB_USER) & ~(0xE);

	// switches are active low, checking for 0
	if (value == 1) {
		return false;
	}

	// checking for when switch is released, no debounce delay needed
	while (value != 1) {
		value = (*gpiocIdrAddr >> PB_USER) & ~(0xE);
	}

	return true;
}

bool PushButtonControlRead() {
	// load register value and clear all pins other than pin 12
	volatile const uint32_t *const gpiocIdrAddr = (volatile const uint32_t *const) GPIOC_IDR;
	uint32_t value = (*gpiocIdrAddr >> PB_CONTROL) & ~(0x1E);

	// switches are active low, checking for 0
	if (value == 1) {
		return false;
	}

	// handle quick debounce
	BusyMilli(5);

	// checking for when switch is releaesd before enabling final debounce delay
	while (value != 1) {
		value = (*gpiocIdrAddr >> PB_CONTROL) & ~(0x1E);
	}

	BusyMilli(15);
	return true;
}
