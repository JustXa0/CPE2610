/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab7
 * Dependencies: stdint.h
 * Description: SPI2 functions
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

/**
 * SpiInit - Initializes SPI2 and sets GPIO ports needed to AF (PB15 & PB13)
 * Args - N/A
 * Return - N/A
 */
extern void SpiInit();

/**
 * SpiWrite - Writes 16 bits of data to SPI data register to be transmitted, waits until tranmission is complete
 * Args - data: 2 bytes of data to be sent
 * Return - N/A
 */
extern void SpiWrite(uint16_t data);

#endif /* SPI_H_ */
