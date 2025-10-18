/*
 * eeprom.c
 *
 * Created: 2025. 10. 12. 12:08:14
 *  Author: bagob
 */ 

 #include "eepromh.h"
 //#include <avr/interrupt.h>


 void eeprom_write_byte(unsigned int uiAddress, uint8_t ucData)
 {
	 /*while(EECR & (1<<EEPE));   // el�z� �r�s v�r�sa

	 cli();
	 EEAR = uiAddress;	  // adat + c�m megad�sa 
	 EEDR = ucData;
	 
	 EECR |= (1<<EEMPE);  // adat be�r�sa --> eeprom
	 EECR |= (1<<EEPE);	 
	 sei();*/
 }


 uint8_t eeprom_read_byte(unsigned int uiAddress)
 {
	 /*while(EECR & (1<<EEPE));  // el�z� �r�s v�r�sa
	 
	 EEAR = uiAddress;	// c�m megad�sa
	 EECR |= (1<<EERE);	  // adat kiolvas�sa ,  eeprom   --> EEDR
	 
	 return EEDR;*/
	 return 0;
 }



 /*

 EEPROM_write(0x01, rev_p);
 EEPROM_write(0x02, rev_t);
 EEPROM_write(0x03, bit_mode);
 
 EEPROM_write(0x04, dmx_adress);
 EEPROM_write(0x05, dmx_adress >> 8);



 lamp_time_flag = EEPROM_read(0x00);  // letelt e az 5 p
 rev_p = EEPROM_read(0x01);
 rev_t = EEPROM_read(0x02);
 bit_mode = EEPROM_read(0x03);
 dmx_adress = EEPROM_read(0x04);
 dmx_adress |= EEPROM_read(0x05) <<8;


 */
