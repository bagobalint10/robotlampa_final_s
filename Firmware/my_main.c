/*
 * my_main.c
 *
 *  Created on: Oct 18, 2025
 *      Author: bagob
 */

#include <control_board_s.h>
#include <my_main.h>

#include <dmx_usart_s.h>
#include <motor_1_drive_s.h>
#include <motor_2_drive_s.h>


#define	PAN_CH 11
#define	PAN_F_CH 12
#define	TILT_CH 13
#define	TILT_F_CH 14
#define	PT_SPEED_CH 15

#define SM_TIME 5							// 70ms egy frame
#define SM_COUNT 17							// 85 ms mintavételezés
#define SM_COUNT_REV 0.058824f				// 17 db interpolálás 5ms enként

static uint16_t dmx_pan_value = 0;
static uint16_t dmx_tilt_value = 0;
static uint8_t  dmx_speed_value = 0;
static uint8_t  reset_f = 0; 				// 0--reset,  > 0 fut a program

static uint16_t dmx_pan_sm_value = 0;
static uint16_t dmx_tilt_sm_value = 0;

static void dmx_channel_map(void)
{
	dmx_pan_value = ((uint16_t)*(dmx_adress_pointer + (PAN_CH-1))<<8)  | ((uint16_t)*(dmx_adress_pointer + (PAN_F_CH-1)));
	dmx_tilt_value = ((uint16_t)*(dmx_adress_pointer + (TILT_CH-1))<<8)  | ((uint16_t)*(dmx_adress_pointer + (TILT_F_CH-1)));
	dmx_tilt_value =  (dmx_tilt_value >> 1);
	dmx_speed_value = *(dmx_adress_pointer + (PT_SPEED_CH-1));
}

static void dmx_signal_smoothing(void)
{
	static uint16_t pan_next = 0;
	static uint16_t pan_prev = 0;
	static uint16_t tilt_next = 0;
	static uint16_t tilt_prev = 0;


	static uint32_t current_time = 0;
	static uint32_t prev_time = 0;
	static uint16_t count = 0;

	current_time = HAL_GetTick();

	if ((uint32_t)(current_time - prev_time) >= SM_TIME)	// időzítés
	{
		prev_time = current_time;
		count++;

		if(count >= SM_COUNT )								// mintavételezés
		{
			count = 0;

			tilt_prev = tilt_next;
			tilt_next = dmx_tilt_value;

			pan_prev = pan_next;
			pan_next = dmx_pan_value;
		}

		float step;											// interpolálás

		step = ((float)tilt_next - tilt_prev) * SM_COUNT_REV; 	// lépéskülönbség / interp. szám
		dmx_tilt_sm_value = (uint16_t)((float)tilt_prev + ((float)count * step));

		step = ((float)pan_next - pan_prev) * SM_COUNT_REV; 	// lépéskülönbség / interp. szám
		dmx_pan_sm_value = (uint16_t)((float)pan_prev + ((float)count * step));
	}
}

void my_main_init(void)
{
	control_board_init();
}

void my_main_loop(void)
{
	reset_f = control_board_main();

	dmx_usart_send();

	dmx_signal_smoothing();

	if(reset_f)
	{
	 dmx_channel_map(); 			// normál futás

	 motor_1_main(dmx_pan_sm_value,  dmx_speed_value);
	 motor_2_main(dmx_tilt_sm_value, dmx_speed_value);
	}
	else							// reset
	{
	 motor_1_main(0 , 0); 			// 0 speed, 0 pos resethez
	 motor_2_main(0 , 0); 			// 0 speed, 0 pos resethez
	}
}

