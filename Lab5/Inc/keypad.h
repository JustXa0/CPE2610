/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab5
 * Dependencies: stdint.h
 * Description: Functions to read from keypad
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <stdint.h>

#define RCC_BASE 	0x40023800
#define RCC_AHB1ENR 0x40023830
#define RCC_APB2ENR 0x40023844

/**
 * KeypadInit - initializes keypad interface
 * Args - N/A
 * Return - N/A
 */
extern void KeypadInit();

/**
 * KeypadStatus - non-blocking function to read and return position of key pressed
 * Args - N/A
 * Return - 1-16 -> based on position of key pressed, 0 if no key was pressed during the time of check
 */
extern uint8_t KeypadStatus();

/**
 * KeypadGetKey - blocking function to read and return position of key pressed
 * 				  handles de-bounce
 * Args - N/A
 * Return - 1-16 -> based on position of key pressed
 */
extern uint8_t KeypadGetKey();


#endif /* KEYPAD_H_ */
