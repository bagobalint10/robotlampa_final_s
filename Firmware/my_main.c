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

static uint16_t dmx_pan_value = 0;
static uint16_t dmx_tilt_value = 0;
static uint8_t  dmx_speed_value = 0;
static uint8_t  reset_f = 0; 			// 0--reset,  > 0 fut a program

static void dmx_channel_map(void)
{
	dmx_pan_value = ((uint16_t)*(dmx_adress_pointer + (PAN_CH-1))<<8)  | ((uint16_t)*(dmx_adress_pointer + (PAN_F_CH-1)));
	dmx_tilt_value = ((uint16_t)*(dmx_adress_pointer + (TILT_CH-1))<<8)  | ((uint16_t)*(dmx_adress_pointer + (TILT_F_CH-1)));
	dmx_tilt_value =  (dmx_tilt_value >> 1);
	dmx_speed_value = *(dmx_adress_pointer + (PT_SPEED_CH-1));
}

void my_main_init(void)
{
	control_board_init();
}

void my_main_loop(void)
{
	reset_f = control_board_main();

	dmx_usart_send();

	if(reset_f)
	{
	 dmx_channel_map(); 			// normál futás

	 motor_1_main(dmx_pan_value,  dmx_speed_value);
	 motor_2_main(dmx_tilt_value, dmx_speed_value);
	}
	else							// reset
	{
	 motor_1_main(0 , 0); 			// 0 speed, 0 pos resethez
	 motor_2_main(0 , 0); 			// 0 speed, 0 pos resethez
	}
}

