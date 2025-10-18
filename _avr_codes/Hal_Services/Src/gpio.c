/*
 * gpio.c
 *
 * Created: 2025. 10. 07. 19:20:50
 *  Author: bagob
 */ 

 #include "gpio.h"

 void PORT_Init(volatile uint8_t *dir, uint8_t pin, uint8_t state ) {
	 
	 if (state)	 
	 *dir |= (1 << pin);
	 else
	 *dir &= ~(1 << pin);
 }

 void PORT_Write(volatile uint8_t *port, uint8_t pin, uint8_t state ) {

	 if (state)
	 *port |= (1 << pin);
	 else
	 *port &= ~(1 << pin);
 }

 void PORT_Toggle(volatile uint8_t *port, uint8_t pin) {

	 *port ^= (1 << pin);
 }

 uint8_t PORT_Read(volatile uint8_t *port){

	return *port;
 }
