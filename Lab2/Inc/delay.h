/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab1
 * Dependencies: stdint.h
 * Description: Busy time delay functions for micro, milli, and seconds
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

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
void BusyMicro(uint32_t N);

/**
 * @brief Pauses any action from occurring for the set period of time
 * @param N - time to delay for in milliseconds
 */
void BusyMilli(uint32_t N);

/**
 * @brief Pauses any action from occurring for the set period of time
 * @param N - time to delay for in seconds
 */
void BusySec(uint32_t N);



#endif /* DELAY_H_ */
