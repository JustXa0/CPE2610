/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab5
 * Dependencies: gpio.h, tim.h, piezo.h
 * Description: Functions to read from keypad
 */

#include "gpio.h"
#include "tim.h"
#include "piezo.h"

/*
 * FUTURE NOTES:
 * 		tim3 is used for PWM
 * 		tim4 is used for time argument in Play() function
 * 		piezo is on PB4, already set up to output, no need for pupdr
 */

// struct for interacting with GPIOC
static volatile gpio_s *const gpiob = (volatile gpio_s *const) GPIOB_BASE;

// structs for interacting with timers
static volatile TIMx_t *const tim3 = (volatile TIMx_t *const) TM3;
static volatile TIMx_t *const tim4 = (volatile TIMx_t *const) TM4;

/**
 * PiezoInit - initalizes all registers & timers used for playing piezos
 * Args - N/A
 * Return - N/A
 */
void PiezoInit() {
	// enable GPIOB
	volatile uint32_t *const rccAbh1enrAddr = (volatile uint32_t *const) RCC_AHB1ENR;
	*rccAbh1enrAddr |= (1 << RCC_AHB1ENR_GPIOB);
	gpiob->moder = (~(0b11 << 8) & gpiob->moder) | (ALT <<  8);
	gpiob->afrl = (~(0xF << 16) & gpiob->afrl) | (0x2 << 16);

	// enable TIM3 & TIM4
	volatile uint16_t *const rccApb1enrAddr = (volatile uint16_t *const) RCC_APB1ENR;
	*rccApb1enrAddr |= (1 << RCC_APB1ENR_TIM3 | 1 << RCC_APB1ENR_TIM4);

	// setup TIM4 for general delay timing
	tim4->PSC = TIM_1MS_PSC;
	tim4->EGR = 1;
	tim4->SR &= ~(0b1);
	tim4->DIER |= 0b1;

	volatile uint32_t *const nvic_iser0 = (volatile uint32_t *const) 0xE000E100;
	*nvic_iser0 |= (0b1 << 30);
}

/**
 * Play - plays a note on the piezo
 * Args - freq -> frequency in hertz to be played
 * 		  time -> length of frequency to be played
 * Return - N/A
 */
void Play(uint16_t freq, uint16_t time) {
    uint32_t period = 1000000 / freq; // Period in microseconds
    period /= 2;
    tim3->PSC = TIM_1US_PSC;
    tim3->ARR = period - 1;
    tim3->CCR1 = period - 1;

    // Configure TIM3 in Output Compare Toggle Mode
    // clear OC1M bits
    // set OC1M to toggle mode (011)
    // enable Output Compare Preload (OC1PE = 1)
    tim3->CCMR1 = (tim3->CCMR1 & ~(0b111 << 4)) | (0b011 << 4) | (1 << 3);

    // Enable TIM3 CH1 Output
    // ensure active high output
    // enable output on CH1
    tim3->CCER = (tim3->CCER & ~(1 << 1)) | (1 << 0);

    tim3->EGR |= 1;  // Force update
    tim3->SR &= ~(0b1); // Clear update flag

    // Configure TIM4 for delay timing
    tim4->ARR = time;
    tim4->CNT = 0;
    tim3->CNT = 0;

    // Start both timers
    tim3->CR1 |= (1 << 0);  // Enable TIM3
    tim4->CR1 |= (1 << 0);  // Enable TIM4


}
