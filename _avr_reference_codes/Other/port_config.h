 /* port_config.h
 *
 * Created: 2025. 10. 07. 21:01:13
 *  Author: bagob
 */ 






#ifndef PORT_CONFIG_H_
#define PORT_CONFIG_H_

	#include <avr/io.h>

	// port config shift registerhez

	#define SR_LATCH_PORT PORTD		// --> D6
	#define SR_LATCH_PIN PORTD6
	#define SR_LATCH_DIR DDRD

	#define SR_DATA_PORT PORTD		// --> D7
	#define SR_DATA_PIN PORTD7
	#define SR_DATA_DIR DDRD

	#define SR_CLOCK_PORT PORTD		// --> D5
	#define SR_CLOCK_PIN PORTD5
	#define SR_CLOCK_DIR DDRD
	 /*	  pinout shift register 
  
	data	-		d0x80 -> d7
	clock -		d0x20 -> d5
	latch	- 		d0x40 -> d6
	*/

	// port config a gombokhoz 

	#define BTN_UP_PORT PORTC		// --> C0  
	#define BTN_UP_PIN PORTC0
	#define BTN_UP_DIR DDRC
	#define BTN_UP_PIN_IN PINC		// bemenet olvasáshoz 

	#define BTN_DOWN_PORT PORTC		// --> C1
	#define BTN_DOWN_PIN PORTC1
	#define BTN_DOWN_DIR DDRC
	#define BTN_DOWN_PIN_IN PINC		

	#define BTN_ENTER_PORT PORTC		// --> C2
	#define BTN_ENTER_PIN PORTC2
	#define BTN_ENTER_DIR DDRC
	#define BTN_ENTER_PIN_IN PINC		

	#define BTN_MODE_PORT PORTC		// --> C30
	#define BTN_MODE_PIN PORTC3
	#define BTN_MODE_DIR DDRC
	#define BTN_MODE_PIN_IN PINC
	
	#define BTN_COMMON_PIN_IN PINC // csak ha az összes gomb  egy porton van 	

	//
	/*
			gomb bekotes:	 (egy port ra a pinchange miatt majd)

							Up 		- 	A0 - PC0 - 0x01 	
							Down 	- 	A1 - PC1 - 0x02
							Enter	-	A2 - PC2 - 0x04 
							Mode 	- 	A3 - PC3 - 0x08
	*/
	//

	// interruptok 

	#define PCINT_B 0
	#define PCINT_C 1
	#define PCINT_D 2

	//pcint valid mask--> amiket használhatok 
	#define PCMSK0_VALID_MASK  0b00111111  // PB0–PB5, ha XTAL1/2-t nem használod
	#define PCMSK1_VALID_MASK  0b00111111  // PC0–PC5
	#define PCMSK2_VALID_MASK  0xFF        // PD0–PD7

	// timers

	#define TIMER0 0
	#define TIMER1 1
	#define TIMER2 2

	//	timer config paraméterek, van még mit fejleszteni- bõvíteni rajtuk --> v0

	// Timer0 módok (TCCR0A értékek)
	#define TIMER0_NORMAL        0x00						// WGM01:0 = 00
	#define TIMER0_PWM_PC        (1<<WGM00)					// Phase Correct PWM, WGM01:0 = 01
	#define TIMER0_CTC           (1<<WGM01)					// CTC, WGM01:0 = 10
	#define TIMER0_PWM_FAST      ((1<<WGM01)|(1<<WGM00))	// Fast PWM, WGM01:0 = 11

	// Prescaler beállítások (TCCR0B CS02:0 értékek)
	#define TIMER0_NO_CLK        0x00
	#define TIMER0_PRESCALE_1    (1<<CS00)
	#define TIMER0_PRESCALE_8    (1<<CS01)
	#define TIMER0_PRESCALE_64   ((1<<CS01)|(1<<CS00))
	#define TIMER0_PRESCALE_256  (1<<CS02)
	#define TIMER0_PRESCALE_1024 ((1<<CS02)|(1<<CS00))

	// Interrupt típusok
	#define TIMER0_INT_NONE      0
	#define TIMER0_INT_OVF       (1<<TOIE0)
	#define TIMER0_INT_COMP_A    (1<<OCIE0A)


	// logic

	#define OUTPUT 1
	#define INPUT 0

	// eeprom adresses
	#define EEPR_ADR_LAMP 0x01
	#define EEPR_ADR_DMX_ARD_0 0x02
	#define EEPR_ADR_DMX_ADR_1 0x03

	// relé 
	#define RELAY_PORT PORTB		// --> 9 LÁB -- PB1
	#define RELAY_PIN PORTB1
	#define RELAY_DIR DDRB

	// globan variables    

	 //globális változók
	 extern uint8_t *dmx_adress_pointer;
	 extern uint8_t dmx_array[512];

	 // dmx config változók 

	 #define DMX_RESET_TIME 15	
	 #define DMX_SEND_TIME 5
	 #define DMX_START_ADRESS 0
	 #define DMX_MAX_ADRESS 15

	 // motor speed control 

	 #define ARRAY_ELEMENT 300 
	 #define ARRAY_ELEMENT_1 300

	 #define MOTOR_1_DIR_PORT PORTC      // A5 - PC5
	 #define MOTOR_1_DIR_PIN PORTC5
	 #define MOTOR_1_DIR_DIR DDRC

	 #define MOTOR_1_PULSE_PORT PORTB     // A5 - PC5
	 #define MOTOR_1_PULSE_PIN PORTB1
	 #define MOTOR_1_PULSE_DIR DDRB

	 #define TIM_COMPARE 10
	 #define TIM_START_VALUE 1000 



	  
	 




#endif /* PORT_CONFIG_H_ */