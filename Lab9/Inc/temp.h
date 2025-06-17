/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab9
 * Dependencies: stdint.h, stm32f411.h
 * Description: Functions to read from temperature sensor
 */

#ifndef TEMP_H_
#define TEMP_H_

#include <stdint.h>

/**
 * TempInit - initializes the ADC to work with the TMP36 sensor attached on PB0
 * Args - N/A
 * Returns- N/A
 */
void TempInit();

/**
 * ConvertF - takes ADC reading in and converts it to Fahrenheit
 * Args - data: ADC reading to be converted
 * Returns - ADC reading in Fahrenheit
 */
uint16_t ConvertF(uint16_t data);

/**
 * ConvertFAdc - takes temperature in Fahrenheit and converts it to the corresponding ADC reading
 * Args - temp: temperature in Fahrenheit to be converted
 * Returns - ADC measurement
 */
uint16_t ConvertFAdc(uint16_t temp);

#endif /* TEMP_H_ */
