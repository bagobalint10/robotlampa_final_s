/*
 * interrupt.h
 *
 * Created: 2025. 10. 09. 16:56:21
 *  Author: bagob
 */ 

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

	#include "stm32f4xx_hal.h"
	#include "main.h"

	void set_pcint_Callback(uint8_t port ,void (*Callback_function)(uint16_t) );
	void set_timer_int_Callback(uint8_t timer ,void (*Callback_function)(void) );
	void set_usart_int_Callback(uint8_t type ,void (*Callback_function)(void) );
	
	void ISR_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* INTERRUPT_H_ */
