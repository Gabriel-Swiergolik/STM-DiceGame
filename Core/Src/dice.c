/*
 * dice.c
 *
 *  Created on: Dec 24, 2025
 *      Author: Szymon
 */

#include "dice.h"
#include "main.h"
#include "stm32f429i_discovery_lcd.h"

void Draw_Dice(Dice dice)
{
	//Ladna ramka dla kostki
	BSP_LCD_SetTextColor(dice.spot_color);
	BSP_LCD_DrawRect(dice.x - 1, dice.y - 1, 10*dice.scale + 1, 10*dice.scale + 1);

	//Wybor koloru kostki
	BSP_LCD_SetTextColor(dice.dice_color);
	BSP_LCD_FillRect(dice.x, dice.y, 10*dice.scale, 10*dice.scale);

	//Wybor korou oczek
	BSP_LCD_SetTextColor(dice.spot_color);

	// srodkowe oczko -> 1 3 5
	if(dice.value % 2 != 0)
		BSP_LCD_FillCircle((5 * dice.scale) + dice.x, (5 * dice.scale) + dice.y, dice.scale);

	// l. dolne oraz p. gorne oczko -> 2 3 4 5 6
	if(dice.value > 1)
	{
		BSP_LCD_FillCircle((2 * dice.scale) + dice.x, (8 * dice.scale) + dice.y, dice.scale);
		BSP_LCD_FillCircle((8 * dice.scale) + dice.x, (2 * dice.scale) + dice.y, dice.scale);
	}

	// l. gorne oraz p. dolne oczko -> 4 5 6
	if(dice.value > 3)
	{
		BSP_LCD_FillCircle((2 * dice.scale) + dice.x, (2 * dice.scale) + dice.y, dice.scale);
		BSP_LCD_FillCircle((8 * dice.scale) + dice.x, (8 * dice.scale) + dice.y, dice.scale);
	}

	//boczne oczka dla 6
	if(dice.value == 6)
	{
		BSP_LCD_FillCircle((2 * dice.scale) + dice.x, (5 * dice.scale) + dice.y, dice.scale);
		BSP_LCD_FillCircle((8 * dice.scale) + dice.x, (5 * dice.scale) + dice.y, dice.scale);
	}
}
