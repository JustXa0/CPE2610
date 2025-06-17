/*
 * exti.h
 *
 *  Created on: Apr 6, 2025
 *      Author: prendivillej
 */

#ifndef EXTI_H_
#define EXTI_H_

#define EXTI 0x40013C00

typedef struct {
	uint32_t IMR;
	uint32_t EMR;
	uint32_t RTSR;
	uint32_t FTSR;
	uint32_t SWIER;
	uint32_t PR;
} exti_t;

#endif /* EXTI_H_ */
