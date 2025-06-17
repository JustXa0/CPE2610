/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab1
 * Dependencies: delay.h
 * Description: Busy time delay functions for micro, milli, and seconds
 */

#include "delay.h"

/**
 * @brief Pauses any action from occurring for the set period of time
 * @param N - time to delay in microseconds
 */
void BusyMicro(uint32_t N) {
	// busy loop (each iteration takes ~2.6 microseconds, so the delay given by user is sufficient)
	for (uint32_t i = 0; i < N; i++);
}

/**
 * @brief Pauses any action from occurring for the set period of time
 * @param N - time to delay for in milliseconds
 */
void BusyMilli(uint32_t N) {
	// calculates the delay in milliseconds using the equation given in class
	uint32_t delay = (uint32_t)(520.0*(((N) - 3.3) / 0.7));
	// busy loop
	for (uint32_t i = 0; i < delay; i++);
}

/**
 * @brief Pauses any action from occurring for the set period of time
 * @param N - time to delay for in seconds
 */
void BusySec(uint32_t N) {
	// calculates the delay in seconds using the equation given in class
	uint32_t delay = (uint32_t)(5200.0*(((N) - 3.3) / 0.7));
	// busy loop
	for (uint32_t i = 0; i < delay; i++);
}


