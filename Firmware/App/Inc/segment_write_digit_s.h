/*
 * segment_write_digit.h
 *
 * Created: 2025. 10. 08. 0:09:46
 *  Author: bagob
 */ 


#ifndef SEGMENT_WRITE_DIGIT_H_
#define SEGMENT_WRITE_DIGIT_H_

 	#include "stm32f4xx_hal.h"
	#include "main.h"

 	void segment_write_digit(uint8_t digit, uint8_t kar, uint8_t dot, uint8_t enable);

#endif /* SEGMENT_WRITE_DIGIT_H_ */
