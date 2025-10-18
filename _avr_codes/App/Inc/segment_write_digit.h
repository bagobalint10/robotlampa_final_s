/*
 * segment_write_digit.h
 *
 * Created: 2025. 10. 08. 0:09:46
 *  Author: bagob
 */ 


#ifndef SEGMENT_WRITE_DIGIT_H_
#define SEGMENT_WRITE_DIGIT_H_

 	#include <avr/io.h>

 	void segment_write_digit(uint8_t digit, uint8_t kar, uint8_t dot, uint8_t enable);
 	void segment_Init(void);



#endif /* SEGMENT_WRITE_DIGIT_H_ */