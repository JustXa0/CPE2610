/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab5
 * Dependencies: keypad.h, piezo.h, delay.h, tim.h, music.h, lcd.h
 * Description: Hex game on LCD screen
 */

#include <stdbool.h>

#include "keypad.h"
#include "piezo.h"
#include "delay.h"
#include "tim.h"
#include "music.h"
#include "lcd.h"

// structs used to interact with tim3 & tim4
static volatile TIMx_t *const tim3 = (volatile TIMx_t *const) TM3;
static volatile TIMx_t *const tim4 = (volatile TIMx_t *const) TM4;

// array used to convert uint to chars for numbers
static const char deci[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

// array used to play scale
static music_t scale[] = {{C, HALF, "C "},
		{D, HALF, "D "}, {E, HALF, "E "},
		{F, HALF, "F "}, {G, HALF, "G "},
		{A, HALF, "A "}, {B, HALF, "B "},
		{Cs, HALF, "C#"}};

// array used to play twinkle twinkle
static music_t twinkle[] = {
    {C, FOURTH, "C "}, {C, FOURTH, "C "},
    {G, FOURTH, "G "}, {G, FOURTH, "G "},
    {A, FOURTH, "A "}, {A, FOURTH, "A "},
    {G, HALF,   "G "}, {REST, FOURTH, "RS"},

    {F, FOURTH, "F "}, {F, FOURTH, "F "},
    {E, FOURTH, "E "}, {E, FOURTH, "E "},
    {D, FOURTH, "D "}, {D, FOURTH, "D "},
    {C, HALF,   "C "}, {REST, FOURTH, "RS"},

    {G, FOURTH, "G "}, {G, FOURTH, "G "},
    {F, FOURTH, "F "}, {F, FOURTH, "F "},
    {E, FOURTH, "E "}, {E, FOURTH, "E "},
    {D, HALF,   "D "}, {REST, FOURTH, "RS"},

    {G, FOURTH, "G "}, {G, FOURTH, "G "},
    {F, FOURTH, "F "}, {F, FOURTH, "F "},
    {E, FOURTH, "E "}, {E, FOURTH, "E "},
    {D, HALF,   "D "}, {REST, FOURTH, "RS"},

    {C, FOURTH, "C "}, {C, FOURTH, "C "},
    {G, FOURTH, "G "}, {G, FOURTH, "G "},
    {A, FOURTH, "A "}, {A, FOURTH, "A "},
    {G, HALF,   "G "}, {REST, FOURTH, "RS"},

    {F, FOURTH, "F "}, {F, FOURTH, "F "},
    {E, FOURTH, "E "}, {E, FOURTH, "E "},
    {D, FOURTH, "D "}, {D, FOURTH, "D "},
    {C, WHOLE,  "C "}
};

// enums used to store current song playing and if a new song is requested
static volatile song_e currentSong = SONG_NONE;
static volatile song_e requestedSong = SONG_NONE;

// boolean used to handle changing song logic upon request
static volatile bool changeSong = false;

// boolean to ensure next note is played only after first note ends
static volatile bool newNote = false;

// boolean used to handle playing in reverse
static volatile bool playReverse = false;

// Macro for the frequency of the processor, 16MHz
#define F_CPU 16000000UL

void TIM4_IRQHandler();

/**
 * Controls - handles all features of keypad, pause, play, change bpm
 * Args - N/A
 * Return - N/A
 */
void Controls();

/**
 * PlaySong - handles playing each note of the song
 * Args - song -> enumeration used to control which song is played
 * Return - N/A
 */
void PlaySong(song_e song);

// main
int main(void){

	KeypadInit();
	PiezoInit();
	LcdSetup();
	LcdInit();
	uint8_t key = 0;
	uint16_t bpm = 0;

	LcdWriteStr("Welcome!");
	LcdSetPosition(1,0);
	LcdWriteStr("Play a tune!");
	SystemMilli(500);

	LcdClear();
	LcdWriteStr("Enter a BPM");
	LcdSetPosition(1,0);
	LcdWriteStr("BPM:");
	for (uint8_t i = 0; i < 4; i++) {
		while((key = KeypadGetKey()) == 0);
		LcdWriteChar(key);
		bpm = (10* bpm) + (key - '0');
	}
	SetBPM(bpm);

	// home screen
	LcdClear();
	LcdWriteStr("A: Scale");
	LcdSetPosition(1,0);
	LcdWriteStr("B: Custom");

	while ((key = KeypadGetKey()) != 'A' && key != 'B');

	// main loop for music
	while(1) {
		switch(key = KeypadGetKey()) {
		case 'A': requestedSong = SONG_SCALE; break;
		case 'B': requestedSong = SONG_TWINKLE; break;
		default: break;
		}

		if (requestedSong != SONG_NONE) {
			changeSong = false;
			PlaySong(requestedSong);
			requestedSong = SONG_NONE;
		}
	}

	// never return
	for(;;){}

	return 0;
}

/**
 * Controls - handles all features of keypad, pause, play, change bpm
 * Args - N/A
 * Return - N/A
 */
void Controls() {
	static uint8_t prevKey = 0;
	uint8_t key = KeypadGetKey();
	// if a key was pressed, we will catch it here
	if (key != 0 && key != prevKey) {
		switch (key) {
			// pause
			case 'F': {
				// blocking wait until key is recieved that unpauses
				while ((key = KeypadGetKey()) != 'E');
				break;
			}

			case '0': {
					playReverse = !playReverse;
				break;
			}

			// change bpm
			case '1': {
				uint16_t bpm = 0;
				LcdClear();
				LcdWriteStr("Enter a BPM");
				LcdSetPosition(1,0);
				LcdWriteStr("BPM:");
				for (uint8_t i = 0; i < 4; i++) {
					while((key = KeypadGetKey()) == 0);
					LcdWriteChar(key);
					bpm = (10* bpm) + (key - '0');
				}
				SetBPM(bpm);

				// restore previous song title and label
				LcdClear();
				if (currentSong == SONG_SCALE) {
					LcdWriteStr("Title: Scale");
				} else if (currentSong == SONG_TWINKLE) {
					LcdWriteStr("Title: Twinkle");
				}

				LcdSetPosition(1,0);
				LcdWriteStr("Note:");
				LcdSetPosition(1,7);
				LcdWriteStr("Num:");
				break;
			}

			// play scale
			case 'A': {
				if (currentSong != SONG_SCALE) {
					requestedSong = SONG_SCALE;
					changeSong = true;
				}
				break;
			}

			// play twinkle
			case 'B': {
				if (currentSong != SONG_TWINKLE) {
					requestedSong = SONG_TWINKLE;
					changeSong = true;
				}
				break;
			}
		}
	}

	// remember previous song to help prevent reversing from happening multiple times
	prevKey = key;
}

/**
 * PlaySong - handles playing each note of the song
 * Args - song -> enumeration used to control which song is played
 * Return - N/A
 */
void PlaySong(song_e song) {
	currentSong = song;
	LcdClear();

	switch (song) {
	case SONG_SCALE: {
		LcdClear();
		LcdWriteStr("Title: Scale");
		LcdSetPosition(1, 0);
		LcdWriteStr("Note:");
		LcdSetPosition(1, 7);
		LcdWriteStr("Num:");

		uint8_t i = playReverse ? 7 : 0;
		while (i >= 0 && i < 8) {
			if (changeSong) return;

			Controls();
			while (newNote && !changeSong);

			LcdSetPosition(1, 5);
			LcdWriteStr(scale[i].name);
			LcdSetPosition(1, 11);
			LcdWriteChar(deci[i + 1]);

			PlayNote(scale[i].tone, scale[i].note);
			newNote = true;

			playReverse ? i-- : i++;
		}
	}
	break;

	case SONG_TWINKLE: {
		LcdClear();
		LcdWriteStr("Title: Twinkle");
		LcdSetPosition(1, 0);
		LcdWriteStr("Note:");
		LcdSetPosition(1, 7);
		LcdWriteStr("Num:");

		int8_t i = playReverse ? 54 : 0;
		while (i >= 0 && i < 55) {
		    if (changeSong) return;

		    Controls();
		    while (newNote && !changeSong);

		    if (changeSong) return;

		    LcdSetPosition(1, 5);
		    LcdWriteStr(twinkle[i].name);
		    LcdSetPosition(1, 11);

		    uint8_t n = i + 1;
		    if (n >= 10) {
		        uint8_t temp = n, divisor = 1;
		        while (temp >= 10) {
		            temp /= 10;
		            divisor *= 10;
		        }
		        temp = n;
		        while (divisor > 0) {
		            LcdWriteChar(deci[temp / divisor]);
		            temp %= divisor;
		            divisor /= 10;
		        }
		    } else {
		        LcdWriteChar(deci[n]);
		        LcdWriteChar(' ');
		    }

		    PlayNote(twinkle[i].tone, twinkle[i].note);
		    newNote = true;

		    playReverse ? i-- : i++;
		}
	}
	break;

	default: break;
	}
}

// handles time delay of note
void TIM4_IRQHandler() {
	// stop clocks
	tim4->CR1 = 0;
	tim3->CR1 &= ~(0b1);

	// clear update flag on tim4
	tim4->SR &= ~(0b1);
	newNote = false;
}
