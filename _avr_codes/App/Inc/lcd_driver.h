/*
 * lcd_driver.h
 *
 * Created: 2025. 10. 11. 0:14:10
 *  Author: bagob
 */ 


#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

	#include <avr/io.h>

	void lcd_write_buffer(uint8_t *lcd_buffer,uint8_t *lcd_dot_buffer,uint8_t lcd_enable); 
	void lcd_init(uint8_t mux_time);


#endif /* LCD_DRIVER_H_ */