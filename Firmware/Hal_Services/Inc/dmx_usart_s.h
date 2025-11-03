/*
 * dmx_usart.h
 *
 * Created: 2025. 10. 12. 19:37:33
 *  Author: bagob
 */ 


#ifndef DMX_USART_H_
#define DMX_USART_H_

	#include "stm32f4xx_hal.h"
	
	void dmx_usart_send(void);
	void usart_rx_fe_callback(void);
	void usart_rx_callback(uint8_t data);

	extern uint8_t *dmx_adress_pointer;
	extern uint8_t dmx_array[512];

#endif /* DMX_USART_H_ */
