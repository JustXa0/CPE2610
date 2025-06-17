/*
 * accel.h
 *
 *  Created on: May 7, 2025
 *      Author: prendivillej
 */

#ifndef ACCEL_H_
#define ACCEL_H_

#include <stdint.h>

/**
 * ADXL345Init - initializes the I2C component and accelerometer for reads
 * Args - N/A
 * Returns - N/A
 */
extern void ADXL345Init();

/**
 * ADXL345Read - reads from x, y, and z position registers and passes them to addresses specified
 * Args - x, y, z: variables to store read positions
 * Returns - N/A
 */
extern void ADXL345Read(int16_t* x, int16_t* y, int16_t* z);

#endif /* ACCEL_H_ */
