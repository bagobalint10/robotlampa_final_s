/*
 * eepromh.h
 *
 * Created: 2025. 10. 12. 12:08:00
 *  Author: bagob
 */ 


#ifndef EEPROMH_H_
#define EEPROMH_H_
	#include "stm32f4xx_hal.h"

	void eeprom_write_byte(unsigned int uiAddress, uint8_t ucData);
	uint32_t eeprom_read(void);

#endif /* EEPROMH_H_ */
