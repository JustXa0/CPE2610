/*
 * music.h
 *
 *  Created on: Apr 3, 2025
 *      Author: prendivillej
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#include <stdint.h>

typedef enum {A, As, B, C, Cs, D, Ds, E, F, Fs, G, Gs, REST} tone_e;
typedef enum {SIXTEENTH, FOURTH, HALF, WHOLE} note_e;

typedef struct {
	tone_e tone;
	note_e note;
	char name[3];
} music_t;

extern uint16_t SetBPM(uint16_t bpm);
extern void PlayNote(tone_e t, note_e n);
extern uint8_t Control();




#endif /* MUSIC_H_ */
