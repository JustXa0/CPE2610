/*
 * syscfg.h
 *
 *  Created on: Apr 4, 2025
 *      Author: prendivillej
 */

#ifndef SYSCFG_H_
#define SYSCFG_H_

#include <stdbool.h>
#include <stdint.h>

#define SYSCFG 0x40013800

typedef struct {
	uint8_t MEM_MODE;
	const uint16_t rsv0;
	const uint8_t rsv1;

	const uint16_t rsv2;
	uint8_t ADC1DC2;
	const uint8_t rsv3;

	uint16_t EXTICR1;
	const uint16_t rsv4;

	uint16_t EXTICR2;
	const uint16_t rsv5;

	uint16_t EXTICR3;
	const uint16_t rsv6;

	uint16_t EXTICR4;
	const uint16_t rsv7;

	uint8_t CMPCR;
	const uint16_t rsv8;
	const uint8_t rsv9;

} syscfg_t;


#endif /* SYSCFG_H_ */
