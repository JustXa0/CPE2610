/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab7
 * Dependencies: stdint.h, delay.h, matrix.h
 * Description: Looping program to display different faces
 */

// Macro for the frequency of the processor, 16MHz
#define F_CPU 16000000UL

#include <stdint.h>
#include "delay.h"
#include "matrix.h"

static const uint8_t face_smile[8] = {
	0b00100100,
	0b00100100,
    0b00000000,
    0b00000000,
    0b10000001,
    0b01000010,
    0b00111100,
    0b00000000
};

static const uint8_t face_sad[8] = {
    0b00100100,
    0b00100100,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b10000001,
    0b00000000
};

static const uint8_t face_neutral[8] = {
    0b00100100,
    0b00100100,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b00000000,
    0b00000000
};

static const uint8_t face_cat[8] = {
	0b00000000,
	0b01100110,
	0b01100110,
	0b00000000,
	0b01011010,
	0b01011010,
	0b00100100,
	0b00000000
};

static const uint8_t face_unsure[8] = {
    0b00100100,
    0b00100100,
    0b00000000,
    0b00000000,
    0b00001100,
    0b00011000,
    0b01100000,
    0b00000000
};

// main
int main(void){

	MatrixInit();
	MatrixClear();

	while (1) {
		for(uint8_t row = 0; row < 8; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				if (face_smile[row] & (1 << (7 - col))) {
					MatrixSetPixel(col, row);
				}
			}
		}

		SystemSec(3);

		MatrixClear();

		for(uint8_t row = 0; row < 8; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				if (face_sad[row] & (1 << (7 - col))) {
					MatrixSetPixel(col, row);
				}
			}
		}

		SystemSec(3);

		MatrixClear();

		for(uint8_t row = 0; row < 8; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				if (face_neutral[row] & (1 << (7 - col))) {
					MatrixSetPixel(col, row);
				}
			}
		}

		SystemSec(3);

		MatrixClear();

		for(uint8_t row = 0; row < 8; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				if (face_cat[row] & (1 << (7 - col))) {
					MatrixSetPixel(col, row);
				}
			}
		}

		SystemSec(3);

		MatrixClear();

		for(uint8_t row = 0; row < 8; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				if (face_unsure[row] & (1 << (7 - col))) {
					MatrixSetPixel(col, row);
				}
			}
		}

		SystemSec(3);

		MatrixClear();
	}

	// never return
	for(;;){}

	return 0;
}
