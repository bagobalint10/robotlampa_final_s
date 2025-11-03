/*
 * buttons.c
 *
 * Created: 2025. 10. 10. 22:43:48
 *  Author: bagob
 */ 

 #include "buttons.h"

 //#include "port_config.h"
 #include "interrupt.h"
 //#include "gpio.h"
 #include "stddef.h"
 //#include "timer.h"


 static volatile uint8_t buttons = 0;	// direktbe beolvasott gomb �rt�kek
 static uint8_t db_time = 0;			// debounce time

 static uint8_t *button_0_p = NULL;
 static uint8_t *button_1_p = NULL;
 static uint8_t *button_2_p = NULL;
 static uint8_t *button_3_p = NULL;

 // private 

 static void pcint_c_callback(void)				// pcint_C lekezek�se
 {
//	buttons = PORT_Read(&BTN_COMMON_PIN_IN);	//C port beolvas�s	--> diregt beolvasott dolog
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
	db_time = debounce_time;

	// gomb pinek bemenet + pullup  - init
/*
	PORT_Init(&BTN_UP_DIR,BTN_UP_PIN,0);		// PC0 - input
	PORT_Write(&BTN_UP_PORT,BTN_UP_PIN,1);		// P0 - pull up

	PORT_Init(&BTN_DOWN_DIR,BTN_DOWN_PIN,0);	// PC1
	PORT_Write(&BTN_DOWN_PORT,BTN_DOWN_PIN,1);

	PORT_Init(&BTN_ENTER_DIR,BTN_ENTER_PIN,0); // PC2
	PORT_Write(&BTN_UP_PORT,BTN_ENTER_PIN,1);

	PORT_Init(&BTN_MODE_DIR,BTN_MODE_PIN,0);	// PC3
	PORT_Write(&BTN_MODE_PORT,BTN_MODE_PIN,1);*/

	// pcint  callback be�ll�t�sa gombokra

	set_pcint_Callback(PCINT_C, pcint_c_callback);		// Callback fgv. be�ll�t�sa
	//pcint_init(PCINT_C, 0b00001111);					// C portra enable , maszkol�s 4 gombra

//	buttons = PORT_Read(&BTN_COMMON_PIN_IN); // 0. olvas�s
	// hal_gpio
 }

 void button_read(void)
 {	  
	static uint32_t prev_time = 0;		// perg�s mentes�t�shez v�ltoz�k
	static uint16_t interval_time = 0;

	uint32_t current_time = millis();
	interval_time = db_time;	//devounce time be�ll�t�sa 

	if ((uint32_t)(current_time - prev_time)>= interval_time)  //perg�s mentes�t�s
	{
		prev_time = current_time;

		*button_0_p = ((buttons & 0x01) && 0x01);	//pointereknek gomb �rt�k�nek �tad�sa
		*button_1_p = ((buttons & 0x02) && 0x01);
		*button_2_p = ((buttons & 0x04) && 0x01);
		*button_3_p = ((buttons & 0x08) && 0x01);
		
		} 
 }
