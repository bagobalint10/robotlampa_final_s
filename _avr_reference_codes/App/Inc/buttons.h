/*
 * buttons.h
 *
 * Created: 2025. 10. 10. 22:43:34
 *  Author: bagob
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

	#include <avr/io.h>

	void buttons_init(uint8_t debounce_time);
	void set_buttons_variables(uint8_t *button_0, uint8_t *button_1, uint8_t *button_2, uint8_t *button_3 );
	void button_read(void);





#endif /* BUTTONS_H_ */