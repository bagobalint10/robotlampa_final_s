/*
 * motor_drive.h
 *
 * Created: 2025. 10. 14. 16:32:55
 *  Author: bagob
 */ 


#ifndef MOTOR_DRIVE_H_
#define MOTOR_DRIVE_H_

	#include "stm32f4xx_hal.h"


	void motor_1_main(uint16_t dmx_pos_1, uint8_t dmx_speed);
	void motor_update_timer(void);



#endif /* MOTOR_DRIVE_H_ */
