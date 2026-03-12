/*
 * menu.c
 *
 *  Created on: Dec 24, 2025
 *      Author: Szymon
 */

#include "menu.h"
#include "stm32f429i_discovery_lcd.h"
#include "dice.h"
#include "display.h"

volatile uint8_t menu_playing = 1;

void Draw_Menu_Dice(Dice d1, Dice d2)
{
	//LAYER 1

	uint32_t rnd = (HAL_RNG_GetRandomNumber(&hrng) % 6) + 1;
	d1.value = rnd;

	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillRect(0, 20, 240, 150);   //czarwone tlo
	Draw_Dice(d1);
	Swap_And_Sync();
	HAL_Delay(500);

	//LAYER 2
	d2.value = rnd; //nie zamazujemy malej kostki tlem, bo duza i tak ja pokrywa
	Draw_Dice(d2);
	Swap_And_Sync();
	HAL_Delay(500);
}

void Draw_Menu_Background()
{
	Swap_And_Sync();
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"DICE GAME", CENTER_MODE);
	BSP_LCD_DisplayStringAt(0, 230, (uint8_t*)"START", CENTER_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillRect(0, 20, 240, 150);   //czarwone tlo
	BSP_LCD_FillRect(0, 230, 50, 20);   //pasek lewy
	BSP_LCD_FillRect(0, 230, 50, 20);   //pasek lewy
	BSP_LCD_FillRect(190, 230, 50, 20); //pasek prawy

	Swap_And_Sync();

	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"DICE GAME", CENTER_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillRect(0, 20, 240, 150);   //czarwone tlo
	BSP_LCD_FillRect(0, 230, 50, 20);   //pasek lewy
	BSP_LCD_FillRect(0, 230, 50, 20);   //pasek lewy
	BSP_LCD_FillRect(190, 230, 50, 20); //pasek prawy
}

void Start_Menu()
{
	//Tlo dla menu
	Draw_Menu_Background();
	HAL_Delay(1);
	Draw_Menu_Background();

	Dice d1 = {0, 0, 3, 4, 100, 75, LCD_COLOR_WHITE, LCD_COLOR_BLACK};
	Dice d2 = {0, 0, 5, 7, 85, 60, LCD_COLOR_WHITE, LCD_COLOR_BLACK};

	while(menu_playing)
	      Draw_Menu_Dice(d1, d2);

	BSP_LCD_Clear(LCD_COLOR_BLACK);
	Swap_And_Sync();

	BSP_LCD_Clear(LCD_COLOR_BLACK);
	Swap_And_Sync();
}


