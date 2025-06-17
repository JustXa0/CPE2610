/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab5
 * Dependencies: tim.h, delay.h, keypad.h, stm32f411.h
 * Description: Functions to read from keypad
 */

#include <stdbool.h>

#include "tim.h"
#include "delay.h"
#include "syscfg.h"
#include "exti.h"
#include "stm32f411.h"
#include "keypad.h"

// stores the char of the key pressed
static volatile char keyPressed;

// checks to ensure a key is ready to be pressed
static volatile bool keyReady = false;

// stores the column where the interrupt was found
static uint8_t colNum = 0;

// w/ help from Braydon Hanson for this character array implementation!
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
	// enable gpioc
	RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOCEN_Pos);

	// Make all gpioc pins 0 - 7 pull-up resistors
	GPIOC->PUPDR = (GPIOC->PUPDR & ~(0xFFFF)) | 0x5555;

    // activate all cols
    GPIOC->ODR |= 0xF;

	// enable sysconfig
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// enable C port interupts for pins [3..0]
	SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI3_PC | SYSCFG_EXTICR1_EXTI2_PC
			| SYSCFG_EXTICR1_EXTI1_PC | SYSCFG_EXTICR1_EXTI0_PC;

	// enable rising edge on pins [3..0]
	EXTI->RTSR |= EXTI_RTSR_TR3 | EXTI_RTSR_TR2 | EXTI_RTSR_TR1
			| EXTI_RTSR_TR0;

	// enable falling edge on pins [3..0]
	EXTI->FTSR |= EXTI_FTSR_TR3 | EXTI_FTSR_TR2 | EXTI_FTSR_TR1
			| EXTI_FTSR_TR0;

	// enable interrupt mask on pins [3..0]
	EXTI->IMR |= EXTI_IMR_IM3 | EXTI_IMR_IM2 | EXTI_IMR_IM1 | EXTI_IMR_IM0;

	// clear flags
	EXTI->PR = (EXTI_PR_PR3 | EXTI_PR_PR2 |
			EXTI_PR_PR1 | EXTI_PR_PR0);

	// enable external interrupts
	NVIC->ISER[0] = (1 << (EXTI3_IRQn)) | (1 << (EXTI2_IRQn))
			| (1 << (EXTI1_IRQn)) | (1 << (EXTI0_IRQn));

    // reset rows as output and columns as input
	GPIOC->MODER = (GPIOC->MODER & ~(0xFFFF)) | (0x55 << 8);

	// enable tim2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// enable tim2 interrupt on nvic
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
		SystemMicro(10);  // wait for circuit to settle

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
	GPIOC->MODER = (GPIOC->MODER & ~(0xFFFF)) | (0x55 << 8);

    // Activate COLs
    GPIOC->ODR &= ~0xF0;

    // returns a char value in the array only if a button was detected, 0 otherwise since
    // it has a weird positon on the keypad
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
    	// turn off external interrupts
    	EXTI->IMR &= ~(EXTI_IMR_IM0 | EXTI_IMR_IM1 | EXTI_IMR_IM2 | EXTI_IMR_IM3);

    	// Settings for tim2
    	TIM2->ARR = 80000 - 1; // 5ms
    	TIM2->CNT = 0;
    	TIM2->SR = 0;
    	TIM2->DIER = TIM_DIER_UIE;

    	// set tim2 to one pulse
    	TIM2->CR1 = TIM_CR1_CEN | TIM_CR1_OPM;

    	colNum = 0;				// Column 0
        EXTI->PR = EXTI_PR_PR0; // Clear pending bit
    }
}

void EXTI1_IRQHandler(void) {
    if ((EXTI->PR & EXTI_PR_PR1) == EXTI_PR_PR1) {
    	// turn off external interrupts
    	EXTI->IMR &= ~(EXTI_IMR_IM0 | EXTI_IMR_IM1 | EXTI_IMR_IM2 | EXTI_IMR_IM3);

    	// Settings for tim2
    	TIM2->ARR = 80000 - 1; // 5ms
    	TIM2->CNT = 0;
    	TIM2->SR = 0;
    	TIM2->DIER = TIM_DIER_UIE;

    	// set tim2 to one pulse
    	TIM2->CR1 = TIM_CR1_CEN | TIM_CR1_OPM;

    	colNum = 1;				// Column 1
        EXTI->PR = EXTI_PR_PR1; // Clear pending bit
    }
}

void EXTI2_IRQHandler(void) {
    if ((EXTI->PR & EXTI_PR_PR2) == EXTI_PR_PR2) {
    	// turn off external interrupts
    	EXTI->IMR &= ~(EXTI_IMR_IM0 | EXTI_IMR_IM1 | EXTI_IMR_IM2 | EXTI_IMR_IM3);

    	// Settings for tim2
    	TIM2->ARR = 80000 - 1; // 5ms
    	TIM2->CNT = 0;
    	TIM2->SR = 0;
    	TIM2->DIER = TIM_DIER_UIE;

    	// set tim2 to one pulse
    	TIM2->CR1 = TIM_CR1_CEN | TIM_CR1_OPM;

    	colNum = 2;				// Column 2
        EXTI->PR = EXTI_PR_PR2; // Clear pending bit
    }
}

void EXTI3_IRQHandler(void) {
    if ((EXTI->PR & EXTI_PR_PR3) == EXTI_PR_PR3) {
    	// turn off external interrupts
    	EXTI->IMR &= ~(EXTI_IMR_IM0 | EXTI_IMR_IM1 | EXTI_IMR_IM2 | EXTI_IMR_IM3);

    	// Settings for tim2
    	TIM2->ARR = 80000 - 1; // 5ms
    	TIM2->CNT = 0;
    	TIM2->SR = 0;
    	TIM2->DIER = TIM_DIER_UIE;

    	// set tim2 to one pulse
    	TIM2->CR1 = TIM_CR1_CEN | TIM_CR1_OPM;

    	colNum = 3;				// Column 3
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
