/*
 * interrupt.h
 *
 * Created: 2025. 10. 09. 16:56:21
 *  Author: bagob
 */ 


#ifndef INTERRUPT_H_
#define INTERRUPT_H_

	#include <avr/io.h>
	#include <avr/interrupt.h>

	void set_pcint_Callback(uint8_t port ,void (*Callback_function)(void) );
	void pcint_init(uint8_t port, uint8_t mask);

	void set_timer_int_Callback(uint8_t timer ,void (*Callback_function)(void) );
	void timer_int_init(uint8_t timer, uint8_t interrupt_type);

	void set_usart_int_Callback(uint8_t type ,void (*Callback_function)(void) );
	



#endif /* INTERRUPT_H_ */