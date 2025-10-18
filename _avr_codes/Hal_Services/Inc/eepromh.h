/*
 * eepromh.h
 *
 * Created: 2025. 10. 12. 12:08:00
 *  Author: bagob
 */ 


#ifndef EEPROMH_H_
#define EEPROMH_H_
	#include "avr/io.h"

	void eeprom_write_byte(unsigned int uiAddress, uint8_t ucData);
	uint8_t eeprom_read_byte(unsigned int uiAddress);



#endif /* EEPROMH_H_ */