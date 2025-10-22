/*
 * motor_drive.c
 *
 * Created: 2025. 10. 14. 16:33:07
 *  Author: bagob
 */ 

 #include "motor_drive.h"

 #include "main.h"
 #include "math.h"


// 	s görbe számoláshoz paraméterek
//	bemenő paraméterek
		static uint32_t curve_n_used = 9000; 	// ennyi lépés alatt gyorsul v max ra
		static float curve_bottom = 0.35f; 		// 0-1 közötti szám
		static float curve_top = 1.0f;
		#define V_MAX 100.54451f 				//50 -100?
		#define START_SPEED_S 400 				// ezt majd állítani kéne v0 kezdeti sebesség eltolása

// változók

		//#define MAX_SPEED_LEVEL 6000 // ez számított
 #define T_MAX 109.54451f
 #define DT_0 840000UL 			// 10ms
 //#define TIMER_CONST 644095.0f	// 1/16 max speed --> dt = 70us
 #define TIMER_CONST 322047.0f	// 1/32 max speed --> dt = 70us

 static uint32_t max_speed_level = 30000;// ez majd lehet nem kell -- ideiglenesen
 static float curve_max_speed = 0.0f;
 static uint32_t curve_n = 0;
 static float v_bottom = 0;
 static float v_top = 0;
 static uint32_t curve_level_bottom = 0;
 static uint32_t curve_level_top = 0;
///
 static int32_t current_level = 0;
 static int32_t input_pos = 0;		// bemenet --> majd �ttenni 32 bitesre �ket
 static int32_t current_pos = 0;
 static int32_t pos_diff = 0;


 static uint8_t direction = 0;
 static uint8_t motor_enable = 0;
			
 // private 
 static uint32_t  calculate_time()
 {

	 float tmp_ss, v_s;
	 float dt;

	 uint32_t tmp_level = current_level + curve_level_bottom; // 500-900 / 6000???


	 if(!current_level) return 0; 			//0 sebességnál 0
	 else if(tmp_level > curve_level_top) v_s = v_top;	//max sebesség felett = max
	 else
	 {
		 tmp_ss = (float)tmp_level/curve_n ;         //  s/smax
		 v_s = curve_max_speed * ((3.0f*(tmp_ss * tmp_ss))-(2.0f*(tmp_ss * tmp_ss * tmp_ss))); //v_s számolása
		 v_s = v_s - v_bottom;
	 }

	 dt = (1/v_s); 				// dt számolása v_s ből

	 dt *= TIMER_CONST; 		// timer hez illesztés
	 return (uint32_t) dt;


 }

 //public

 void motor_update_timer(void)
 {
	// motor sebedseg szabalyzas
	 static uint32_t d_t = 0;
	
	if(motor_enable && direction)  
	{
		pos_diff = (input_pos - current_pos)-1;		// -1 hogy ne l�pjen 1 el t�bbet ! 

		if ((pos_diff > current_level) && (current_level < max_speed_level))	  	// gyorsítás
		{
			current_level++;
			d_t = calculate_time();
		}
		else if((pos_diff < current_level) && (current_level > 0))
		{
			current_level--;				// lassítás
			d_t = calculate_time();
		}

		current_pos++;

		if(!d_t) d_t = DT_0;					// mintavételezése idő
		tim_2_set_period(d_t);
	}
	else if(motor_enable && (!direction))
	{
		pos_diff = (current_pos - input_pos)-1 ;	// -1 hogy ne l�pjen 1 el t�bbet !	

		if ((pos_diff > current_level) && (current_level < max_speed_level))
		{
			current_level++;
			d_t = calculate_time();
		}
		else if((pos_diff < current_level) && (current_level > 0))
		{
			current_level--;				// lassítás
			d_t = calculate_time();
		}

		current_pos--;


		if(!d_t) d_t = DT_0;					// mintavételezése idő
		tim_2_set_period(d_t);					// timer kompar�l�si ovf �rt�k --> frek frisst�s
	} 

	if ((!current_level) && motor_enable)			// ha lenull�z�dott a sebess�g  
	{
	motor_enable = 0;								// motor tilt�sa -> main b�l lehet �jra ind�tani 
	} 
 }


 void motor_1_main(uint16_t dmx_pos_1, uint8_t dmx_speed)
 {
	uint32_t tim_state;									// timer jelenlegi �ll�sa
	uint16_t motor_enable_buf;							// timer-enable, szinkroniz�l�s --> bufferel�s

	input_pos =(int32_t)dmx_pos_1;								// beolvasott poz�ci�
	motor_enable_buf = motor_enable;

	tim_state = tim_2_get_value(); // kiolvasni a timer �rt�k�t

	if((!motor_enable_buf) && (input_pos != current_pos) && ( tim_state > PWM_ON_DUTY) && ( tim_state < DT_0))	// �ll a motor + menni k�ne + timer kimenete = 0;
	{	
		//motor ind�t�s + ir�ny meghat�roz�s																												// < time_level[0] --> ovf interrupt ut�n m�g  
		if(input_pos>current_pos)	direction = 1;		// pozitiv  // ir�ny meghat�roz�s
		else 						direction = 0;		// negat�v

		motor_enable = 1;	// enged�lyez�s

// gyorsulás kalkulálás, továbbadás + max_s számolás

		curve_n =(uint32_t)((float)curve_n_used/(curve_top-curve_bottom));
		curve_max_speed = V_MAX;
		curve_level_top = (uint32_t)(curve_top *(float) curve_n);// felső szám érték kiszámolás
		curve_level_bottom = (uint32_t)(curve_bottom *(float)curve_n);

		float tmp_ss = (float)(curve_level_bottom - START_SPEED_S)/curve_n ;     //
		v_bottom =  curve_max_speed * ((3.0f*(tmp_ss * tmp_ss))-(2.0f*(tmp_ss * tmp_ss * tmp_ss))); //v_s számolása

		tmp_ss = (float)(curve_level_top)/curve_n ;         //  s/smax
		v_top =  curve_max_speed * ((3.0f*(tmp_ss * tmp_ss))-(2.0f*(tmp_ss * tmp_ss * tmp_ss))); //v_s számolása
		v_top = v_top - v_bottom;

////
		HAL_GPIO_WritePin(MOTOR_1_DIRECTION_GPIO_Port, MOTOR_1_DIRECTION_Pin, direction);
		//enable_timer_1_out();
		tim_2_set_duty(1);// PWM kimenet enged�lyez�se
				

	}else if((!motor_enable_buf) && ( tim_state > PWM_ON_DUTY) && ( tim_state < DT_0) )
	{	  
		// timer kimenet letilt�sa --> motor stop 

		tim_2_set_duty(0);// PWM lev�laszt�sa

	}
 }


 
