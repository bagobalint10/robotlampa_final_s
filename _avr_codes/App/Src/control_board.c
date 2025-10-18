/*
 * control_board.c
 *
 * Created: 2025. 10. 10. 16:52:46
 *  Author: bagob
 */ 

 #include "control_board.h"

 #include "buttons.h"
 #include "timer.h"
 #include "lcd_driver.h"
 #include "eepromh.h"
 #include "port_config.h"
 #include "relay.h"

 //glob�lis v�ltoz�k
 uint8_t *dmx_adress_pointer;
 uint8_t dmx_array[512];
 //

 static int dmx_adress = 0x01;

  // gomb v�ltoz�k 

  static uint8_t bt_up = 1;
  static uint8_t bt_down = 1;
  static uint8_t bt_enter = 1;
  static uint8_t bt_mode = 1;

  //lcd v�ltoz�k

  static uint8_t lcd_buffer[4] = "abcd";
  static uint8_t lcd_dot_buffer[4] = {0,0,0,0};
  static uint8_t lcd_enable = 1;

static int menu_n = 1;				// 0-3 f�men�
static uint8_t sub_menu_f = 0;
static int sub_menu_n = 0;

static uint8_t dmx_menu_blink = 0;
/////
static uint8_t lamp_cold_f = 0;  // 0-nem --> heat = lamp_wait , 1 igen, (szabad e ind�tani) 
static uint8_t lamp_count = 0;


 void push_string(void)										//--------------------------
 {

 static const char *menu_string[3]=	{	"rst ",
										"a   ",
										"lamp" };
			
 static const char *sub_menu_string[3]=	{	"  on",
											" off",
											"    "};

 const char *menu_pointer;	// string_push hoz

	if(sub_menu_f) menu_pointer = sub_menu_string[sub_menu_n];  		// SUBMENU	KIJELZ�S  
	else menu_pointer = menu_string[menu_n];							// sima MENU KIJELZ�S
	if(menu_n == 1) menu_pointer = menu_string[1];						// dmx m�dba fix!!
	if((menu_n == 1) && dmx_menu_blink) menu_pointer = sub_menu_string[2];	// dmx m�dba VILLOG (�res)

	for (int i = 0; i<4; i++)							//str_copy
	{
		lcd_buffer[i] = *(menu_pointer+i);
	}

	if(menu_n == 1)										// dmx adress kijelz�se 
	{
		 // adress convert string
		 char s_adress[3];

		 s_adress[2] = (dmx_adress%10)+48;	// egyes helyiertek
		 s_adress[1] = (dmx_adress/10%10)+48;
		 s_adress[0] = (dmx_adress/100%10)+48;
		 //adresss kijelz�s 					
		 for (int i = 1; i<4; i++)							// str_copy 1-3
		 {
			 lcd_buffer[i] = s_adress[i-1];
		 }
	}
	 
 }
 static void menu(void)
 {
 
	static uint32_t current_time = 0;
	current_time = millis();
	static uint32_t prev_time_long = 0;
	static uint16_t interval_long = 1000;		// hossz� gombnyom�s ideje 
	
	static uint32_t prev_time_counter = 0;
	static uint16_t interval_counter = 50;  	// gyors l�ptet�s sebess�ge (ideje)

	static uint32_t prev_time_blink = 0;
	static uint16_t interval_blink = 350;  		// A betu villog�s ideje 

	static uint32_t prev_time_save = 0;
	static uint16_t interval_save = 100;  		// ment�sn�l villog�s ideje 

	static uint32_t prev_time_heat_blink = 0;
	static uint16_t interval_heat_blink = 100;  // heat dots animacio

	static uint32_t prev_time_lamp_cold = 0;
	static uint32_t interval_lamp_cold = 300000;  // kih�l�si id� 5 perc  5*60 *1000


	static uint8_t heat_dots = 0x02;
	static uint8_t heat_dots_dir = 0x01; 

	// felfut� �l olvas�s
	static uint8_t bt_up_tmp = 1;
	static uint8_t bt_down_tmp = 1;
	static uint8_t bt_enter_tmp = 1;
	static uint8_t bt_mode_tmp = 1;
	//
	uint8_t enter_f = 0; // dinamikus, gomb flagek --> egy ciklus �let�
	uint8_t mode_f = 0;
	uint8_t up_f = 0;
	uint8_t down_f = 0;
	//
	uint8_t up_long_f = 0;
	uint8_t down_long_f = 0;
	uint8_t enter_long_f = 0;
	//
	static uint8_t save_f = 0;
	static uint8_t save_counter = 0;
	static uint8_t save_once = 0;
	static uint8_t clear_once = 0;
	static uint8_t lamp_on_f = 0;
	
	

	//---------> �ldetekt�l�s
	if(bt_up^bt_up_tmp)	 // �l detekt�l�s
	{
		up_f = !bt_up;		// l�ptet�s, lefut� �ln�k
		bt_up_tmp = bt_up;
	}
	if(bt_down^bt_down_tmp)	 // �l detekt�l�s
	{
		down_f = !bt_down;	// l�ptet�s, lefut� �ln�k
		bt_down_tmp = bt_down;
	}
	if(bt_enter^bt_enter_tmp)	 // �l detekt�l�s
	{
		enter_f = !bt_enter;		 // csak lefut�
		bt_enter_tmp = bt_enter;
	}
	if(bt_mode^bt_mode_tmp)	 // �l detekt�l�s
	{
		mode_f = !bt_mode;	 // csak lefut�
		bt_mode_tmp = bt_mode;
	}
	
	// hosszu gombnyom�s ellen�rz�sek: enter, up, down

	if((bt_enter+bt_down+bt_up) == 2) // ha 1 gomb van csak lenyomva
	{ 
		if (!bt_enter)	// nyomva van 
		{
			if(enter_f) prev_time_long = current_time;	// felfuto el --> reset timer
			if ((uint32_t)(current_time - prev_time_long)>= interval_long) enter_long_f = 1; // letelt az ido --> set flag 
		}
		else if (!bt_up)	// nyomva van
		{
			if(up_f) prev_time_long = current_time;	// felfuto el --> reset timer
			if ((uint32_t)(current_time - prev_time_long)>= interval_long) up_long_f = 1; // letelt az ido --> set flag
		}
		else if (!bt_down)	// nyomva van
		{
			if(down_f) prev_time_long = current_time;	// felfuto el --> reset timer
			if ((uint32_t)(current_time - prev_time_long)>= interval_long) down_long_f = 1; // letelt az ido --> set flag
		}
	}
	else
	{
	enter_long_f = 0;	 //  gomb elengedve --> reset flag
	up_long_f = 0;
	down_long_f = 0;
	} 
	
	//---------> �ldetekt�l�s eddig

	// men� + submen� switch --> hosszu --> belsej�t f�ggv�nyezni 
	switch(menu_n)	// --> ha "r�l�ptem" az adott men�pontra 
	{
	case 0:		// reset 
				// reset();
				// kil�p�s --> reset �ll�tja a flaget 
				break;

	case 1:		// DMX ADRESS
				if(sub_menu_f)  				// SUBMENU 
				{
												// ENTER HOSSZAN  --> ment�s
					if(mode_f) sub_menu_f = 0;	// MODE	 --> kil�p�s --> men�
					if(up_f) dmx_adress++;		// UP-DOWN -->  dmx adress
					if(down_f) dmx_adress--;
					// A bet� villogtat�s
					if ((uint32_t)(current_time - prev_time_blink)>= interval_blink)
					{
						prev_time_blink = current_time;
						dmx_menu_blink ^= 0x01;
					}
					// gyors l�ptet�s

					if (up_long_f) //gyors l�ptet�s up 
					{
						if ((uint32_t)(current_time - prev_time_counter)>= interval_counter)
						{
							prev_time_counter = current_time;
							dmx_adress++;
						}
					}
					if (down_long_f) //gyors l�ptet�s down
					{
						if ((uint32_t)(current_time - prev_time_counter)>= interval_counter)
						{
							prev_time_counter = current_time;
							dmx_adress--;
						}
					}	    
				}
				else							// SIMA MENU		
				{
												// MODE	 --> semmi
					if(enter_f) sub_menu_f = 1;	// ENTER  --> bel�p�s --> submen�
					if(up_f) menu_n--;			// UP-DOWN -->  menu_n
					if(down_f) menu_n++;

					dmx_menu_blink = 0;
					
				} 
				break;

	case 2:		// LAMP
	
				if(sub_menu_f)  				// SUBMENU
				{
												// ENTER  --> ment�s, hoszan
					if(mode_f) sub_menu_f = 0;	// MODE	 --> kil�p�s --> men�
					if(up_f) sub_menu_n--;		// UP-DOWN -->  sub_menu_n,
					if(down_f) sub_menu_n++;
				}
				else							// SIMA MENU
				{				
												// MODE	 --> semmi
					if(enter_f) sub_menu_f = 1;	// ENTER  --> bel�p�s --> submen�
					if(up_f) menu_n--;			// UP-DOWN -->  menu_n
					if(down_f) menu_n++;
				}
				break;
	default: break;
	}

	// Overflow guard
	if(menu_n < 1) menu_n = 1;
	if(menu_n > 2) menu_n = 2;

	if(sub_menu_n < 0) sub_menu_n = 0;
	if(sub_menu_n > 1) sub_menu_n = 1;

	// + ADRESS OVERFLOF GUARD
	if(dmx_adress < 1) dmx_adress = 400;
	if(dmx_adress > 400) dmx_adress = 1;

	push_string();

	//save_enter

	if (sub_menu_f && enter_long_f)	   // submen�be van + enter long lenyomva  --> set save_f
	{ 
		save_f = 1;
	}

	if(save_f && !save_once)  // 1x fut le 
	{
		save_once = 1;
		

		// rel� set 
		// rel� reset 
		if(menu_n == 2)			// l�mpa save
		{
			if(sub_menu_n == 0) lamp_on_f = 1;		//on 
			else if (sub_menu_n == 1) lamp_on_f = 0;//off

		}else if(menu_n == 1)	//adress save 
		{
			eeprom_write_byte(EEPR_ADR_DMX_ARD_0, (uint8_t)dmx_adress);				// csak dmx adress ment�se
			eeprom_write_byte(EEPR_ADR_DMX_ADR_1, (uint8_t)(dmx_adress >> 8));		// lamp ment�s majd be kapcsol�sn�l

			// DMX ADRESST ITT LEHET GLOB�LIS V�LTOZ�BA BET�LTENI !!! 
			dmx_adress_pointer = (dmx_array+(dmx_adress-1)+2);
		}

	}
	if (save_f && (save_counter < 10))	  // save counter csak 5x engedi lefutni
	{
		// villogtasson 5 x, 6dikn�l -> save_blink = 0
		if ((uint32_t)(current_time - prev_time_save)>= interval_save)
		{
			prev_time_save = current_time;
			lcd_enable ^= 0x01;
			save_counter++;
		}
	}else lcd_enable = 1;

	if (bt_enter)	// ha az enter felvan engedve 
	{
		//reset flags
		save_counter = 0; 
		save_f = 0;
		save_once = 0;
	}
	//// save enter eddig 

	// heat animation

	// if heat --> ide-oda fut
	//if (heat)	  // save counter csak 5x engedi lefutni
	if(lamp_on_f && lamp_count)
	{
		// villogtasson 5 x, 6dikn�l -> save_blink = 0
		if ((uint32_t)(current_time - prev_time_heat_blink)>= interval_heat_blink)
		{
			prev_time_heat_blink = current_time;
			if(heat_dots==0x01 || heat_dots == 0x08) heat_dots_dir ^= 0x01;
			if(heat_dots_dir) heat_dots = (heat_dots<<1);
			else heat_dots = (heat_dots>>1);
			
		}
		for (int i = 0; i<4;i++)
		{
			lcd_dot_buffer[i] = (heat_dots&(0x01<<i))&&0x01;
		}
		clear_once = 1;
	}else if(clear_once)   //clear all flag; 
	{
		clear_once = 0;
		for (int i = 0; i<4;i++)
		{
			lcd_dot_buffer[i] = 0x00;
		}
	}



	/// rel� id�,eeprom, kiirat�s kezel�s
	if (lamp_on_f && lamp_cold_f)	  // bekapcsolva �s kih�lt  
	{
		lamp_cold_f = 0;	// set 
		relay_set();		// set 
		eeprom_write_byte(EEPR_ADR_LAMP, lamp_cold_f);

		for (int i = 0; i<4;i++)
		{
			lcd_dot_buffer[i] = 0x01;
		}

	}else if (!lamp_on_f && !lamp_cold_f && !lamp_count)  // kikapcsoljuk �s m�g meleg,
	{ 
		relay_reset(); // ezt csa egyszer k�ne 
		// sz�ml�l�s ind�t�sa  	 --> ha letelt set lamp_cold f
		prev_time_lamp_cold = current_time;
		lamp_count = 1;
		for (int i = 0; i<4;i++)
		{
			lcd_dot_buffer[i] = 0x00;
		}
	}

	if (lamp_count) // l�mpa id�z�t�s 
	{
		if ((uint32_t)(current_time - prev_time_lamp_cold)>= interval_lamp_cold)
		{
			// reset lamp count 
			// eeprom reset 
			// lamp cold 
			lamp_cold_f = 1;
			eeprom_write_byte(EEPR_ADR_LAMP, lamp_cold_f);
			lamp_count = 0;

			
		}
	}

	//
	  
	//
 }

 // public f�ggv�nyek 

 void control_board_init(void)
 {
	buttons_init(10); //debounce time in ms
	set_buttons_variables(&bt_up, &bt_down, &bt_enter, &bt_mode);
	lcd_init(4);	// 4x4 = 16ms refresh time 
	relay_init();

	// eeprom kiolvas�s
	lamp_cold_f = eeprom_read_byte(EEPR_ADR_LAMP);
	dmx_adress = eeprom_read_byte(EEPR_ADR_DMX_ARD_0);
	dmx_adress |= (eeprom_read_byte(EEPR_ADR_DMX_ADR_1)<<8);

	dmx_adress_pointer = (dmx_array+(dmx_adress-1)+2);
	// T�R�LD KI !!!! 
	dmx_adress_pointer = dmx_array; // FIX �RT�KRE �LL�T�S IDEIGLENESEN !!


 }

 void control_board_main(void) // ideiglenes sketchi verzio
 {
	button_read(); // kiolvas�s
	menu();	 // gomb -->  string , dot, enable
	lcd_write_buffer(lcd_buffer,lcd_dot_buffer,lcd_enable);	
 }
   


