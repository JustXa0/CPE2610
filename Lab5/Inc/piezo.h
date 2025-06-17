/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab5
 * Dependencies: stdint.h
 * Description: Functions to read from keypad
 */

#ifndef PIEZO_H_
#define PIEZO_H_

#include <stdint.h>

#define RCC_AHB1ENR 0x40023830
#define RCC_AHB1ENR_GPIOB 1

#define GPIOB_BASE 0x40020400

#define ALT 2

/**
 * PiezoInit - initalizes all registers & timers used for playing piezos
 * Args - N/A
 * Return - N/A
 */
extern void PiezoInit();

/**
 * Play - plays a note on the piezo
 * Args - freq -> frequency in hertz to be played
 * 		  time -> length of frequency to be played
 * Return - N/A
 */
extern void Play(uint16_t freq, uint16_t time);

#endif /* PIEZO_H_ */
