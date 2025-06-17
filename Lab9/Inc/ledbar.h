/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab2
 * Dependencies: none
 * Description: Information for led array functions
 */

#ifndef LEDBAR_H_
#define LEDBAR_H_

#include <stdint.h>
#include <stdbool.h>

//What Pins? (Schematic)

//PB5-PB10, PB12-15

//What Ports? (Schematic)

// PortB

//What Constants? (Ref Manual) ???

//Which bus?

//RCC Base address (RM38)

// 0x40023800

//But where do we turn on PortB?

// 0x400204000

//RCC_AHB1ENR - offset 30  (RM136)
#define RCC_AHB1ENR 0x40023830

//What bit?

#define RCC_AHB1ENR_GPIOB 1

//Base address PortB Control (RM38)

// 0x40020400

//Mode Register (offset 0x0) (RM157)
#define GPIOB_MODER 0x40020400

//Constants for modes
#define MODER_IN 	0b00
#define MODER_OUT 	0b01
#define MODER_ALT 	0b10
#define MODER_ANA 	0b11
#define MODER_MASK 	0b11
#define MODER_WIDTH 2


//ODR Register (offset 0x14) (RM159)
#define GPIOB_ODR	0x40020414
//BSRR Register (offset 0x18) (RM160)
#define GPOIB_BSRR	0x40020418

#define LEDBAR_ALL	0xF7E0

//What functions?

/**
* LedBarInit - Sets up port for outputs
* Args - N/A
* Return - 10 bit current pattern stored in uint16_t
*/
extern uint16_t LedBarInit();

/**
* LedOn - Turns on Led at the offset
* Args - offset - 0-9 led position (out of range has no effect)
* Return - 10 bit current pattern stored in uint16_t
*/
extern uint16_t LedBarOn(uint8_t offset);

/**
* LedOff - Turns off Led at the offset
* Args - offset - 0-9 led position (out of range has no effect)
* Return - 10 bit current pattern stored in uint16_t
*/
extern uint16_t LedBarOff(uint8_t offset);

/**
* LedToggle - Toggles Led at the offset
* Args - offset - 0-9 led position (out of range has no effect)
* Return - 10 bit current pattern stored in uint16_t
*/
extern uint16_t LedBarToggle(uint8_t offset);

/**
* LedSetPattern - Sets 10 LSBits to the leds
* Args - offset - 0-9 led position (out of range has no effect)
* Return - 10 bit current pattern stored in uint16_t
*/
extern uint16_t LedBarSetPattern(uint16_t pattern);

/**
* LedShiftRight - Shifts the leds right by "bits" positions, can roll
* Args - bits - number bits to shift
        roll - true: LED9(New) = LED0(OLD)
               false: LED9(New) = 0
* Return - 10 bit current pattern stored in uint16_t
*/
extern uint16_t LedBarShiftRight(uint8_t bits, bool roll);

/**
* LedShiftLeft - Shifts the leds left by "bits" positions, can roll
* Args - bits - number bits to shift
        roll - true: LED0(New) = LED9(OLD)
               false: LED0(New) = 0
* Return - 10 bit current pattern stored in uint16_t
*/
extern uint16_t LedBarShiftLeft(uint8_t bits, bool roll);

/**
* LedGetPattern - return the current light pattern in 10 LSBits.
* Args - n/a
* Return - 10 bit current pattern stored in uint16_t
*/
extern uint16_t LedBarGetPattern();

/**
 * LedBarWin - displays winning animation to LED bar
 * Args - N/A
 * Return - N/A
 */
extern void LedBarWin();




#endif /* LEDBAR_H_ */
