/**
 * Name: Dr. Livingston
 * Course: CPE2610
 * Assignment: Lab1
 * Dependencies: none
 * Description: Initializes and controls the User Led on the STM32F411
 */

#ifndef FLASH_H_
#define FLASH_H_

#define RCC_AHB1ENR 0x40023830
#define RCC_GPIOA_EN 0

#define GPIOA_MODER 0x40020000
#define GPIOA_ODR 0x40020014
#define GPIOA_USER 5

/**
* Initializes PA5 to output
*/
extern void UserLedInit();

/**
* Turns on LED
*/
extern void UserLedOn();

/**
* Turns off LED
*/
extern void UserLedOff();


#endif /* FLASH_H_ */
