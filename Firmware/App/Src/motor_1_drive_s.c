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
//#define DT_0 840000UL 			// 10ms
#define DT_0 84000UL 				// 1ms
//#define TIMER_CONST 644095.0f		// 1/16 max speed --> dt = 70us
#define TIMER_CONST 322047.0f		// 1/32 max speed --> dt = 70us


static uint32_t max_speed_level = 30000;	// ez majd lehet nem kell -- ideiglenesen
static int32_t current_level = 0;
static int32_t input_pos = 0;				// bemenet --> majd �ttenni 32 bitesre �ket
static int32_t current_pos = MAX_POS;		// indulásnál max érték
static int32_t pos_diff = 0;
static uint8_t direction = 0;
static uint8_t motor_enable = 0;

static uint32_t n_max = 30000; 	//min 8000 - max 30000
static float v_max = 80.0f; 	// min 20 - max 80
static float v_start = 1.5f; 	//min 0.2 - max 1.5  --> 10ms
static float k_mul   = 0;
static float r_max   = 0;  		// hogy a végeredmény 0-1 közé essen
#define SYMMETRY_CONST 0.1f 	//0.1-0.5

			
 // private 
static uint32_t  calculate_time(uint32_t n)
{
	float v_s;

	if(!n) return 0; 					//0 sebességnál 0
	else if(n > n_max) v_s = v_max ;	//max sebesség felett = max
	else
	{
	 float k 	= ((float)(n_max - n) * k_mul) + 1.0f;				// korrigáció
	 float r_n 	= sqrtf((float)n) * k / r_max;    					// gyök n + korrigáció
	 v_s 		= ((3.0f*r_n*r_n)-( 2.0f*r_n*r_n*r_n)) * v_max;		// s görbe számítás
	}

	v_s += v_start;
	float dt = 1.0f / v_s;

	dt *= TIMER_CONST; 					// timer hez illesztés

	return (uint32_t) dt;
}

static void calculate_speed(uint8_t speed) 	//0-255
{
	// calculate speed v2
	// speed ből --> n_max, v_max, v_start

	n_max = 8000 + ((uint32_t)((float)speed/255.0f)*12000); //min 8000 - max 30000
	v_max = 20.0f + (((float)speed/255.0f)*60.0f); // min 20 - max 80
	v_start = 0.2f +(((float)speed/255.0f)*0.8f); 	// min 0.2 - max 1.5  --> 10ms

	k_mul   = (SYMMETRY_CONST / (float)n_max);
	r_max   = sqrtf((float)n_max);  				// hogy a végeredmény 0-1 közé essen
	max_speed_level = n_max;

}

//public

void motor_1_update_timer(void)
{
	// motor sebedseg szabalyzas
	static uint32_t d_t = 0;
	
	if(motor_enable && direction)  
	{
		pos_diff = (input_pos - current_pos)-1;		// -1 hogy ne l�pjen 1 el t�bbet ! 

		if ((pos_diff > current_level) && (current_level < max_speed_level))	// gyorsítás
		{
			current_level++;
			d_t = calculate_time(current_level);
		}
		else if((pos_diff < current_level) && (current_level > 0))
		{
			current_level--;						// lassítás
			d_t = calculate_time(current_level);
		}

		current_pos++;

		if(!d_t) d_t = DT_0;						// mintavételezése idő
		tim_2_set_period(d_t);
	}
	else if(motor_enable && (!direction))
	{
		pos_diff = (current_pos - input_pos)-1 ;	// -1 hogy ne l�pjen 1 el t�bbet !	

		if ((pos_diff > current_level) && (current_level < max_speed_level))
		{
			current_level++;
			d_t = calculate_time(current_level);
		}
		else if((pos_diff < current_level) && (current_level > 0))
		{
			current_level--;						// lassítás
			d_t = calculate_time(current_level);
		}

		current_pos--;

		if(!d_t) d_t = DT_0;						// mintavételezése idő
		tim_2_set_period(d_t);						// timer kompar�l�si ovf �rt�k --> frek frisst�s
	} 

	if ((!current_level) && motor_enable)			// ha lenull�z�dott a sebess�g  
	{
		motor_enable = 0;							// motor tilt�sa -> main b�l lehet �jra ind�tani
	} 
}

void motor_1_main(uint16_t dmx_pos_1, uint8_t dmx_speed)
{
	uint32_t tim_state;								// timer jelenlegi �ll�sa
	uint16_t motor_enable_buf;						// timer-enable, szinkroniz�l�s --> bufferel�s

	input_pos =((int32_t)dmx_pos_1 + ZERO_POS);		// beolvasott poz�ci�
	motor_enable_buf = motor_enable;

	tim_state = tim_2_get_value(); 					// kiolvasni a timer �rt�k�t

	if((!motor_enable_buf) && (input_pos != current_pos) && ( tim_state > PWM_ON_DUTY) && ( tim_state < DT_0))
	{	
		//motor ind�t�s + ir�ny meghat�roz�s
		if(input_pos>current_pos)	direction = 1;	// pozitiv  // ir�ny meghat�roz�s
		else 						direction = 0;	// negat�v

		motor_enable = 1;							// enged�lyez�s

		calculate_speed(dmx_speed);

		HAL_GPIO_WritePin(MOTOR_1_DIRECTION_GPIO_Port, MOTOR_1_DIRECTION_Pin, direction);
		tim_2_set_duty(1);							// PWM kimenet enged�lyez�se
				
	}else if((!motor_enable_buf) && ( tim_state > PWM_ON_DUTY) && ( tim_state < DT_0) )
	{	  
		// timer kimenet letilt�sa --> motor stop 
		tim_2_set_duty(0);// PWM lev�laszt�sa
	}
}

void motor_1_set_0_pos(void) 	// hall hatására fut le --> resetből!!!!
{
	current_pos = ZERO_POS;
	current_level = 50;
}


 
