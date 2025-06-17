/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab1
 * Dependencies: delay.h
 * Description: Busy time delay functions for micro, milli, and seconds
 */

#include "delay.h"

static volatile systick_s *const systick = (volatile systick_s *const) SYSTICK_BASE;

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

void SystemMicro(uint32_t micro) {
	systick->val = 0;
	systick->load = micro*16;
	systick->ctrl = (0b1 << 2) | (0b1);
	while (systick->flag == 0);

	systick->ctrl = 0;
}

void SystemMilli(uint32_t milli) {
	systick->val = 0;
	systick->load = milli*2000;
	systick->ctrl = (0b1);
	while (systick->flag == 0);

	systick->ctrl = 0;
}

void SystemSec(uint32_t sec) {
	systick->val = 0;
	systick->load = sec*2000000;
	systick->ctrl = (0b1);
	while (systick->flag == 0);

	systick->ctrl = 0;
}


