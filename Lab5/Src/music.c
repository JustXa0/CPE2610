/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab5
 * Dependencies: keypad.h, music.h, piezo.h
 * Description: Functions to make it easy to play music on the piezo speaker
 */

#include "keypad.h"
#include "music.h"
#include "piezo.h"

// used to store the bpm song should be playing at, can be updated to change note
static uint16_t bpm_i = 0;
static uint16_t beat_i = 0;

// internal arrays to define frequency of note and duration (starts in 4th octave and jumps to 5th)
static uint16_t freq[13] = {440, 466, 494, 523, 554, 587, 622, 659, 698, 734, 784, 831, 0};

/**
 * SetBPM - used to set the bpm to calculate the length of the note
 * Args - bpm -> uint16 storing bpm
 * Returns - bpm -> set bpm
 */
uint16_t SetBPM(uint16_t bpm) {
	bpm_i = bpm;
	beat_i = 60000/bpm_i;
	return bpm_i;
}

/**
 * PlayNote - takes an enumeration of supported notes and tones and converts them to correct
 * 			  frequency
 * Args - t -> enumeration of note to be played
 * 		  n -> enumeration of length to be played
 */
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
