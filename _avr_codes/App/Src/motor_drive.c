/*
 * motor_drive.c
 *
 * Created: 2025. 10. 14. 16:33:07
 *  Author: bagob
 */ 

 #include "motor_drive.h"
 #include "port_config.h"
 #include "interrupt.h"
 #include "gpio.h"
 #include "timer.h"

 static uint16_t time_levels[301] = {17289, 17289, 7161, 5495, 4633, 4081, 3690, 3393, 3158, 2966, 2806, 2668, 2550, 2446, 2353, 2270, 2196, 2128, 2067, 2010, 1958, 1909, 1864, 1823, 1783, 1747, 1712, 1679, 1649, 1619, 1592, 1565, 1540, 1516, 1494, 1472, 1451, 1431, 1412, 1393, 1375, 1358, 1342, 1326, 1311, 1296, 1282, 1268, 1254, 1241, 1229, 1216, 1205, 1193, 1182, 1171, 1160, 1150, 1140, 1130, 1121, 1111, 1102, 1093, 1085, 1076, 1068, 1060, 1052, 1044, 1037, 1030, 1022, 1015, 1008, 1002, 995, 988, 982, 976, 970, 963, 958, 952, 946, 940, 935, 929, 924, 919, 914, 909, 904, 899, 894, 889, 885, 880, 875, 871, 867, 862, 858, 854, 850, 846, 842, 838, 834, 830, 826, 822, 819, 815, 811, 808, 804, 801, 797, 794, 791, 788, 784, 781, 778, 775, 772, 769, 766, 763, 760, 757, 754, 751, 748, 745, 743, 740, 737, 735, 732, 729, 727, 724, 722, 719, 717, 714, 712, 709, 707, 705, 702, 700, 698, 695, 693, 691, 689, 687, 684, 682, 680, 678, 676, 674, 672, 670, 668, 666, 664, 662, 660, 658, 656, 654, 653, 651, 649, 647, 645, 643, 642, 640, 638, 636, 635, 633, 631, 630, 628, 626, 625, 623, 621, 620, 618, 617, 615, 614, 612, 611, 609, 607, 606, 605, 603, 602, 600, 599, 597, 596, 594, 593, 592, 590, 589, 588, 586, 585, 583, 582, 581, 580, 578, 577, 576, 574, 573, 572, 571, 569, 568, 567, 566, 565, 563, 562, 561, 560, 559, 557, 556, 555, 554, 553, 552, 551, 549, 548, 547, 546, 545, 544, 543, 542, 541, 540, 539, 538, 537, 536, 535, 534, 533, 532, 531, 530, 529, 528, 527, 526, 525, 524, 523, 522, 521, 520, 519, 518, 517, 516, 515, 514, 513, 513, 512, 511, 510, 509, 508, 507, 506, 505, 505, 504, 503, 502, 501, 500, 500,};
 
 static int current_level = 0;
 static int input_pos = 0;		// bemenet --> majd áttenni 32 bitesre õket 
 static int current_pos = 0;
 static int pos_diff = 0;

 static uint8_t direction = 0;
 static uint8_t motor_enable = 0;
			
 // private 

 static void motor_update_timer(void)
 {
	// motor sebedseg szabalyzas
	
	if(motor_enable && direction)  
	{
		pos_diff = (input_pos - current_pos)-1;		// -1 hogy ne lépjen 1 el többet ! 

		if ((pos_diff > current_level) && (current_level < ARRAY_ELEMENT_1))	  current_level++;
		else if((pos_diff < current_level) && (current_level > 0))	 current_level--;

		current_pos++;
		set_timer_1_ovf_value(time_levels[current_level]);	
	}
	else if(motor_enable && (!direction))
	{
		pos_diff = (current_pos - input_pos)-1 ;	// -1 hogy ne lépjen 1 el többet !	

		if ((pos_diff > current_level) && (current_level < ARRAY_ELEMENT_1))	  current_level++;
		else if((pos_diff < current_level) && (current_level > 0))	 current_level--;

		current_pos--;	   
		set_timer_1_ovf_value(time_levels[current_level]);	  // timer komparálási ovf érték --> frek frisstés
	} 

	if ((!current_level) && motor_enable)			// ha lenullázódott a sebesség  
	{
	motor_enable = 0;								// motor tiltása -> main ból lehet újra indítani 
	} 
 }

 void motor_1_init(void)
 {
	timer_1_init(); 												// timer beállítása  

	set_timer_int_Callback(TIMER1,motor_update_timer);				// set interrupt callback 

	PORT_Init(&MOTOR_1_PULSE_DIR,MOTOR_1_PULSE_PIN, OUTPUT);		// timer output --> motor pulse
	PORT_Init(&MOTOR_1_DIR_DIR,MOTOR_1_DIR_PIN, OUTPUT); 			// direction láb 

 }
 void motor_1_main(uint16_t dmx_pos_1)
 {
	uint16_t tim_state;									// timer jelenlegi állása 
	uint16_t motor_enable_buf;							// timer-enable, szinkronizálás --> bufferelés

	input_pos = dmx_pos_1;								// beolvasott pozíció 
	motor_enable_buf = motor_enable;
	tim_state = get_timer_1_value();					// kiolvasni a timer értékét 

	if((!motor_enable_buf) && (input_pos != current_pos) && ( tim_state > TIM_COMPARE) && ( tim_state < time_levels[0]))	// áll a motor + menni kéne + timer kimenete = 0;
	{	
		//motor indítás + irány meghatározás																												// < time_level[0] --> ovf interrupt után még  
		if(input_pos>current_pos)	direction = 1;		// pozitiv  // irány meghatározás
		else 						direction = 0;		// negatív

		motor_enable = 1;											// engedélyezés 
		PORT_Write(&MOTOR_1_DIR_PORT,MOTOR_1_DIR_PIN,direction);	// direction pin állítás
		enable_timer_1_out();										// PWM kimenet engedélyezése
				

	}else if((!motor_enable_buf) && ( tim_state > TIM_COMPARE) && ( tim_state < time_levels[0]) )		
	{	  
		// timer kimenet letiltása --> motor stop 
		disable_timer_1_out();									// PWM leválasztása					
		PORT_Write(&MOTOR_1_PULSE_PORT,MOTOR_1_PULSE_PIN,0);	// kimenet 0 ra állítása
	}
 }


 