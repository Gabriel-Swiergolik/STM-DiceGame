/*
 * note.h
 *
 *  Created on: Dec 2, 2025
 *      Author: Szymon
 */

#ifndef INC_NOTE_H_
#define INC_NOTE_H_
#define GET_PITCH(note, octave) \
    ((uint16_t)(note) << (octave-3))
#define REST 0

#include "main.h"

extern ADC_HandleTypeDef hadc1;

typedef struct
{
	uint16_t frequency;
	uint16_t duration;
} Note;

typedef struct
{
	Note *melody;
	uint16_t length;
	uint16_t index;
	uint16_t note_timer;

	uint8_t is_playing;
	uint8_t is_rest;

} Music_player;

typedef enum
{
	WHOLE = 1000,
	HALF = 500,
	QUARTER = 250,
	EIGHTH = 125,
	SIXTEENTH = 62,

	WHOLE_DOT = 1500,
	HALF_DOT = 750,
	QUARTER_DOT = 325,
	EIGHTH_DOT = 187,
	SIXTEENTH_DOT = 93,
} Note_names;

typedef enum
{
	C = 131,
	D = 147,
	E = 165,
	F = 175,
	G = 196,
	A = 220,
	B = 247,

} Note_pitch;

void Set_Tone(uint16_t frequency);
void Change_Melody(Note *new_melody, uint16_t len);
void Scale_Melody(Note *melody, uint16_t len, const uint16_t bpm_base, const uint16_t bpm_current);
float Bpm_Scale(const uint16_t bpm_base, const uint16_t bpm_current);
uint16_t Read_From_ADC(ADC_HandleTypeDef *hadc);


#endif /* INC_NOTE_H_ */
