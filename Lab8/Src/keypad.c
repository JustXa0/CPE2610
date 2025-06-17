/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab4
 * Dependencies: tim.h, gpio.h, delay.h, keypad.h
 * Description: Functions to read from keypad
 */

#include <stdbool.h>

#include "tim.h"
#include "gpio.h"
#include "delay.h"
#include "syscfg.h"
#include "exti.h"
#include "stm32f411.h"
#include "keypad.h"

static volatile gpio_s *const gpioc = (volatile gpio_s *const) GPIOC_BASE;

static volatile char keyPressed;
static volatile bool keyReady = false;
static uint8_t colNum = 0;

const char characters[4][4] = {
		{'1','2','3','A'},
		{'4','5','6','B'},
		{'7','8','9','C'},
		{'E','0','F','D'}
};

/**
 * Row[4..1] = PC[7..4]
 * Col[4..1] = PC[3..0]
 */

/**
 * KeypadInit - initializes keypad interface
 * Args - N/A
 * Return - N/A
 */
void KeypadInit() {
	// ************** GPIO *************
	RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOCEN_Pos);

	// Make all gpioc pins 0 - 7 pull-up resistors
	GPIOC->PUPDR &= ~0xFFFF;
	GPIOC->PUPDR |= 0x5555;	// 0x5555

    // Activate COLs
    GPIOC->ODR |= 0xF;

    // ************** SysConfig *************
	// Turn on sysconfig
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Clear and enable portC interrupt for each pin ( 3 - 0 )

	SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI3_PC | SYSCFG_EXTICR1_EXTI2_PC
			| SYSCFG_EXTICR1_EXTI1_PC | SYSCFG_EXTICR1_EXTI0_PC;

	// Rising edge
	EXTI->RTSR |= EXTI_RTSR_TR3 | EXTI_RTSR_TR2 | EXTI_RTSR_TR1
			| EXTI_RTSR_TR0;

	// Falling edge
	EXTI->FTSR |= EXTI_FTSR_TR3 | EXTI_FTSR_TR2 | EXTI_FTSR_TR1
			| EXTI_FTSR_TR0;

	// Enable IM Mask - Interrupt mask, will let it go through
	EXTI->IMR |= EXTI_IMR_IM3 | EXTI_IMR_IM2 | EXTI_IMR_IM1 | EXTI_IMR_IM0;

	// Clear flag
	EXTI->PR = (EXTI_PR_PR3 | EXTI_PR_PR2 |
			EXTI_PR_PR1 | EXTI_PR_PR0);

	// 1ST index because 0 index only goes up to 31
	NVIC->ISER[0] = (1 << (EXTI3_IRQn)) | (1 << (EXTI2_IRQn))
			| (1 << (EXTI1_IRQn)) | (1 << (EXTI0_IRQn));

    // Reset rows as output and columns as input
    GPIOC->MODER &= ~(0xFFFF);	// 0xFFFF
    GPIOC->MODER |= 0x5500; // 0x5500


	// *************** TIM2 **************

	// Enable clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// Enable timer on nvic
	NVIC->ISER[0] = 1 << TIM2_IRQn;
}

/**
 * KeypadStatus - non-blocking function to read and return position of key pressed
 * Args - N/A
 * Return - 1-16 -> based on position of key pressed, 0 if no key was pressed during the time of check
 */
uint8_t KeypadStatus(){
	// Re enables the interrupt
	uint8_t row = 0xFF;

	// Temporarily configure ROWs as outputs
	GPIOC->MODER &= ~0x0000FF00;
	GPIOC->MODER |= 0x5500;  // ROWs 4-7 as outputs

	// Scan each row
	for (uint8_t i = 0; i < 4; i++) {
		// Drive one row low, others high
		GPIOC->ODR = (GPIOC->ODR & ~0xF0) | (0x0F << 4);
		GPIOC->ODR &= ~(1 << (i + 4));
		SystemMicro(10);  // Stabilize

		if (!(GPIOC->IDR & (1 << colNum))) {
			row = i;
			break;
		}
	}

	if (row < 4 && colNum < 4) {
		keyPressed = characters[row][colNum];
		keyReady = true;
	}

	// Reset rows as output and columns as input
	GPIOC->MODER &= ~(0xFFFF);	//0xFFFF
	GPIOC->MODER |= 0x5500;	// 0X5500

    // Activate COLs
    GPIOC->ODR &= ~0xF0;

	return keyReady ? keyPressed : 0;
}

/**
 * KeypadGetKey - blocking function to read and return position of key pressed
 * 				  handles de-bounce
 * Args - N/A
 * Return - 1-16 -> based on position of key pressed
 */
uint8_t KeypadGetKey() {
	if(keyReady) {
		uint8_t key = keyPressed;
		keyReady = false;
		keyPressed = '\0';
		return key;
	}
	return 0;
}

void EXTI0_IRQHandler(void) {
    if ((EXTI->PR & EXTI_PR_PR0) == EXTI_PR_PR0) {
    	// Turn off EXTI Interrupt
    	EXTI->IMR &= ~(EXTI_IMR_IM0 | EXTI_IMR_IM1 | EXTI_IMR_IM2 | EXTI_IMR_IM3);

    	// Settings for tim2
    	TIM2->ARR = 80000 - 1; // 5ms
    	TIM2->CNT = 0;
    	TIM2->SR = 0;
    	TIM2->DIER = TIM_DIER_UIE;

    	// Make one pulse moder
    	TIM2->CR1 = TIM_CR1_CEN | TIM_CR1_OPM;

    	colNum = 0;				// Column 0
        EXTI->PR = EXTI_PR_PR0; // Clear pending bit
    }
}

void EXTI1_IRQHandler(void) {
    if ((EXTI->PR & EXTI_PR_PR1) == EXTI_PR_PR1) {
    	// Turn off EXTI Interrupt
    	EXTI->IMR &= ~(EXTI_IMR_IM0 | EXTI_IMR_IM1 | EXTI_IMR_IM2 | EXTI_IMR_IM3);

    	// Settings for tim2
    	TIM2->ARR = 80000 - 1; // 5ms
    	TIM2->CNT = 0;
    	TIM2->SR = 0;
    	TIM2->DIER = TIM_DIER_UIE;

    	// Make one pulse moder
    	TIM2->CR1 = TIM_CR1_CEN | TIM_CR1_OPM;

    	colNum = 1;			// Column 1
        EXTI->PR = EXTI_PR_PR1; // Clear pending bit
    }
}

void EXTI2_IRQHandler(void) {
    if ((EXTI->PR & EXTI_PR_PR2) == EXTI_PR_PR2) {
    	// Turn off EXTI Interrupt
    	EXTI->IMR &= ~(EXTI_IMR_IM0 | EXTI_IMR_IM1 | EXTI_IMR_IM2 | EXTI_IMR_IM3);

    	// Settings for tim2
    	TIM2->ARR = 80000 - 1; // 5ms
    	TIM2->CNT = 0;
    	TIM2->SR = 0;
    	TIM2->DIER = TIM_DIER_UIE;

    	// Make one pulse moder
    	TIM2->CR1 = TIM_CR1_CEN | TIM_CR1_OPM;

    	colNum = 2;			// Column 2
        EXTI->PR = EXTI_PR_PR2; // Clear pending bit
    }
}

void EXTI3_IRQHandler(void) {
    if ((EXTI->PR & EXTI_PR_PR3) == EXTI_PR_PR3) {
    	// Turn off EXTI Interrupt
    	EXTI->IMR &= ~(EXTI_IMR_IM0 | EXTI_IMR_IM1 | EXTI_IMR_IM2 | EXTI_IMR_IM3);

    	// Settings for tim2
    	TIM2->ARR = 80000 - 1; // 5ms
    	TIM2->CNT = 0;
    	TIM2->SR = 0;
    	TIM2->DIER = TIM_DIER_UIE;

    	// Make one pulse moder
    	TIM2->CR1 = TIM_CR1_CEN | TIM_CR1_OPM;

    	colNum = 3;			// Column 3

        EXTI->PR = EXTI_PR_PR3; // Clear pending bit
    }
}

void TIM2_IRQHandler() {
	if((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF) {
		keyPressed = KeypadStatus();

        // Re-enable interrupts
        EXTI->IMR |= EXTI_IMR_IM0 | EXTI_IMR_IM1 | EXTI_IMR_IM2 | EXTI_IMR_IM3;
        colNum = 0xFF;
        TIM2->SR &= ~TIM_SR_UIF;
	}
}
