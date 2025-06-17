/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab9
 * Dependencies: stdint.h, stm32f411.h
 * Description: Functions to read from temperature sensor
 */

#include "temp.h"
#include "stm32f411.h"

/**
 * TempInit - initializes the ADC to work with the TMP36 sensor attached on PB0
 * Args - N/A
 * Returns- N/A
 */
void TempInit() {
	// start GPIOB and ADC1
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	// set PB0 to analog
	GPIOB->MODER |= (0x3 << GPIO_MODER_MODE0_Pos);

	// no pullup or pulldown resistors
	GPIOB->PUPDR &= ~(0x3 << GPIO_PUPDR_PUPD0_Pos);

	// enable end of conversion interrupt
	ADC1->CR1 |= ADC_CR1_EOCIE;

	// enable conversion irq
	NVIC->ISER[0] = (1 << ADC_IRQn);

	// set channels to be watched
	ADC1->SQR3 = 8;

	// enabling ADC module
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 &= ~ADC_CR2_ALIGN;

	// clear status register
	ADC1->SR = 0;
}

/**
 * ConvertF - takes ADC reading in and converts it to Fahrenheit
 * Args - data: ADC reading to be converted
 * Returns - ADC reading in Fahrenheit
 */
uint16_t ConvertF(uint16_t data) {
	float voltageMv = (data * 3300.0f) / 4095.0f;
	return (uint16_t) ((voltageMv - 500.0f) / 10.0f) * 9.0f / 5.0f + 32.0f;
}

/**
 * ConvertFAdc - takes temperature in Fahrenheit and converts it to the corresponding ADC reading
 * Args - temp: temperature in Fahrenheit to be converted
 * Returns - ADC measurement
 */
uint16_t ConvertFAdc(uint16_t temp) {
	float voltage = 0.5 + (((5.0 / 9.0) * (temp - 32)) * 0.01);
	uint16_t ret = (uint16_t) ((voltage / 3.3) * 4095);
	return ret;
}
