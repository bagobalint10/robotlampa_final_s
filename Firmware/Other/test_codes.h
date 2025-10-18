/*
 * test_codes.h
 *
 * Created: 2025. 10. 10. 17:34:17
 *  Author: bagob
 */ 

#ifndef TEST_CODES_H_
#define TEST_CODES_H_
#endif

		// ne includáld sehova!!!! csak egy jegyzet
		// ha kell egy függvény másold ki és illeszd be !!!


 ////// 1. CONTROL_BOARD modul 

 static void lcd_button_test_code(void)
 {
	// ideiglenes kijelzõ gomb test code

	#define TEST_BUTTONS valtozo_neve
	 
	segment_write_digit(0x01,(uint8_t) (((TEST_BUTTONS&0x01) && 0x01)+48), 0, 1);
	_delay_ms(1);
	segment_write_digit(0x02,(uint8_t) (((TEST_BUTTONS&0x02) && 0x01)+48), 0, 1);
	_delay_ms(1);
	segment_write_digit(0x04,(uint8_t) (((TEST_BUTTONS&0x04) && 0x01)+48), 0, 1);
	_delay_ms(1);
	segment_write_digit(0x08,(uint8_t) (((TEST_BUTTONS&0x08) && 0x01)+48), 0, 1);
	_delay_ms(1);
 }

 static void lcd_abcd_test_code(void)
 {
	// ideiglenes kijelzõ abcd  test code
	  
	segment_write_digit(0x01,'d', 0, 1);
	_delay_ms(1);
	segment_write_digit(0x02,'c', 0, 1);
	_delay_ms(1);
	segment_write_digit(0x04,'b', 0, 1);
	_delay_ms(1);
	segment_write_digit(0x08,'a', 0, 1);
	_delay_ms(1);
 }


 // timer millis minta kód 
	
	static uint32_t tmp_current_time = 0;
	tmp_current_time = millis();

	static uint32_t tmp_time_0 = 0;
	static uint16_t tmp_interval_0 = 1000;

	if ((uint32_t)(tmp_current_time - tmp_time_0)>= tmp_interval_0)
	{
		tmp_time_0 = tmp_current_time;
		villog ^= 0x01;
	} 


			




