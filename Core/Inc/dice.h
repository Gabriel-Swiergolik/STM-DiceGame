/*
 * dice.h
 *
 *  Created on: Dec 24, 2025
 *      Author: Szymon
 */

#ifndef INC_DICE_H_
#define INC_DICE_H_

#include "main.h"

typedef struct
{
	uint8_t is_selected;
	uint8_t score_added;
	uint8_t value;
	uint8_t scale;

	uint16_t x;
	uint16_t y;

	uint32_t dice_color;
	uint32_t spot_color;
} Dice;

extern Dice Dices[];

void Draw_Dice(Dice dice);

#endif /* INC_DICE_H_ */
