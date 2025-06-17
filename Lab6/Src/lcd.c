/**
 * Name:	Jamie Prendiville
 * Course:	CPE 2610
 * Section:	121
 * Assignment:	Lab 3 - LED Maze
 * File: lcd.c
 * Dependencies:	lcd.h, delay.h, stdbool.h
 * Usage:			API to interact with LCD on board
 */

//Includes
//Necessary standard libraries
#include <lcd.h>
#include <stdbool.h>

//Custom header files
#include "delay.h"

//Static helper functions
/**
 * lcdWriteCmd()
 * Description:
 * 		Sends command over to display.
 * 		Steps:
 * 		1. E=1,RS->CMD,RW->W
 * 		2. CMD-> DB
 * 		3. E=0,RS->CMD,RW->W
 * 		4. Wait appropriate delay or (EC)Poll BF
 * 	Inputs:
 * 		cmd(uint8_t) - cmd to execute on the display
 * 	Outputs: None
 */
static void LcdWriteCmd(uint8_t cmd);


/** EXTRA CREDIT
 * lcdWaitBusyFlag()
 * Description:
 * 		Blocks polling the BusyFlag until it reads '0'
 * 		Note: The mode of DB7 must be switched to input during polling
 * 				and back to output after polling is complete.
 */
static void LcdWaitBusyFlag();


//File static pointers to necessary gpio ports
static volatile gpio_s *const gpioa = (volatile gpio_s *const) GPIOA_BASE;
static volatile gpio_s *const gpioc = (volatile gpio_s *const) GPIOC_BASE;

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
void LcdSetup(){
	// not using gpioc before, need to enable clock
	volatile uint32_t *const rccAbh1enrAddr = (volatile uint32_t *const) RCC_AHB1ENR;
	*rccAbh1enrAddr |= (1 << RCC_AHB1ENR_GPIOC) | (1 << RCC_AHB1ENR_GPIOB) | (1 << RCC_AHB1ENR_GPIOA);
	gpioa->moder |= (0x00555500);
	gpioc->moder |= (0x00150000);
	gpioa->pupdr &= ~(0x00FFFF00);
}

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
void LcdInit() {
    // Wait 40ms for LCD to power up
    SystemMilli(40);

    // RS = 0 (Command mode), RW = 0 (Write mode)
    gpioc->odr &= ~((0b1 << 8) | (0b1 << 9));

    // Send 0x38 (8-bit mode) two times as required for initialization
    for (int i = 0; i < 2; i++) {
        gpioa->odr = (gpioa->odr & ~0x0FF0) | (0x38 << 4); // Set command
        gpioc->odr |= (0b1 << 10);  // Set Enable high
        SystemMicro(1);             // Short delay
        gpioc->odr &= ~(0b1 << 10); // Set Enable low
        SystemMicro(37);
    }

    // Display ON, Cursor ON, Blink ON
    gpioa->odr = (gpioa->odr & ~0x0FF0) | (0x0F << 4);
    gpioc->odr |= (0b1 << 10);
    SystemMicro(1);
    gpioc->odr &= ~(0b1 << 10);
    SystemMicro(37);

    // Clear display
    gpioa->odr = (gpioa->odr & ~0x0FF0) | (0x01 << 4);
    gpioc->odr |= (0b1 << 10);
    SystemMicro(1);
    gpioc->odr &= ~(0b1 << 10);
    SystemMilli(2);  // LCD requires ~1.52ms for clear

    // Entry mode set (increment cursor)
    gpioa->odr = (gpioa->odr & ~0x0FF0) | (0x06 << 4);
    gpioc->odr |= (0b1 << 10);
    SystemMicro(1);
    gpioc->odr &= ~(0b1 << 10);
    SystemMicro(37);
}


/**
 * LcdClear()
 * Description:
 * 		Clear Display
 * Inputs: None
 * Outputs: None
 */
void LcdClear(){
	// set RS & R/W to 0
	gpioc->odr &= ~((0b1 << 8) | (0b1 << 9));

	// sending command
	gpioa->odr = (gpioa->odr & ~0x0FF0) | (0x01 << 4);

	// set enable to high, pause briefly, then set low
	gpioc->odr |= (0b1 << 10);
	SystemMilli(1);
	gpioc->odr &= ~(0b1 << 10);

	// delay for approx 1.52ms per documentation
	SystemMilli(2);
}

/**
 * LcdHome()
 * Description:
 * 		Move Cursor to the home position (0,0)
 * Inputs: None
 * Outputs: None
 */
void LcdHome(){
	// set RS & R/W to 0
	gpioc->odr &= ~((0b1 << 8) | (0b1 << 9));

	// sending command
	gpioa->odr = ((gpioa->odr & ~(0x0FF0)) | (0x02 << 4));

	// set enable to high, pause briefly, then set low
	gpioc->odr |= (0b1 << 10);
	SystemMilli(1);
	gpioc->odr &= ~(0b1 << 10);

	// delay for approx 1.52ms per documentation
	SystemMilli(2);
}

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
void LcdSetDisplay(bool onOff, bool cursor, bool blink){
	// building command
	uint8_t command = (0b1 << 3) | (onOff << 2) | (cursor << 1) | (blink);

	// set RS and R/W to 00
	gpioc->odr &= ~((0b1 << 8) | (0b1 << 9));

	// sending command, delay is handled in this function
	LcdWriteCmd(command);
}

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
void LcdSetPosition(uint8_t row, uint8_t col) {
    // Ensure row and column values are within bounds
    if (row > 1 || col > 15) return;

    // Compute the DDRAM address based on row and column
    uint8_t address = (row == 0) ? (0x00 + col) : (0x40 + col);

    // Send the command to set the cursor position
    gpioc->odr &= ~((0b1 << 8) | (0b1 << 9)); // RS = 0, RW = 0 (Command mode)
    gpioa->odr = (gpioa->odr & ~0x0FF0) | ((0x80 | address) << 4);

    // Enable high, short delay, then low (falling edge)
    gpioc->odr |= (0b1 << 10);
    SystemMicro(1);
    gpioc->odr &= ~(0b1 << 10);

    // Delay for command processing (Minimum 37μs)
    SystemMicro(37);
}


/**
* LcdWriteChar()
* Description:
* 		Writes the character to screen according table in
* 		documentation.
* 		(See lcdWriteCmd for implementation details)
* 	Input:
* 		c(char) - char to write
* 	Output: none
*/
void LcdWriteChar(char c) {
    // RS = 1 (Data mode), RW = 0 (Write mode)
    gpioc->odr |= (0b1 << 8);  // RS high
    gpioc->odr &= ~(0b1 << 9); // RW low

    // Clear PA4-PA11 before setting new data
    gpioa->odr = (gpioa->odr & ~0x0FF0) | ((c & 0xFF) << 4);

    // Enable high
    gpioc->odr |= (0b1 << 10);
    SystemMicro(1);  // Short delay (~1μs for safety)

    // Enable low (falling edge triggers LCD write)
    gpioc->odr &= ~(0b1 << 10);

    // Wait for LCD to process (37μs required)
    SystemMicro(50);
}



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
void LcdWriteStr(const char * const str){
	uint8_t i = 0;
	while (str[i] != '\0') {
		LcdWriteChar(str[i]);
		i++;
	}
}

/** ExtraCredit
 * LcdGetPosition()
 * Description:
 * 		Finds the current position of the cursor.
 * 	Inputs:
 * 		row(uint8_t*) - returned row location by reference
 * 		col(uint8_t*) - returned col location by reference
 */
void LcdGetPosition(uint8_t* const row, uint8_t* const col){

}

/**
 * lcdWriteCmd()
 * Description:
 * 		Sends command over to display.
 * 		Steps:
 * 		1. E=1,RS->CMD,RW->W
 * 		2. CMD-> DB
 * 		3. E=0,RS->CMD,RW->W
 * 		4. Wait appropriate delay or (EC)Poll BF
 * 	Inputs:
 * 		cmd(uint8_t) - cmd to execute on the display
 * 	Outputs: None
 */
static void LcdWriteCmd(uint8_t cmd){
	// set RS and RW to 0
	gpioc->odr &= ~((0b1 << 8) | (0b1 << 9));

	// sending command to command line
	gpioa->odr = ((gpioa->odr & ~(0x0FF0)) | (cmd << 4));

	// set enable to high, pause briefly, then set low
	gpioc->odr |= (0b1 << 10);
	SystemMicro(1);
	gpioc->odr &= ~(0b1 << 10);

	if (cmd == 0x3 || cmd == 0x1) {
		SystemMilli(2);
	} else {
		SystemMicro(37);
	}

}

/** EXTRA CREDIT
 * lcdWaitBusyFlag()
 * Description:
 * 		Blocks polling the BusyFlag until it reads '0'
 * 		Note: The mode of DB7 must be switched to input during polling
 * 				and back to output after polling is complete.
 */
static void LcdWaitBusyFlag(){

}
