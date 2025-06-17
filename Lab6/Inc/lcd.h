/** [COMPLETE THIS!!!!]
 * Name:
 * Course: 
 * Section: 
 * Assignment: 
 * File: lcd.h
 * Dependencies: 
 * 		gpio.h - gpio structs, stdint.h, stdbool.h
 * Usage: API to interact with LCD on device
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

//Find values for these constant in class with Dr. L.
#define DB_OFFSET 4
#define DB_MASK 0xFF
#define BUSYFLAG 11
#define CNTL_OFFSET 8
#define RS 8
#define RW 9
#define E 10
//BSRR Constants
#define RS_DATA (1<<RS)
#define RW_RD (1<<RW)
#define E_SET (1<<E)
#define RS_CMD (1<<(RS+16))
#define RW_WR (1<<(RW+16))
#define E_CLR (1<<(E+16))

#define GPIOA_BASE 0x40020000
#define GPIOC_BASE 0x40020800

#define RCC_AHB1ENR 0x40023830
#define RCC_AHB1ENR_GPIOA 0
#define RCC_AHB1ENR_GPIOB 1
#define RCC_AHB1ENR_GPIOC 2

/**
 * LcdSetup()
 * Description:
 * 		Configure PA[11..4] for use as 8-bit output Databus(DB)
 * 		Configure PC8 as output for RS
 * 		Configure PC9 as output for RW
 * 		Configure PC10 as output for R10
 * 		**For EC: Turn of Input Resistors in PA[11..4]
 * Inputs: None
 * Outputs: None
 */
void LcdSetup();

/**
 * LcdInit()
 * Description:
 * 		Wait 40 MS
 * 		Set Function
 * 			8bit mode DL = 1
 * 			2Line mode N = 1
 * 			5x8 dots   F = 0
 * 		Set Function (again)
 * 		Display ON
 * 			DisplayOn D=1
 * 			CursorOn  C=1
 * 			BlinkOn   B=1
 * 		Display Clear (note longer delay)
 * 		Entry Mode Set
 * 			IncrementAddress I/D=1
 * 			ShiftOff S=0
 * Inputs: None
 * Outputs: None
 */
void LcdInit();

/**
 * LcdClear()
 * Description:
 * 		Clear Display
 * Inputs: None
 * Outputs: None
 */
void LcdClear();

/**
 * LcdHome()
 * Description:
 * 		Move Cursor to the home position (0,0)
 * Inputs: None
 * Outputs: None
 */
void LcdHome();

/**
 * LcdSetDisplay()
 * Description:
 * 		Set/change the display options
 * 	Inputs:
 * 		onOff(bool) - true (display on), false (display off)
 * 		cursor(bool) - true (cursor on), false (cursor off)
 * 		blink(bool) - true (blink on), flase (blink off)
 * 	Outputs: none
 */
void LcdSetDisplay(bool onOff, bool cursor, bool blink);

/**
 * LcdSetPosition()
 * Description:
 * 		Set the position of the cursor.
 * 		Top left (0,0)     Top right (0,15)
 * 		Bottom left (1,0)  Bottom right (1,15)
 * 	Inputs:
 * 		row(uint8_t) - row index, 0-top, 1-bottom
 * 		col(uint8_t) - col index, 0-left, 1-right
 */
void LcdSetPosition(uint8_t row, uint8_t col);

/**
 * LcdWriteChar()
 * Description:
 * 		Writes the character to screen according table in
 * 		documentation.
 * 		(See lcdWriteCmd for implementation details)
 * 		Note: RS = 1 for data
 * 	Input:
 * 		c(char) - char to write
 * 	Output: none
 */
void LcdWriteChar(char c);

/**
 * LcdWriteStr()
 * Description:
 * 		Writes string to the current console.
 * 		Note: no protection provided for overwriting end of screen
 * 	Input:
 * 		str(char*) - string to write
 * 	Output:
 * 		None
 */
void LcdWriteStr(const char * const str);

/** ExtraCredit
 * LcdGetPosition()
 * Description:
 * 		Finds the current position of the cursor.
 * 	Inputs:
 * 		row(uint8_t*) - returned row location by reference
 * 		col(uint8_t*) - returned col location by reference
 */
void LcdGetPosition(uint8_t* const row, uint8_t* const col);

#endif /* LCD_H_ */
