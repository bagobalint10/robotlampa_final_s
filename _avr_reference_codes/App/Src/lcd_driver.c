/*
 * lcd_driver.c
 *
 * Created: 2025. 10. 11. 0:14:24
 *  Author: bagob
 */ 

 #include "lcd_driver.h"

 #include "segment_write_digit.h"
 #include "timer.h"

 static uint8_t interval_time = 0;

// private 


// public 
 void lcd_init(uint8_t mux_time)
 {
	segment_Init();	
	interval_time = mux_time;
 }

 void lcd_write_buffer(uint8_t *lcd_buffer,uint8_t *lcd_dot_buffer,uint8_t lcd_enable) 	 // 4 byteos buffer kiiratása (részletenként) --> intervallumonként 
 {  
	static uint32_t prev_time = 0;
	static uint8_t n = 0;

	uint32_t current_time = millis();	

	if ((uint32_t)(current_time - prev_time)>= interval_time)  // idõzítés
	{
		prev_time = current_time;
		segment_write_digit((0x01 << n),*((lcd_buffer+3)-n),*((lcd_dot_buffer+3)-n), lcd_enable); // digit sorrend invertálás 

		n++;
		if (n >= 4) n = 0;
	}
 }