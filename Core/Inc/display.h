/*
 * display.h
 *
 *  Created on: Dec 21, 2025
 *      Author: Szymon
 */
#include "main.h"

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

extern volatile uint8_t LTDC_ReadyToSwap;
extern uint32_t current_display_layer;
extern LTDC_HandleTypeDef hltdc;

void Initialize_display(void);
void Swap_And_Sync(void);
void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc);

#endif /* INC_DISPLAY_H_ */
