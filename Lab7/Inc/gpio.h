/**
 * Name:		Jamie Prendiville
 * Course: 		CPE 2610
 * Section: 	121
 * Assignment: 	Lab 3 - LED Maze
 * File: gpio.h
 * Dependencies:
 * 		none
 * Usage:		Structure for interacting with GPIO ports
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

#define GPIO_A 0x40020000 // PortA
#define GPIO_B 0x40020400 // PortB
#define GPIO_C 0x40020800 // PortC

// generic gpio register structure
typedef struct {
	uint32_t moder; // Offset 0x00
	// input mode = 00, output mode = 01
	uint32_t otyper; // Offset 0x04
	const uint16_t rsv0; //Reserved space on speeder
	uint16_t ospeedr;
	uint32_t pupdr;
	const uint16_t idr;
	const uint16_t rsv1;
	uint16_t odr;
	const uint16_t rsv2;
	uint32_t bsrr; // Sets and clears bits in a single memory access
	uint32_t lckr;
	uint32_t afrl;
	uint32_t afrh;

}gpio_s;



#endif /* GPIO_H_ */
