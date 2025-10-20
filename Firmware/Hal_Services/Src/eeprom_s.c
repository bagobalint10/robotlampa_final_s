/*
 * eeprom.c
 *
 * Created: 2025. 10. 12. 12:08:14
 *  Author: bagob
 */ 

 #include <eepromh_s.h>

 //static uint32_t data = 0;

 //#define FLASH_START_ADRESS  0x08020000U   // flash sector 5

 void eeprom_write_byte(unsigned int uiAddress, uint8_t ucData)
 {

/*
	 data = (data | ((uint32_t)ucData<<(uiAddress*8)));

	 HAL_FLASH_Unlock();

	 FLASH_Erase_Sector(FLASH_SECTOR_5, VOLTAGE_RANGE_3);
	 HAL_FLASH_Program(TYPEPROGRAM_WORD, FLASH_START_ADRESS , data); // 4 byte írása

	 HAL_FLASH_Lock();*/
 }


 uint32_t eeprom_read(void)
 {
/*
	 data = *(uint32_t*)FLASH_START_ADRESS;
	 return data;
*/
	 return 0;
 }


