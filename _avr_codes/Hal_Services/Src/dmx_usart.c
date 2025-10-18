/*
 * dmx_usart.c
 *
 * Created: 2025. 10. 12. 19:37:49
 *  Author: bagob
 */ 

 #include "dmx_usart.h"
 #include "port_config.h"
 #include "interrupt.h"
 #include "timer.h"

 #define FOSC 16000000			// Clock Speed
 #define BAUD 250000
 #define MYUBRR FOSC/16/BAUD-1

 //volatile uint8_t dmx_adat[520];				// átírni a globálisra õket meg angolra 
 //volatile unsigned int dmx_receive_num = 0;

 #define USART_RX_INT 0
 #define USART_TX_INT 1

 /*
 extern uint8_t *dmx_adress_pointer;   --> globális változók 
 extern uint8_t tmp_dmx_array[512];
 */


 // private 

 static void usart_transmit_poll( unsigned char data )
 {									
	 while ( !( UCSR0A & (1<<UDRE0)) );		// ehelyett interrupt !!!! 
											
	 UDR0 = data;
 }


 static void usart_rx_callback(void)
 {
	// dmx olvasas 
	static unsigned char  data_overrun = 0;
	static unsigned int dmx_receive_num = 0;

	if (UCSR0A & 0x10 ) // frame error
	{
		dmx_receive_num=0;
		data_overrun = 0;
	}

	if(UCSR0A & 0x08) //data overrun
	{
		data_overrun = 1;
	}
	
	volatile unsigned char tmp_UDR0 = UDR0;
	
	if (!data_overrun)
	{
		dmx_array[dmx_receive_num] = tmp_UDR0;
		if(dmx_receive_num < 512)dmx_receive_num ++;  // tömb túlcímzés védelem 
	}
 }


 // public 
 void dmx_usart_init(void)
 {
		// usart felkonfigurálása
	  	UBRR0H = (uint8_t)((MYUBRR)>>8);				// set baud rate 
	  	UBRR0L = (uint8_t)(MYUBRR);
	  													//Enable receiver and transmitter
	  	UCSR0B = (1<<RXEN0)|(1<<TXEN0)| (1<<RXCIE0);	// read interrupt enable tx interrupt enable : TXCIE0
	  	
	  	UCSR0C = (1<<USBS0)|(3<<UCSZ00) ;				// 8 data, 2 stop, no parity ??????

		// interrupt isr callback felkonfigurálása
		set_usart_int_Callback(USART_RX_INT,usart_rx_callback);
 }

 void dmx_usart_send(void)
 {
	// minden ciklusban lefut 
	// idõalapon küldi tovább a jelet 
	// saját bufferbõl dolgozik 

	static uint32_t current_time = 0;
	static uint32_t time_dmx_send = 0;
	static uint16_t interval_dmx_send_0 = DMX_SEND_TIME;	
	//static uint16_t interval_dmx_send_1 = DMX_RESET_TIME;
		
	static uint8_t dmx_send_buffer[16];	
	static uint8_t send_n = 0;

	current_time = millis();

	if ((uint32_t)(current_time - time_dmx_send)>= interval_dmx_send_0)		//fix 10ms enként lefut 
	{
		time_dmx_send = current_time;
		usart_transmit_poll(dmx_send_buffer[send_n]);	
		send_n++;
		interval_dmx_send_0 = DMX_SEND_TIME;	// reset idõ 

		if(send_n > DMX_MAX_ADRESS) // max value ig küldjük 
		{
			send_n = DMX_START_ADRESS;
			interval_dmx_send_0 = DMX_RESET_TIME;	// reset idõ 

			for (int i = 0; i<15; i++)	  // buffr frissítése
			{
				dmx_send_buffer[i] = *(dmx_adress_pointer+i);
			}
		}
	} 
 }