/*
 * motor_drive.c
 *
 * Created: 2025. 10. 14. 16:33:07
 *  Author: bagob
 */ 

 #include "motor_drive.h"

 #include "main.h"
 #include "math.h"


//tmp portkonfig

		//#define MAX_SPEED_LEVEL 6000 // ez számított
		#define T_MAX 109.54451f

		#define S_NUM_MIN 1800 //50 -100?
		#define DT_0 840000UL 			// 10ms
		//#define TIMER_CONST 644095.0f	// 1/16 max speed --> dt = 70us
		#define TIMER_CONST 322047.0f	// 1/32 max speed --> dt = 70us
		//static float a = 1.0f; // 0.8 mar gecigyors
		//static int s_current = 0;
		static uint32_t max_speed_level = 30000;// ez majd lehet nem kell -- ideiglenesen
		static float curve_max_speed = 0.0f;
		static uint32_t curve_n = 0;
		static float v_bottom = 0;
		static float v_top = 0;


		//bemenő paraméterek
		static uint32_t curve_n_used = 9000; 	// ennyi lépés alatt gyorsul v max ra
		#define V_MAX 90.54451f 			//50 -100?

		static float curve_bottom = 0.4f; 	// 0-1 közötti szám
		static float curve_top = 1.0f;		//0-1 közötti szám

		static uint32_t curve_level_bottom = 0;
		static uint32_t curve_level_top = 0;


 //static uint16_t time_levels[301] = {17289, 17289, 7161, 5495, 4633, 4081, 3690, 3393, 3158, 2966, 2806, 2668, 2550, 2446, 2353, 2270, 2196, 2128, 2067, 2010, 1958, 1909, 1864, 1823, 1783, 1747, 1712, 1679, 1649, 1619, 1592, 1565, 1540, 1516, 1494, 1472, 1451, 1431, 1412, 1393, 1375, 1358, 1342, 1326, 1311, 1296, 1282, 1268, 1254, 1241, 1229, 1216, 1205, 1193, 1182, 1171, 1160, 1150, 1140, 1130, 1121, 1111, 1102, 1093, 1085, 1076, 1068, 1060, 1052, 1044, 1037, 1030, 1022, 1015, 1008, 1002, 995, 988, 982, 976, 970, 963, 958, 952, 946, 940, 935, 929, 924, 919, 914, 909, 904, 899, 894, 889, 885, 880, 875, 871, 867, 862, 858, 854, 850, 846, 842, 838, 834, 830, 826, 822, 819, 815, 811, 808, 804, 801, 797, 794, 791, 788, 784, 781, 778, 775, 772, 769, 766, 763, 760, 757, 754, 751, 748, 745, 743, 740, 737, 735, 732, 729, 727, 724, 722, 719, 717, 714, 712, 709, 707, 705, 702, 700, 698, 695, 693, 691, 689, 687, 684, 682, 680, 678, 676, 674, 672, 670, 668, 666, 664, 662, 660, 658, 656, 654, 653, 651, 649, 647, 645, 643, 642, 640, 638, 636, 635, 633, 631, 630, 628, 626, 625, 623, 621, 620, 618, 617, 615, 614, 612, 611, 609, 607, 606, 605, 603, 602, 600, 599, 597, 596, 594, 593, 592, 590, 589, 588, 586, 585, 583, 582, 581, 580, 578, 577, 576, 574, 573, 572, 571, 569, 568, 567, 566, 565, 563, 562, 561, 560, 559, 557, 556, 555, 554, 553, 552, 551, 549, 548, 547, 546, 545, 544, 543, 542, 541, 540, 539, 538, 537, 536, 535, 534, 533, 532, 531, 530, 529, 528, 527, 526, 525, 524, 523, 522, 521, 520, 519, 518, 517, 516, 515, 514, 513, 513, 512, 511, 510, 509, 508, 507, 506, 505, 505, 504, 503, 502, 501, 500, 500,};
 
 static int32_t current_level = 0;
 static int32_t input_pos = 0;		// bemenet --> majd �ttenni 32 bitesre �ket
 static int32_t current_pos = 0;
 static int32_t pos_diff = 0;


 static uint8_t direction = 0;
 static uint8_t motor_enable = 0;
			
 // private 
 static uint32_t  calculate_time()
 {
	// static float t_p = 0;
	 //static float t_c = 0;


	 /*if(shift)t_p = t_c; // irányváltásnál nem shiftelünk

	 t_c = sqrt(2*s/a);  // t pillanatnyi kiszámolása

	 if(t_c > t_p) dt =  t_c - t_p;	//dt kiszámolása--> absz
	 else dt = t_p - t_c;*/
	 // s görbe --> simán current levellel számolva


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

/*
	 if(tmp_level > S_NUM_MIN && (tmp_level<12000))
	 {
	 tmp_ss = (float)tmp_level/12000.0f ;
	 v_s = V_MAX*((3.0f*(tmp_ss*tmp_ss))-(2.0f*(tmp_ss*tmp_ss*tmp_ss)));
	 }else if (tmp_level == S_NUM_MIN) v_s = 0.0;   // v= 0;
	 else if (tmp_level >= 12000) v_s = V_MAX; //max
*/

 }

 //public

 void motor_update_timer(void)
 {
	// motor sebedseg szabalyzas
	 static uint32_t d_t = 0;
	 //static uint32_t curr_s = 0;
	 //static uint32_t prev_s = 0;
	 //static uint8_t count_dir = 0;
	
	if(motor_enable && direction)  
	{
		pos_diff = (input_pos - current_pos)-1;		// -1 hogy ne l�pjen 1 el t�bbet ! 

		if ((pos_diff > current_level) && (current_level < max_speed_level))	  	// gyorsítás
		{
			current_level++;
			/*s_current = current_level;

			if(!count_dir) d_t = calculate_time(s_current, 0); // irányváltás --> nem shiftelés
			else 		   d_t = calculate_time(s_current, 1);
			count_dir = 1;*/
			//calculate_time();
			d_t = calculate_time();
		}
		else if((pos_diff < current_level) && (current_level > 0))
		{
			current_level--;				// lassítás
			/*s_current = current_level-1;
			if(s_current < 0)s_current = 0;

			if(count_dir)  d_t = calculate_time(s_current, 0); // irányváltás --> nem shiftelés
			else 		   d_t = calculate_time(s_current, 1);
			count_dir = 0;*/
			//calculate_time();
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
			/*s_current = current_level;

			if(!count_dir) d_t = calculate_time(s_current, 0); // irányváltás --> nem shiftelés
			else 		   d_t = calculate_time(s_current, 1);
			count_dir = 1;*/
			//d_t = calculate_time();
			d_t = calculate_time();
		}
		else if((pos_diff < current_level) && (current_level > 0))
		{
			current_level--;				// lassítás
			/*s_current = (current_level-1);
			if(s_current < 0)s_current = 0;

			if(count_dir)  d_t = calculate_time(s_current, 0); // irányváltás --> nem shiftelés
			else 		   d_t = calculate_time(s_current, 1);
			count_dir = 0;*/
			//d_t = calculate_time();
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


		//a = ((float)dmx_speed * (0.9f/255.0f))+ 0.1f; // 0.2 --> 1.0 , 0 - 255
		curve_n =(uint32_t)((float)curve_n_used/(curve_top-curve_bottom));
		curve_max_speed = V_MAX;
		curve_level_top = (uint32_t)(curve_top *(float) curve_n);// felső szám érték kiszámolás
		curve_level_bottom = (uint32_t)(curve_bottom *(float)curve_n);

		float tmp_ss = (float)(curve_level_bottom-50)/curve_n ;         //  s/smax
		v_bottom =  curve_max_speed * ((3.0f*(tmp_ss * tmp_ss))-(2.0f*(tmp_ss * tmp_ss * tmp_ss))); //v_s számolása

		tmp_ss = (float)(curve_level_top)/curve_n ;         //  s/smax
		v_top =  curve_max_speed * ((3.0f*(tmp_ss * tmp_ss))-(2.0f*(tmp_ss * tmp_ss * tmp_ss))); //v_s számolása
		v_top = v_top - v_bottom;
				 //v_s = v_s - v_bottom;



		//float tmp =  a/2.0f * (T_MAX*T_MAX);
		//max_speed_level =(uint32_t) tmp;

		//PORT_Write(&MOTOR_1_DIR_PORT,MOTOR_1_DIR_PIN,direction);	// direction pin �ll�t�s
		HAL_GPIO_WritePin(MOTOR_1_DIRECTION_GPIO_Port, MOTOR_1_DIRECTION_Pin, direction);
		//enable_timer_1_out();
		tim_2_set_duty(1);// PWM kimenet enged�lyez�se
				

	}else if((!motor_enable_buf) && ( tim_state > PWM_ON_DUTY) && ( tim_state < DT_0) )
	{	  
		// timer kimenet letilt�sa --> motor stop 

		tim_2_set_duty(0);// PWM lev�laszt�sa

	}
 }


 
