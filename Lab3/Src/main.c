/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab3
 * Dependencies: ledbar.h, pushbutton.h, delay.h, lcd.h, uart_driver.h
 * Description: Maze game on LCD screen
 */



#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "delay.h"
#include "lcd.h"
#include "ledbar.h"
#include "pushbutton.h"
#include "uart_driver.h"

// Macro for the frequency of the processor, 16MHz
#define F_CPU 16000000UL

// Game Constants
#define PLAYER_CHAR 	0xEF
#define GOAL_CHAR		'G'
#define OBSTACLE_CHAR	'#'
#define GOAL_ROW		1
#define GOAL_COL		15

// maze to be drawn
static const char maze[2][16] = {
		{PLAYER_CHAR, ' ', ' ', OBSTACLE_CHAR, ' ', ' ', ' ', ' ',
				OBSTACLE_CHAR, ' ', ' ', ' ', OBSTACLE_CHAR, ' ', ' ', ' '},

		{OBSTACLE_CHAR, OBSTACLE_CHAR, ' ', ' ', ' ', OBSTACLE_CHAR,
				OBSTACLE_CHAR, ' ', ' ', ' ', OBSTACLE_CHAR, ' ', ' ',
				' ', OBSTACLE_CHAR, GOAL_CHAR}
};

// 2D array of all obstacle positions
static uint8_t obstacles[9][2] = {
		{1, 0},
		{1, 1},
		{0, 3},
		{1, 5},
		{1, 6},
		{0, 8},
		{1, 10},
		{0, 12},
		{1, 14}
};

static uint8_t row = 0;
static uint8_t col = 0;
static uint8_t lives = 5;

/**
 *	isObstacle - Checks given row and column positions for obstacle
 *	Args - 	row - row position to be checked
 *			col - column position to be checked
 *	Return - true if obstacle is at row, column position
 *			 false otherwise
 */
static bool isObstacle(uint8_t row, uint8_t col);

// main
int main(void){
	//Initializes serial communication, LED bar, pushbuttons, & LCD
	init_usart2(57600,F_CPU);
	LedBarInit();
	PushButtonEncInit();
	PushButtonUserInit();
	PushButtonControlInit();
	LcdSetup();
	LcdInit();

	// Drawing maze
	for (uint8_t col = 0; col < 2; col++) {
		for (uint8_t row = 0; row < 16; row++) {
			LcdSetPosition(col, row);
			LcdWriteChar(maze[col][row]);
		}
	}

	// setting up for start
	LcdSetPosition(0, 0);
	LcdSetDisplay(1, 0, 0);

	// sets led bar to bottom 5
	LedBarSetPattern(0xFC9);
	LedBarShiftRight(1, 0);

	// main game loop
	while ((row != 1 || col != 15) && lives != 0) {
		if (PushButtonUserRead()) { // move between rows
			// statement checks current row value and assigns inverse
			uint8_t newRow = (row == 0) ? 1 : 0;
			// checking to see if new row has obstacle
			if (!isObstacle(newRow, col)) { // no obstacle detected
				// clearing previous player icon
				LcdSetPosition(row, col);
				LcdWriteChar(' ');

				// writing new player icon
				row = newRow;
				LcdSetPosition(row, col);
				LcdWriteChar(PLAYER_CHAR);
			} else { // new row has obstacle
				// clear previous player icon and deduct 1 life
				LcdSetPosition(row, col);
				LcdWriteChar(' ');
				LedBarToggle(lives - 1);
				lives--;

				// flash on obstacle
				LcdSetDisplay(1, 0, 1);
				LcdSetPosition(newRow, col);

				// draw hit icon and flash for effect
				LcdWriteChar('*');
				LcdSetPosition(newRow, col);
				SystemSec(2);

				// reset to starting positions
				LcdSetDisplay(1, 0, 0);
				LcdSetPosition(newRow, col);
				LcdWriteChar(OBSTACLE_CHAR);
				LcdSetPosition(row, col);
				LcdWriteChar(' ');
				row = 0;
				col = 0;
				LcdSetPosition(0, 0);
				LcdWriteChar(PLAYER_CHAR);
			}
		} else if (PushButtonControlRead()) { // move left
			// checking if already at left edge & obstacle to left
			if (col > 0 && !isObstacle(row, col - 1)) { // not at edge, no obstacle
				// moving player icon left 1 position
				LcdSetPosition(row, col);
				LcdWriteChar(' ');
				col--;
				LcdSetPosition(row, col);
				LcdWriteChar(PLAYER_CHAR);
			} else if (isObstacle(row, col - 1)) { // obstacle detected
				// clear previous player icon and deduct 1 life
				LcdSetPosition(row, col);
				LcdWriteChar(' ');
				LedBarToggle(lives - 1);
				lives--;

				// flash on obstacle
				LcdSetDisplay(1, 0, 1);
				LcdSetPosition(row, col - 1);

				// draw hit icon and flash for effect
				LcdWriteChar('*');
				LcdSetPosition(row, col - 1);
				SystemSec(2);

				// reset to starting positions
				LcdSetDisplay(1, 0, 0);
				LcdSetPosition(row, col - 1);
				LcdWriteChar(OBSTACLE_CHAR);
				LcdSetPosition(row, col);
				LcdWriteChar(' ');
				row = 0;
				col = 0;
				LcdSetPosition(0, 0);
				LcdWriteChar(PLAYER_CHAR);
			}

		} else if (PushButtonEncRead()) {	// move right
			// checking if already at right edge & obstacle to right
			if (col < 15 && !isObstacle(row, col + 1)) { // not at edge, no obstacle
				// moving player right 1 position
				LcdSetPosition(row, col);
				LcdWriteChar(' ');
				col++;
				LcdSetPosition(row, col);
				LcdWriteChar(PLAYER_CHAR);
			} else if (isObstacle(row, col + 1)) { // obstacle detected
				// clear previous player icon and deduct 1 life
				LcdSetPosition(row, col);
				LcdWriteChar(' ');
				LedBarToggle(lives - 1);
				lives--;

				// flash on obstacle
				LcdSetDisplay(1, 0, 1);
				LcdSetPosition(row, col + 1);

				// draw hit icon and flash for effect
				LcdWriteChar('*');
				LcdSetPosition(row, col + 1);
				SystemSec(2);

				// reset to starting positions
				LcdSetDisplay(1, 0, 0);
				LcdSetPosition(row, col + 1);
				LcdWriteChar(OBSTACLE_CHAR);
				LcdSetPosition(row, col);
				LcdWriteChar(' ');
				row = 0;
				col = 0;
				LcdSetPosition(0, 0);
				LcdWriteChar(PLAYER_CHAR);
			}
		}
	}
	if (lives == 0) { // game lost
		LcdClear();
		LcdSetPosition(0, 4);
		LcdWriteStr("Game Over");
		LcdSetPosition(1, 3);
		LcdWriteStr("RST TO PLAY");
	} else { // game won
		LcdClear();
		LcdSetPosition(0, 4);
		LcdWriteStr("YOU WON");
		LcdSetPosition(1, 2);
		LcdWriteStr("RESET TO PLAY");
	}

	// never return
	for(;;){}

	return 0;
}

/**
 *	isObstacle - Checks given row and column positions for obstacle
 *	Args - 	row - row position to be checked
 *			col - column position to be checked
 *	Return - true if obstacle is at row, column position
 *			 false otherwise
 */
static bool isObstacle(uint8_t row, uint8_t col) {
	for (uint8_t i = 0; i < 9; i++) {
		if (obstacles[i][0] == row && obstacles[i][1] == col) {
			return true;
		}
	}

	return false;
}

