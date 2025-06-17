/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab3
 * Dependencies: stdint.h
 * Description: Busy time delay functions for micro, milli, and seconds
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

#define SYSTICK_BASE 0xE000E010

// systick register struct
typedef struct {
	uint8_t ctrl;			// offset 0x00
	const uint8_t rsv0;
	uint8_t flag;
	const uint8_t rsv1;
	uint32_t load;			// offset 0x04
	uint32_t val;			// offset 0x08
	const uint32_t calib;	// offset 0x0C
} systick_s;

/**
 * @brief An expression macro used to inline calculation for x millisecond delay
 * @param x - time in milliseconds for delay
 */
#define MILLI(x) (uint32_t)(520.0*(((x) - 3.3) / 0.7))

/**
 * @brief An expression macro used to inline calculation for x second delay
 * @param x - time in seconds for delay
 */
#define SEC(x)	(uint32_t)(5200.0*((x) - 3.3) / 0.7)

/**
 * @brief Pauses any action from occurring for the set period of time
 * @param N - time to delay in microseconds
 */
extern void BusyMicro(uint32_t N);

/**
 * @brief Pauses any action from occurring for the set period of time
 * @param N - time to delay for in milliseconds
 */
extern void BusyMilli(uint32_t N);

/**
 * @brief Pauses any action from occurring for the set period of time
 * @param N - time to delay for in seconds
 */
extern void BusySec(uint32_t N);

/**
 * SystemMicro - Uses the systick timer to delay for a given number of microseconds
 * Args - micro - time in microseconds to delay for
 * Return - N/A
 */
extern void SystemMicro(uint32_t micro);

/**
 * SystemMilli - Uses the systick timer to delay for a given number of milliseconds
 * Args - milli - time in milliseconds to delay for
 * Return - N/A
 */
extern void SystemMilli(uint32_t milli);

/**
 * SystemSec - Uses the systick timer to delay for a given number of seconds
 * Args - sec - time in seconds to delay for
 * Return - N/A
 */
extern void SystemSec(uint32_t sec);

/**
 * TimMicro - Uses the advanced timers to delay for a given number of microseconds
 * Args - micro - time in microseconds to delay for
 * Return - N/A
 */
extern void TimMicro(uint32_t micro);

/**
 * TimMilli - Uses the advanced timers to delay for a given number of milliseconds
 * Args - milli - time in milliseconds to delay for
 * Return - N/A
 */
extern void TimMilli(uint32_t milli);

/**
 * TimSec - Uses the advanced timers to delay for a given number of seconds
 * Args - sec - time in seconds to delay for
 * Return - N/A
 */
extern void TimSec(uint32_t sec);


#endif /* DELAY_H_ */
