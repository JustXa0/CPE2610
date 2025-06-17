/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab7
 * Dependencies: stdint.h
 * Description: Functions to write to LED array using SPI
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdint.h>

/**
 * MatrixInit - Sets up GPIO port and SPI2 for matrix use
 * Args - N/A
 * Return - N/A
 */
extern void MatrixInit();

/**
 * MatrixSetPixel - turns on pixel at position (x,y)
 * Args - x: x-position of pixel
 * 		  y: y-position of pixel
 * Return - N/A
 */
extern void MatrixSetPixel(uint8_t x, uint8_t y);

/**
 * MatrixClearPixel - turns pixel off at position (x,y)
 * Args - x: x-position of pixel
 * 		  y: y-position of pixel
 * Return - N/A
 */
extern void MatrixClearPixel(uint8_t x, uint8_t y);

/**
 * MatrixClear - clears entire LED matrix
 * Args - N/A
 * Return - N/A
 */
extern void MatrixClear();

#endif /* MATRIX_H_ */
