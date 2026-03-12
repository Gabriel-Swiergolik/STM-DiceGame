/*
 * game.h
 *
 *  Created on: Jan 7, 2026
 *      Author: Szymon
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include "main.h"
#include "dice.h"
extern RNG_HandleTypeDef hrng;

typedef enum
{
	PLAYER_1 = 1,
	PLAYER_2 = 2,
}Players;

void Game_Background();
void Draw_Numer_Of_Dices(uint8_t number_of_dices);
void Initialize_Game();
void Blink_Dice(Dice *dice);
void Mark_Dice(Dice *dice);
void Remove_Dice_Mark(Dice *dice);
void Score_Logic(Dice *dice);
void Update_Selected_Score();
void Game_Round();
void Reset_Game_State();
void Jinx_Graphics();
void Jinx_Logic();
void Win_Screen(uint8_t player);
void Win_Condition();

#endif /* INC_GAME_H_ */
