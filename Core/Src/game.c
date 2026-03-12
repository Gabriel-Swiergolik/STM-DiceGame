/*
 * Game.c
 *
 *  Created on: Jan 7, 2026
 *      Author: Szymon
 */

#include "game.h"
#include "main.h"
#include "stm32f429i_discovery_lcd.h"
#include "display.h"
#include "dice.h"
#include "stdio.h"

volatile uint8_t selecting_dices = 1;

volatile  uint8_t highlighted_dice_index = 0;
volatile  uint8_t changed_dice = 0;
volatile  uint8_t current_player = PLAYER_1;
volatile  uint8_t dices_left = 6;
volatile  uint8_t dices_in_round = 6;
volatile static uint8_t ones_count = 0;
volatile static uint8_t fives_count = 0;
volatile static uint16_t target_score = 1000;
volatile uint16_t P1_score = 0;
volatile uint16_t P2_score = 0;
volatile uint16_t selected_score = 0;

volatile uint8_t last_selected_state[6] = {0,0,0,0,0,0};

Dice Dices[] =
{
	{0, 0, 1, 6, 0, 0, LCD_COLOR_BLACK, LCD_COLOR_YELLOW},
	{0, 0, 1, 6, 0, 0, LCD_COLOR_WHITE, LCD_COLOR_BLACK},
	{0, 0, 1, 6, 0, 0, LCD_COLOR_WHITE, LCD_COLOR_BLACK},
	{0, 0, 1, 6, 0, 0, LCD_COLOR_WHITE, LCD_COLOR_BLACK},
	{0, 0, 1, 6, 0, 0, LCD_COLOR_WHITE, LCD_COLOR_BLACK},
	{0, 0, 1, 6, 0, 0, LCD_COLOR_WHITE, LCD_COLOR_BLACK},
};

void Game_Background()
{
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillRect(0, BSP_LCD_GetYSize() / 2, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() / 2);
	BSP_LCD_FillRect(0, BSP_LCD_GetYSize() / 2, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() / 2);

	Swap_And_Sync();

	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillRect(0, BSP_LCD_GetYSize() / 2, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() / 2);
	BSP_LCD_FillRect(0, BSP_LCD_GetYSize() / 2, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() / 2);
}

void Draw_Numer_Of_Dices(uint8_t number_of_dices)
{
	uint8_t frame_number = 0;
	uint16_t x_limit = BSP_LCD_GetXSize();
	uint16_t space = 15;
	uint32_t rnd = 0;

	for(uint8_t i = 0; i < number_of_dices; i++)
	{
		frame_number = 15;

		while(frame_number > 0)
		{
			rnd = (HAL_RNG_GetRandomNumber(&hrng) % 6) + 1;

			uint16_t dice_width_height = Dices[i].scale * 10;
			uint16_t dices_in_row = x_limit / (dice_width_height + space); //dice width
			uint8_t column = i % dices_in_row;
			uint8_t row = i / dices_in_row;

			Dices[i].value = rnd;
			Dices[i].x = space + column * (dice_width_height + space);
			Dices[i].y = (BSP_LCD_GetYSize() / 2) + space + row * (dice_width_height + space); //dice height

			Draw_Dice(Dices[i]);
			Swap_And_Sync();
			Draw_Dice(Dices[i]);

			HAL_Delay(50);

			frame_number--;
		}
	}
}

void Blink_Dice(Dice *dice)
{
	//Usunieta kostka tylko z jednego layera, powinno migac
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillRect(dice->x - 1, dice->y - 1, dice->scale * 10 + 2, dice->scale * 10 + 2);

	Swap_And_Sync();

}

void Mark_Dice(Dice *dice)
{
	//Layer1
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_FillCircle((5 * dice->scale) + dice->x, -(dice->scale+3) + dice->y, dice->scale-1);

	Swap_And_Sync();

	BSP_LCD_FillCircle((5 * dice->scale) + dice->x, -(dice->scale+3) + dice->y, dice->scale-1);
}

void Remove_Dice_Mark(Dice *dice)
{
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillCircle((5 * dice->scale) + dice->x, -(dice->scale+3) + dice->y, dice->scale-1);

	Swap_And_Sync();

	BSP_LCD_FillCircle((5 * dice->scale) + dice->x, -(dice->scale+3) + dice->y, dice->scale-1);
}

void Score_Logic(Dice *dice)
{
	if(dice->value == 1)
	{
		if(dice->score_added == 1 && dice->is_selected == 0)
		{
			ones_count--;
			//dices_left++;
			dice->score_added = 0;

			switch (ones_count)
			{
				case 2:
					selected_score = 0;
					selected_score += 200;
					break;
				case 3:
					selected_score = 0;
					selected_score += 500;
					break;
				case 4:
					selected_score = 0;
					selected_score += 800;
					break;
				case 5:
					selected_score = 0;
					selected_score += 1200;
					break;
				default:
					selected_score -= 100;
					break;
			}
		}
		else if (dice->score_added == 0 && dice->is_selected == 1)
		{
			ones_count++;
			//dices_left--;
			dice->score_added = 1;

			switch (ones_count)
			{
				case 3:
					selected_score = 0;
					selected_score += 500;
					break;
				case 4:
					selected_score = 0;
					selected_score += 800;
					break;
				case 5:
					selected_score = 0;
					selected_score += 1200;
					break;
				case 6:
					selected_score = 0;
					selected_score += 2000;
					break;
				default:
					selected_score += 100;
					break;
			}
		}
	}

	if(dice->value == 5)
	{
		if(dice->score_added == 1 && dice->is_selected == 0)
		{
			fives_count--;
			//dices_left++;
			dice->score_added = 0;

			switch (fives_count)
			{
				case 2:
					selected_score = 0;
					selected_score += 100;
					break;
				case 3:
					selected_score = 0;
					selected_score += 300;
					break;
				case 4:
					selected_score = 0;
					selected_score += 600;
					break;
				case 5:
					selected_score = 0;
					selected_score += 900;
					break;
				default:
					selected_score -= 50;
					break;
			}
		}
		else if (dice->score_added == 0 && dice->is_selected == 1)
		{
			fives_count++;
			//dices_left--;
			dice->score_added = 1;

			switch (fives_count)
			{
				case 3:
					selected_score = 0;
					selected_score += 300;
					break;
				case 4:
					selected_score = 0;
					selected_score += 600;
					break;
				case 5:
					selected_score = 0;
					selected_score += 900;
					break;
				case 6:
					selected_score = 0;
					selected_score += 1500;
					break;
				default:
					selected_score += 50;
					break;
			}
		}
	}
}

void Update_Selected_Score()
{
	//layer1
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(0,0,240,140);


	char target_score_string[20];
	char selected_score_string[20];
	char P1_score_string[20];
	char P2_score_string[20];

	sprintf(target_score_string, "Target: %d", target_score);

	if(current_player == PLAYER_1)
	{
		sprintf(P1_score_string, ">Player1: %d", P1_score);
		sprintf(P2_score_string, " Player2: %d", P2_score);
	}
	else
	{
		sprintf(P1_score_string, " Player1: %d", P1_score);
		sprintf(P2_score_string, ">Player2: %d", P2_score);
	}

	sprintf(selected_score_string, "Selected: %d", selected_score);

	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font20);

	BSP_LCD_DisplayStringAt(30, 10, (uint8_t *)target_score_string, LEFT_MODE);
	BSP_LCD_DisplayStringAt(20, 40, (uint8_t *)P1_score_string, LEFT_MODE);
	BSP_LCD_DisplayStringAt(20, 60, (uint8_t *)P2_score_string, LEFT_MODE);
	BSP_LCD_DisplayStringAt(30, 90, (uint8_t *)selected_score_string, LEFT_MODE);

	Swap_And_Sync();

	//layer2
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(0,0,240,140);

	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font20);

	BSP_LCD_DisplayStringAt(30, 10, (uint8_t *)target_score_string, LEFT_MODE);
	BSP_LCD_DisplayStringAt(20, 40, (uint8_t *)P1_score_string, LEFT_MODE);
	BSP_LCD_DisplayStringAt(20, 60, (uint8_t *)P2_score_string, LEFT_MODE);
	BSP_LCD_DisplayStringAt(30, 90, (uint8_t *)selected_score_string, LEFT_MODE);
}


void Initialize_Game()
{
	Game_Background();
	Update_Selected_Score();
}

void Reset_Game_State()
{
	Game_Background();
	Update_Selected_Score();

	for(uint8_t i = 0; i < 6; i++)
	{
		Dices[i].is_selected = 0;
		Dices[i].score_added = 0;
	  	last_selected_state[i] = 0;
	}
	selecting_dices = 1;
	highlighted_dice_index = 0;
	ones_count = 0;
	fives_count = 0;

	if(dices_left == 0)
		dices_left = 6;

	Draw_Numer_Of_Dices(dices_left);

	dices_in_round = dices_left;
}

void Jinx_Graphics()
{
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(0,0,240,150);
	BSP_LCD_FillRect(0,0,240,150);

	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"J I N X!", CENTER_MODE);

	Swap_And_Sync();
	HAL_Delay(1000);

	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(0,0,240,150);
	BSP_LCD_FillRect(0,0,240,150);

	Swap_And_Sync();
	HAL_Delay(1000);
	Swap_And_Sync();
	HAL_Delay(1000);
	Swap_And_Sync();
	HAL_Delay(1000);

}

void Jinx_Logic()
{
	uint8_t jinx = 1;
	for(uint8_t i = 0; i < dices_in_round; i++)
	{
		if(Dices[i].value == 1 || Dices[i].value == 5)
		{
			jinx = 0;
			break;
		}
	}

	if(jinx)
	{
		selected_score = 0;
		if(current_player == PLAYER_1)
			current_player = PLAYER_2;
		else
			current_player = PLAYER_1;

		Jinx_Graphics();

		dices_left = 6;
		Reset_Game_State();
		Jinx_Logic();
	}

}

void Win_Screen(uint8_t player)
{
	//Zeby kostka nie migala
	Draw_Dice(Dices[highlighted_dice_index]);

	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(0,0,240,150);
	BSP_LCD_FillRect(0,0,240,150);

	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	if(player == PLAYER_1)
	{
		BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"PLAYER1 WINS!", CENTER_MODE);
		BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"PLAYER1 WINS!", CENTER_MODE);
	}
	else
	{
		BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"PLAYER2 WINS!", CENTER_MODE);
		BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"PLAYER2 WINS!", CENTER_MODE);
	}

	Swap_And_Sync();
	HAL_Delay(1000);

	Draw_Dice(Dices[highlighted_dice_index]);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(0,0,240,150);
	BSP_LCD_FillRect(0,0,240,150);

	while(1)
	{
		Swap_And_Sync();
		HAL_Delay(1000);
	}
}

void Win_Condition()
{
	if(P1_score >= target_score)
	{
		Win_Screen(PLAYER_1);
	}
	else if (P2_score >= target_score)
	{
		Win_Screen(PLAYER_2);
	}
}

void Game_Round()
{
	Win_Condition();

	Reset_Game_State();

	Jinx_Logic();

	Dice *current_dice;
	uint8_t blink_state = 0;
	uint32_t last_blink_time = 0;

	while(selecting_dices)
	{
		current_dice = &Dices[highlighted_dice_index];

		if(current_dice->is_selected != last_selected_state[highlighted_dice_index])
		{
			if(current_dice->is_selected == 1)
			{
				Draw_Dice(*current_dice);
				Swap_And_Sync();
				Draw_Dice(*current_dice);

				Mark_Dice(current_dice);
				Mark_Dice(current_dice);

				Score_Logic(current_dice);
				dices_left--;
		    }
			else
			{
				Draw_Dice(*current_dice);
				Swap_And_Sync();
				Draw_Dice(*current_dice);

				Remove_Dice_Mark(current_dice);
				Remove_Dice_Mark(current_dice);
				Score_Logic(current_dice);
				dices_left++;
			}

			Update_Selected_Score();
			Update_Selected_Score();

			last_selected_state[highlighted_dice_index] = current_dice->is_selected;
		}

		if(HAL_GetTick() - last_blink_time > 300)
		{
			last_blink_time = HAL_GetTick();
			blink_state = !blink_state;

			if(!blink_state)
			{
				Blink_Dice(current_dice);
			}
			else
			{
				Draw_Dice(*current_dice);
				Swap_And_Sync();
			}
		}
		if(changed_dice)
		{
			Draw_Dice(Dices[(highlighted_dice_index + dices_in_round - 1) % dices_in_round]);
			Swap_And_Sync();
			Draw_Dice(Dices[(highlighted_dice_index + dices_in_round - 1) % dices_in_round]);
			changed_dice = 0;
		}
	}
}

