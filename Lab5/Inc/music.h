/**
 * Name: James Prendiville
 * Course: CPE2610 - 121
 * Assignment: Lab5
 * Dependencies: keypad.h, music.h, piezo.h
 * Description: Functions to make it easy to play music on the piezo speaker
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#include <stdint.h>

// enum used to organize different tones
typedef enum {A, As, B, C, Cs, D, Ds, E, F, Fs, G, Gs, REST} tone_e;

// enum used to organize different lengths of notes
typedef enum {SIXTEENTH, FOURTH, HALF, WHOLE} note_e;

// enum used to organize different song selections
typedef enum {SONG_NONE, SONG_SCALE, SONG_TWINKLE} song_e;

// struct used to store data about a musical note
typedef struct {
	tone_e tone;
	note_e note;
	char name[3];
} music_t;

/**
 * SetBPM - used to set the bpm to calculate the length of the note
 * Args - bpm -> uint16 storing bpm
 * Returns - bpm -> set bpm
 */
extern uint16_t SetBPM(uint16_t bpm);

/**
 * PlayNote - takes an enumeration of supported notes and tones and converts them to correct
 * 			  frequency
 * Args - t -> enumeration of note to be played
 * 		  n -> enumeration of length to be played
 */
extern void PlayNote(tone_e t, note_e n);

#endif /* MUSIC_H_ */
