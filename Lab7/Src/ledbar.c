/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab2
 * Dependencies:ledbar.h, delay.h
 * Description: Information for led array functions
 */

#include "ledbar.h"
#include "delay.h"

uint16_t LedBarInit() {
	// enable clock
	volatile uint32_t *const rccAbh1enrAddr = (volatile uint32_t *const) RCC_AHB1ENR;
	*rccAbh1enrAddr |= (1<<RCC_AHB1ENR_GPIOB);

	// configure port so pins are set to output *01*
	volatile uint32_t *const gpiobModerAddr = (volatile uint32_t *const ) GPIOB_MODER;
	*gpiobModerAddr &= ~0xFF3FFC00;
	*gpiobModerAddr |= 0x55155400;

	// set all lights off
	volatile uint16_t *const gpiobOdrAddr = (volatile uint16_t *const) GPIOB_ODR;
	*gpiobOdrAddr &= (volatile uint16_t)~LEDBAR_ALL;

	// all lights off
	return 0;
}

uint16_t LedBarOn(uint8_t offset) {
	// load output register value
	volatile uint16_t *const gpiobOdrAddr = (volatile uint16_t *const) GPIOB_ODR;

	// if offset is greater than 9, nothing is done
	if (offset > 9) {
		return *gpiobOdrAddr;
	}

	// handles offset to pin selection
	uint16_t value = (offset < 6) ? (offset + 5) : (offset + 6);

	// assigns a 1 to the correct pin output using the offset calculated above
	*gpiobOdrAddr |= (1 << value);

	return *gpiobOdrAddr;
}

uint16_t LedBarOff(uint8_t offset) {
	// load output register value
	volatile uint16_t *const gpiobOdrAddr = (volatile uint16_t *const) GPIOB_ODR;

	// if offset is greater than 9, nothing is done
	if (offset > 9) {
		return *gpiobOdrAddr;
	}

	// handles offset to pin selection
	uint16_t value = (offset < 6) ? (offset + 5) : (offset + 6);

	// assigns a 1 to the correct pin output using the offset calculated above
	*gpiobOdrAddr &= ~(1 << value);

	return *gpiobOdrAddr;
}

uint16_t LedBarToggle(uint8_t offset) {
	// load output register value
	volatile uint16_t *const gpiobOdrAddr = (volatile uint16_t *const) GPIOB_ODR;

	// if offset is greater than 9, nothing is done
	if (offset > 9) {
		return *gpiobOdrAddr;
	}

	// handles offset to pin selection
	uint16_t value = (offset < 6) ? (offset + 5) : (offset + 6);

	// toggles bit assigned by calculation above
	*gpiobOdrAddr ^= (1 << value);

	return *gpiobOdrAddr;
}

uint16_t LedBarSetPattern(uint16_t pattern) {
	// load output register value
	volatile uint16_t *const gpiobOdrAddr = (volatile uint16_t *const) GPIOB_ODR;

	// clears all bits within output register
	*gpiobOdrAddr &= (uint16_t)~(LEDBAR_ALL);

	// set new pattern to output register value
	*gpiobOdrAddr |= pattern;
	return *gpiobOdrAddr;
}

uint16_t LedBarShiftRight(uint8_t bits, bool roll) {
	// load output register value
    volatile uint16_t *const gpiobOdrAddr = (volatile uint16_t *const)GPIOB_ODR;
    uint16_t pattern = *gpiobOdrAddr;

    // Extract only the LED bits from the mapped positions
    uint16_t ledBits = ((pattern & 0xF800) >> 6) | ((pattern & 0x07E0) >> 5);

    if (roll) {
        ledBits = ((ledBits >> bits) | (ledBits << (10 - bits))) & 0x03FF;
    } else {
        ledBits = (ledBits >> bits) & 0x03FF;
    }

    // Clear out old LED bits in pattern
    pattern &= ~(0xF800 | 0x07E0); // Clear LED bits at positions 15-12 and 10-5

    // Map the shifted bits back to the correct positions
    pattern |= ((ledBits & 0x03C0) << 6) | ((ledBits & 0x003F) << 5);

    *gpiobOdrAddr = pattern;
    return *gpiobOdrAddr;
}



uint16_t LedBarShiftLeft(uint8_t bits, bool roll) {
	// load output register value
    volatile uint16_t *const gpiobOdrAddr = (volatile uint16_t *const) GPIOB_ODR;
    uint16_t pattern = *gpiobOdrAddr;

    // Extract only the LED bits from the mapped positions
    uint16_t ledBits = ((pattern & 0xF800) >> 6) | ((pattern & 0x07E0) >> 5);

    if (roll) {
    	ledBits = ((ledBits << bits) | (ledBits >> (10 - bits))) & 0x03FF;
    } else {
    	ledBits = (ledBits << bits) & 0x03FF;
    }

    // Clear out old LED bits in pattern
    pattern &= ~(0xF800 | 0x07E0);

    // Map the shifts bits back to the correct positions
    pattern |= ((ledBits & 0x03C0) << 6) | ((ledBits & 0x003F) << 5);

    *gpiobOdrAddr = pattern;
    return *gpiobOdrAddr;
}

uint16_t LedBarGetPattern() {
	// Load output register value
	volatile uint16_t *const gpiobOdrAddr = (volatile uint16_t *const) GPIOB_ODR;
	return *gpiobOdrAddr;
}

void LedBarWin() {
	// turn LEDs on sequentially, with a 125ms delay in-between
	for (uint8_t i = 0; i < 10; i++) {
		LedBarOn(i);
		BusyMilli(125);
	}

	// turn LEDs off sequentially, with a 125ms delay in-between
	for (uint8_t i = 10; i > 0; i--) {
		LedBarOff(i-1);
		BusyMilli(125);
	}
}
