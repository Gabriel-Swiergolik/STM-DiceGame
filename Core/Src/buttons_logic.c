/*
 * buttons_logic.c
 *
 *  Created on: Jan 11, 2026
 *      Author: Szymon
 */

#include "buttons_logic.h"
#include "menu.h"
#include "game.h"

extern uint8_t menu_playing;
extern uint8_t highlighted_dice_index;
extern uint8_t changed_dice;
extern uint8_t current_player;
extern uint8_t selecting_dices;
extern uint8_t dices_left;
extern uint8_t dices_in_round;
extern uint16_t P1_score;
extern uint16_t P2_score;
extern uint16_t selected_score;

extern Dice Dices[];

uint32_t time_from_last_click = 0;        // czas od ostatniego klikniecia (interrupta w tym przypadku)
uint32_t current_time = 0;                //obecny czas
const uint32_t button_cooldown_time = 200; // max 1 przerwanie co 50ms, powinno usunac miotanie sie sygnalu

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint32_t current_time = HAL_GetTick();

    if((current_time - time_from_last_click) > button_cooldown_time)
    {
    	time_from_last_click = current_time;

        if(GPIO_Pin == GPIO_PIN_0)
        {
            menu_playing = 0;
        }
        else if(GPIO_Pin == GPIO_PIN_12)
        {
            highlighted_dice_index = (highlighted_dice_index + 1) % dices_in_round;
            changed_dice = 1;
        }
        else if(GPIO_Pin == GPIO_PIN_13)
        {
            Dices[highlighted_dice_index].is_selected = !Dices[highlighted_dice_index].is_selected;
        }
        else if(GPIO_Pin == GPIO_PIN_4)
        {
        	uint8_t any_dice_selected = 0;
        	for(uint8_t i; i < 6; i++)
        	{
        		if(Dices[i].is_selected == 1)
        		{
        			any_dice_selected = 1;
        		}
        	}

        	if(any_dice_selected)
        		selecting_dices = 0;
        }
        else if(GPIO_Pin == GPIO_PIN_5)
        {
        	selecting_dices = 0;

            if(current_player == PLAYER_1)
            {
            	P1_score += selected_score;
            	current_player = PLAYER_2;
            }
            else
            {
            	P2_score += selected_score;
            	current_player = PLAYER_1;
            }

            dices_left = 6;
            selected_score = 0;
        }
    }
}
