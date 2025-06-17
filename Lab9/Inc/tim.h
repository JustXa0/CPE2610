/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab4
 * Dependencies: stdint.h
 * Description: Struct and addresses to interact with timers
 */

#ifndef TIM_H_
#define TIM_H_

#include <stdint.h>

#define TM1 0x40010000
#define TM2 0x40000000
#define TM3 0x40000400
#define TM4 0x40000800
#define TM5 0x40000C00

#define RCC_APB1ENR 0x40023840

#define RCC_APB1ENR_TIM2 0
#define RCC_APB1ENR_TIM3 1
#define RCC_APB1ENR_TIM4 2
#define RCC_APB1ENR_TIM5 3

#define TIM_1MS_PSC 15999
#define TIM_1US_PSC	15

typedef volatile struct {
	uint16_t CR1;
	const uint16_t rsv0;
	uint16_t CR2;
	const uint16_t rsv1;
	uint16_t SMCR;
	const uint16_t rsv2;
	uint16_t DIER;
	const uint16_t rsv3;
	uint16_t SR;
	const uint16_t rsv4;
	uint16_t EGR;
	const uint16_t rsv5;
	uint16_t CCMR1;
	const uint16_t rsv6;
	uint16_t CCMR2;
	const uint16_t rsv7;
	uint16_t CCER;
	const uint16_t rsv8;
	uint32_t CNT;
	uint16_t PSC;
	const uint16_t rsv9;
	uint32_t ARR;
	const uint32_t rsv10;
	uint32_t CCR1;
	uint32_t CCR2;
	uint32_t CCR3;
	uint32_t CCR4;
	const uint32_t rsv11;
	uint16_t DCR;
	const uint16_t rsv12;
	uint16_t DMAR;
	const uint16_t rsv13;
	uint16_t OR;
	const uint16_t rsv14;
} TIMx_t;

#endif /* TIM_H_ */
