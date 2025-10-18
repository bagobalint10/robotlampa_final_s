/*
 * segment_write_digit.c
 *
 * Created: 2025. 10. 08. 0:10:32
 *  Author: bagob
 */ 
 
  
#include "segment_write_digit.h"
#include "gpio.h"
#include "port_config.h"


// privát functions S
 
 static uint8_t bit_functions(uint8_t char_i ,uint8_t dot, uint8_t enable)
 {
	 static const uint8_t character_table[50] = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11100110, 0b11101110, 0b00111110, 0b10011100, 0b01111010, 0b10011110, 0b10001110, 0b10111100, 0b00101110, 0b01100000, 0b01110000, 0b10101110, 0b00011100, 0b11101100, 0b00101010, 0b00111010, 0b11001110, 0b11100110, 0b00001010, 0b10110110, 0b00011110, 0b01111100, 0b00111000, 0b00111000, 0b01101110, 0b01100110, 0b11011010 };// 0-9 ig számok 10- karakterek
	 static const uint8_t dot_mask = 0x01;  // pont bitkép
	 uint8_t char_pixel = 0;

	 if(enable == 0)				// enable == 0  --> kijelzõ törlése
	 {
		 char_i = 40;
		 char_pixel = character_table[char_i];	// ' '
	 }
	 else							// enable == 1  --> normál kiiratás
	 {
		 char_pixel = character_table[char_i];
		 // karakter_hozzárendelés a pixel képekhez
		 if (dot)		// dot == 1 --> pont maszkolás
		 {
			 char_pixel |= (dot_mask);  // karakter + pont
		 }
	 }

	 return char_pixel;
 }

 static void shift_out(uint8_t sr_data) // adat kishiftelés --> csak data + clock kezelés
 {
	 for (int i = 0; i < 8 ; i++){ // 8 bit kishiftekése

		 if(~(sr_data) & (0x01 << i)) //2. kishiftelés
		 {
			 PORT_Write(&SR_DATA_PORT, SR_DATA_PIN, 1); // data pin HIGH
		 }
		 else
		 {
			 PORT_Write(&SR_DATA_PORT, SR_DATA_PIN, 0);	// data pin LOW
		 }

		 PORT_Write(&SR_CLOCK_PORT, SR_CLOCK_PIN, 1); // clock pulse
		 PORT_Write(&SR_CLOCK_PORT, SR_CLOCK_PIN, 0);
	 }
 }

 static uint8_t char_convert(uint8_t char_cnv) // ascii --> karaktertömb_tömbindex
 {
	 if(char_cnv == ' ')  // space
	 {
		 char_cnv =  40;

	 }else if(char_cnv >= 97 && char_cnv <= 122)	// a-z
	 {
		 char_cnv -= 87;

	 }else if(char_cnv >= 48 && char_cnv <= 57) // 0-9
	 {
		 char_cnv -= 48;
	 }else						// default: space
	 {
		 char_cnv = 40;
	 }

	 return char_cnv;
 }


// public functions
 
void segment_Init(void)
{
	PORT_Init(&SR_LATCH_DIR, SR_LATCH_PIN, OUTPUT);
	PORT_Init(&SR_CLOCK_DIR, SR_CLOCK_PIN, OUTPUT);
	PORT_Init(&SR_DATA_DIR, SR_DATA_PIN, OUTPUT);
}


void segment_write_digit(uint8_t digit, uint8_t character, uint8_t dot, uint8_t enable)  //ez kezeli a multiplexelést is ? diregtbe állítja a portokat?
{	

	uint8_t char_pixels = 0;

	character = char_convert(character);   // 1. ez egy karakter konverzió asci --> szimbólumtömb indexe 	
	char_pixels = bit_functions(character, dot, enable);	  // szimbólum index--> bitkép + engedélyezés + pont rámaszkolása

	//1. shift out   
		
	uint8_t shift_szam = 0;			 // shift register bit conversion
	uint8_t shift_digit = 0;
	shift_digit = ((digit & 0x08) >>1);    //0x04 be  
	shift_szam = ((char_pixels & 0x80) >>1) | ((char_pixels & 0x20) >>1) | ((char_pixels & 0x10) <<1) | ((char_pixels & 0x01) <<3) ; // pixel mapping conversion

	shift_out((shift_szam)|(shift_digit));
	
	//2. shift out 
 	
	shift_szam = 0;
	shift_szam = ((char_pixels & 0x40) >> 1) | ((char_pixels & 0x08) << 1) | ((char_pixels & 0x04) << 1) | ((char_pixels & 0x02) << 5) ;  // pixel mapping konverzió
	shift_digit = ((digit & 0x01) <<1) | ((digit & 0x02) <<1) | ((digit & 0x04) >>2)  ;

	shift_out((shift_szam)|(shift_digit));

	//Refresh outputs

	PORT_Write(&SR_LATCH_PORT, SR_LATCH_PIN, 1);  // latch pulse --> kimenet frissítése
	PORT_Write(&SR_LATCH_PORT, SR_LATCH_PIN, 0);		
}



