/*
 * piezo.h
 *
 *  Created on: Mar 28, 2025
 *      Author: prendivillej
 */

#ifndef PIEZO_H_
#define PIEZO_H_

#include <stdint.h>

#define RCC_AHB1ENR 0x40023830
#define RCC_AHB1ENR_GPIOB 1

#define GPIOB_BASE 0x40020400

#define ALT 2

extern void PiezoInit();

extern void Play(uint16_t freq, uint16_t time);

#endif /* PIEZO_H_ */
