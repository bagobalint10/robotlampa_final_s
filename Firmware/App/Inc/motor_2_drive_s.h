/*
 * motor_2_drive.h
 *
 *  Created on: Oct 23, 2025
 *      Author: bagob
 */

#ifndef APP_INC_MOTOR_2_DRIVE_S_H_
#define APP_INC_MOTOR_2_DRIVE_S_H_

	#include "stm32f4xx_hal.h"

	void motor_2_main(uint16_t dmx_pos_1, uint8_t dmx_speed);
	void motor_2_update_timer(void);
	void motor_2_set_0_pos(void);

#endif /* APP_INC_MOTOR_2_DRIVE_S_H_ */
