/**
 * Name: Dr. Livingston
 * Course: CPE2610 - 121
 * Assignment: Lab2
 * Dependencies: delay.h
 * Description: Information for pushbutton functions
 */

#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_

#include <stdbool.h>
#include <stdint.h>


#define RCC_AHB1ENR 0x40023830
//Enable Pins
#define RCC_AHB1ENR_GPIOA 0
#define RCC_AHB1ENR_GPIOC 2

//Mode Register (offset ) (RM157)
#define GPIOA_MODER 0x40020000
#define GPIOC_MODER 0x40020800

//Constants for modes
#define MODER_IN 0b00
#define MODER_OUT 0b01
#define MODER_ALT 0b10
#define MODER_ANA 0b11
#define MODER_MASK 0b11
#define MODER_WIDTH 2

//IDR Register (offset ) (RM159)
#define GPIOA_IDR 0x40020010
#define GPIOC_IDR 0x40020810

//PUPDR Register (offset ) (RM158)
#define GPIOA_PUPDR 0x4002000C
#define GPIOC_PUPDR 0x4002080C

#define PUPDR_NONE 0b00
#define PUPDR_PU 0b01
#define PUPDR_PD 0b10
#define PUPDR_WIDTH 2

//PortA
#define PB_ENC 12
//PortC
#define PB_USER 13
#define PB_CONTROL 11

/**
 * Initialization functions
 */
extern void PushButtonEncInit();
extern void PushButtonUserInit();
extern void PushButtonControlInit();

/**
 * Read functions
 * Return - true if pressed
 * 			false if not pressed
 */
extern bool PushButtonEncRead();
extern bool PushButtonUserRead();
extern bool PushButtonControlRead();



#endif /* PUSHBUTTON_H_ */
