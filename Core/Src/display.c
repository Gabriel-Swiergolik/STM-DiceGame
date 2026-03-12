/*
 * display.c
 *
 *  Created on: Dec 21, 2025
 *      Author: Szymon
 */
#include "display.h"
#include "main.h"
#include "stm32f429i_discovery_lcd.h"


void Initialize_display()
{
    BSP_LCD_Init();


    BSP_LCD_LayerDefaultInit(LCD_BACKGROUND_LAYER, LCD_FRAME_BUFFER);
    BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, LCD_FRAME_BUFFER + (240 * 320 * 4));


    BSP_LCD_SelectLayer(LCD_BACKGROUND_LAYER);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
    BSP_LCD_Clear(LCD_COLOR_BLACK);


    BSP_LCD_SetLayerVisible(LCD_FOREGROUND_LAYER, ENABLE);
    BSP_LCD_SetLayerVisible(LCD_BACKGROUND_LAYER, DISABLE);
    current_display_layer = LCD_FOREGROUND_LAYER;
}

void Swap_And_Sync()
{
	extern DMA2D_HandleTypeDef hdma2d;
	HAL_DMA2D_PollForTransfer(&hdma2d, 10);

    HAL_LTDC_ProgramLineEvent(&hltdc, 320);


    while (LTDC_ReadyToSwap == 0)
    	;
    LTDC_ReadyToSwap = 0;


    if (current_display_layer == LCD_FOREGROUND_LAYER)
    {
        BSP_LCD_SetLayerVisible(LCD_BACKGROUND_LAYER, ENABLE);
        BSP_LCD_SetLayerVisible(LCD_FOREGROUND_LAYER, DISABLE);
        BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
        current_display_layer = LCD_BACKGROUND_LAYER;
    }
    else
    {
        BSP_LCD_SetLayerVisible(LCD_FOREGROUND_LAYER, ENABLE);
        BSP_LCD_SetLayerVisible(LCD_BACKGROUND_LAYER, DISABLE);
        BSP_LCD_SelectLayer(LCD_BACKGROUND_LAYER);
        current_display_layer = LCD_FOREGROUND_LAYER;
    }

    HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_VERTICAL_BLANKING);
    while(hltdc.Instance->SRCR != 0UL);
}


void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc)
{
    LTDC_ReadyToSwap = 1;
}
