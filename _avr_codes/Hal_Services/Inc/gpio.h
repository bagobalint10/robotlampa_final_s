/*
 * gpio.h
 *
 * Created: 2025. 10. 07. 19:19:36
 *  Author: bagob
 */ 


#ifndef GPIO_H_
#define GPIO_H_

	#include <avr/io.h>

	void PORT_Init(volatile uint8_t *dir, uint8_t pin, uint8_t state );
	void PORT_Write(volatile uint8_t *port, uint8_t pin, uint8_t state );
	void PORT_Toggle(volatile uint8_t *port, uint8_t pin);
	uint8_t PORT_Read(volatile uint8_t *port);	  // teljes port ot olvas be 

#endif /* GPIO_H_ */