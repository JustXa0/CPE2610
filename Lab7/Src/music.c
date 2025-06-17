/*
 * music.c
 *
 *  Created on: Apr 4, 2025
 *      Author: prendivillej
 */

#include "keypad.h"
#include "music.h"
#include "piezo.h"

// used to store the bpm song should be playing at, can be updated to change note
static uint16_t bpm_i = 0;
static uint16_t beat_i = 0;

// internal arrays to define frequency of note and duration (starts in 4th octave and jumps to 5th)
static uint16_t freq[13] = {440, 466, 494, 523, 554, 587, 622, 659, 698, 734, 784, 831, 0};

uint16_t SetBPM(uint16_t bpm) {
	bpm_i = bpm;
	beat_i = 60000/bpm_i;
	return bpm_i;
}

void PlayNote(tone_e t, note_e n) {
	uint16_t delay = 0;
	switch(n) {
		case 0: delay = beat_i/4; break;
		case 1: delay = beat_i; break;
		case 2: delay = beat_i * 2; break;
		default: delay = beat_i * 4;
	}

	Play(freq[t], delay);
}

uint8_t Control() {
	uint8_t key = KeypadGetKey();
	uint8_t ret = 0;
	if (key != 0) {
		switch (key) {
		case 'E':
			ret = 1;
			break;

		case 'F' :
			ret = 2;
			break;

		}
	}

	return ret;
}
