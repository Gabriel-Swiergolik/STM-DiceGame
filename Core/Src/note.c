/*
 * note.c
 *
 *  Created on: Dec 2, 2025
 *      Author: G
 */
#include "note.h"
#include "main.h"


Music_player melody_player = { .is_playing = 0 };

void Change_Melody(Note *new_melody, uint16_t len)
{
	melody_player.melody = new_melody;
	melody_player.length = len;
	melody_player.index = 0;
	melody_player.note_timer = 0;
	melody_player.is_playing = 1;
	melody_player.is_rest = 0;
}

float Bpm_Scale(const uint16_t bpm_base, const uint16_t bpm_current)
{
	return (float) bpm_base / bpm_current;
}

void Scale_Melody(Note *melody, uint16_t len, const uint16_t bpm_base, const uint16_t bpm_current)
{
    for(int i = 0; i < len; i++)
    {
        melody[i].duration = (uint16_t)(melody[i].duration * Bpm_Scale(bpm_base, bpm_current));
    }
}

void Set_Tone(uint16_t frequency)
{
    const uint32_t clock_frequency = 45000000;
    const uint32_t arr = 1023;
    uint32_t psc = (clock_frequency / (frequency * (arr + 1))) - 1;   //czestotliwosc ze wzoru z laboratoriow, staly arr dla latwosci obliczen

    TIM3->ARR = arr;
    TIM3->PSC = psc;
}

uint16_t Read_From_ADC(ADC_HandleTypeDef *hadc)
{
	uint32_t val = 0;

	HAL_ADC_Start(hadc);

	if (HAL_ADC_PollForConversion(hadc, 10) == HAL_OK)
	{
	    val = (HAL_ADC_GetValue(hadc) * 50) / 4095;
	}

	HAL_ADC_Stop(hadc);

	return ((uint16_t)val > 50) ? 49 : (uint16_t)val;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        if(melody_player.note_timer > 0)
        	melody_player.note_timer--;
        else
        {
        	if(melody_player.is_rest)
        	{
        		melody_player.is_rest = 0;

        		Note current_note = melody_player.melody[melody_player.index];

        		if(current_note.frequency == REST)
        		{
        			TIM3->CCR2= 0;
        		}
        		else
        		{
            		TIM3->CCR2= Read_From_ADC(&hadc1);
            		Set_Tone(current_note.frequency);
        		}
                melody_player.note_timer = current_note.duration;
                melody_player.index = (melody_player.index + 1) % melody_player.length;
        	}
        	else
        	{
        		melody_player.is_rest = 1;  //aktywacja przerwy, gwarantuje delikatne staccato w melodii.
        		melody_player.note_timer = 20;
        		TIM3->CCR2 = 0;
        	}
        }
    }
}


