/*
 * motor_drive.c
 *
 * Created: 2025. 10. 14. 16:33:07
 *  Author: bagob
 */ 

#include <motor_1_drive_s.h>

#include "main.h"
#include "math.h"

#define ZERO_POS 100
#define MAX_POS 100000UL
#define T_MAX 109.54451f
#define DT_0 84000UL 				// 1ms
//#define TIMER_CONST 644095.0f		// 1/16 max speed --> dt = 70us
#define TIMER_CONST 322047.0f		// 1/32 max speed --> dt = 70us
#define SYMMETRY_CONST 0.1f

static uint32_t max_speed_level = 30000;
static int32_t current_level = 0;
static int32_t input_pos = 0;
static int32_t current_pos = MAX_POS;
static int32_t pos_diff = 0;
static uint8_t direction = 0;
static uint8_t motor_enable = 0;

static uint32_t n_max = 30000;
static float v_max = 80.0f;
static float v_start = 1.5f;
static float k_mul   = 0;
static float r_max   = 0;

static uint32_t calculate_time(uint32_t n)
{
	float v_s;

	if(!n) return 0;
	else if(n > n_max) v_s = v_max ;
	else
	{
		float k 	= ((float)(n_max - n) * k_mul) + 1.0f;
		float r_n 	= sqrtf((float)n) * k / r_max;
		v_s 		= ((3.0f*r_n*r_n)-( 2.0f*r_n*r_n*r_n)) * v_max;
	}

	v_s += v_start;
	float dt = 1.0f / v_s;
	dt *= TIMER_CONST;

	return (uint32_t) dt;
}

static void calculate_speed(uint8_t speed)
{
	n_max = 8000 + ((uint32_t)((float)speed/255.0f)*12000);
	v_max = 20.0f + (((float)speed/255.0f)*60.0f);
	v_start = 0.2f +(((float)speed/255.0f)*0.8f);

	k_mul   = (SYMMETRY_CONST / (float)n_max);
	r_max   = sqrtf((float)n_max);
	max_speed_level = n_max;
}

void motor_1_update_timer(void)
{
	static uint32_t d_t = 0;
	
	if(motor_enable && direction)  
	{
		pos_diff = (input_pos - current_pos)-1;

		if ((pos_diff > current_level) && (current_level < max_speed_level))
		{
			current_level++;
			d_t = calculate_time(current_level);
		}
		else if((pos_diff < current_level) && (current_level > 0))
		{
			current_level--;
			d_t = calculate_time(current_level);
		}

		current_pos++;

		if(!d_t) d_t = DT_0;
		tim_2_set_period(d_t);
	}
	else if(motor_enable && (!direction))
	{
		pos_diff = (current_pos - input_pos)-1 ;

		if ((pos_diff > current_level) && (current_level < max_speed_level))
		{
			current_level++;
			d_t = calculate_time(current_level);
		}
		else if((pos_diff < current_level) && (current_level > 0))
		{
			current_level--;
			d_t = calculate_time(current_level);
		}

		current_pos--;

		if(!d_t) d_t = DT_0;
		tim_2_set_period(d_t);
	} 

	if ((!current_level) && motor_enable)
	{
		motor_enable = 0;
	} 
}

void motor_1_main(uint16_t dmx_pos_1, uint8_t dmx_speed)
{
	uint32_t tim_state;
	uint16_t motor_enable_buf;

	input_pos =((int32_t)dmx_pos_1 + ZERO_POS);
	motor_enable_buf = motor_enable;

	tim_state = tim_2_get_value();

	if((!motor_enable_buf) && (input_pos != current_pos) && ( tim_state > PWM_ON_DUTY) && ( tim_state < DT_0))
	{	
		if(input_pos>current_pos)	direction = 1;	// pozitiv  // irany meghat�roz�s
		else 						direction = 0;	// negat�v

		motor_enable = 1;

		calculate_speed(dmx_speed);

		HAL_GPIO_WritePin(MOTOR_1_DIRECTION_GPIO_Port, MOTOR_1_DIRECTION_Pin, direction);
		tim_2_set_duty(1);							// PWM kimenet enged�lyez�se
				
	}else if((!motor_enable_buf) && ( tim_state > PWM_ON_DUTY) && ( tim_state < DT_0) )
	{	  
		tim_2_set_duty(0);	// PWM kikapcsolasa
	}
}

void motor_1_set_0_pos(void)
{
	current_pos = ZERO_POS;
	current_level = 50;
}


 
