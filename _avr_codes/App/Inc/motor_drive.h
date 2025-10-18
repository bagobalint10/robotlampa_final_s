/*
 * motor_drive.h
 *
 * Created: 2025. 10. 14. 16:32:55
 *  Author: bagob
 */ 


#ifndef MOTOR_DRIVE_H_
#define MOTOR_DRIVE_H_

	#include <avr/io.h>

	void motor_1_init(void);
	void motor_1_main(uint16_t dmx_pos_1);



#endif /* MOTOR_DRIVE_H_ */