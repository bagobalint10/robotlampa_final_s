/*
 * buttons.c
 *
 * Created: 2025. 10. 10. 22:43:48
 *  Author: bagob
 */ 

#include <buttons_s.h>
#include <interrupt_s.h>
#include "stddef.h"


static volatile uint8_t buttons = 0;	// direktbe beolvasott gomb �rt�kek
static uint8_t db_time = 0;				// debounce time

static uint8_t *button_0_p = NULL;
static uint8_t *button_1_p = NULL;
static uint8_t *button_2_p = NULL;
static uint8_t *button_3_p = NULL;

// private

static void pcint_c_callback(uint16_t GPIO_pin)		// gomb ext interrupt callback
{
	switch(GPIO_pin)
	{
	case GOMB_UP_Pin :

		buttons  = (buttons & ~(0x01));
		buttons |= HAL_GPIO_ReadPin(GOMB_UP_GPIO_Port, GOMB_UP_Pin);
		break;

	case GOMB_DOWN_Pin :

		buttons  = (buttons & ~(0x02));
		buttons |= (HAL_GPIO_ReadPin(GOMB_DOWN_GPIO_Port, GOMB_DOWN_Pin)<<1);
		break;

	case GOMB_ENTER_Pin :

		buttons  = (buttons & ~(0x04));
		buttons |= (HAL_GPIO_ReadPin(GOMB_ENTER_GPIO_Port, GOMB_ENTER_Pin)<<2);
		break;

	case GOMB_MODE_Pin :

		buttons  = (buttons & ~(0x08));
		buttons |= (HAL_GPIO_ReadPin(GOMB_MODE_GPIO_Port, GOMB_MODE_Pin)<<3);
		break;

	default: break;
	}
}

// public

void set_buttons_variables(uint8_t *button_0, uint8_t *button_1, uint8_t *button_2, uint8_t *button_3 )
{
	button_0_p =  button_0;
	button_1_p =  button_1;
	button_2_p =  button_2;
	button_3_p =  button_3;
}

void buttons_init(uint8_t debounce_time)
{
	db_time = debounce_time;						// pergés mentesítési idő

	set_pcint_Callback(0, pcint_c_callback);		// Callback fgv. beallitasa

	buttons  = (buttons & ~(0x01));					// kezdő értékek kiolvasása
	buttons |= HAL_GPIO_ReadPin(GOMB_UP_GPIO_Port, GOMB_UP_Pin);

	buttons  = (buttons & ~(0x02));
	buttons |= (HAL_GPIO_ReadPin(GOMB_DOWN_GPIO_Port, GOMB_DOWN_Pin)<<1);

	buttons  = (buttons & ~(0x04));
	buttons |= (HAL_GPIO_ReadPin(GOMB_ENTER_GPIO_Port, GOMB_ENTER_Pin)<<2);

	buttons  = (buttons & ~(0x08));
	buttons |= (HAL_GPIO_ReadPin(GOMB_MODE_GPIO_Port, GOMB_MODE_Pin)<<3);

}

void button_read(void)
{
	static uint32_t prev_time = 0;					// perg�s mentes�t�shez v�ltoz�k
	static uint16_t interval_time = 0;

	uint32_t current_time = HAL_GetTick();
	interval_time = db_time;						//devounce time be�ll�t�sa

	if ((uint32_t)(current_time - prev_time)>= interval_time)  //perg�s mentes�t�s
	{
		prev_time = current_time;

		*button_0_p = ((buttons & 0x01) && 0x01);	//pointereknek gomb �rt�k�nek �tad�sa
		*button_1_p = ((buttons & 0x02) && 0x01);
		*button_2_p = ((buttons & 0x04) && 0x01);
		*button_3_p = ((buttons & 0x08) && 0x01);
		
		} 
}
