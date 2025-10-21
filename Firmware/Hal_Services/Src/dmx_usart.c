/*
 * dmx_usart.c
 *
 * Created: 2025. 10. 12. 19:37:49
 *  Author: bagob
 */ 

 #include "dmx_usart.h"
 #include "main.h"

 #define DMX_RESET_TIME 15
 #define DMX_SEND_TIME 5
 #define DMX_START_ADRESS 0
 #define DMX_MAX_ADRESS 15

 uint8_t *dmx_adress_pointer;
 uint8_t dmx_array[512];


 static uint16_t dmx_receive_num = 0;
 // private 




 void usart_rx_callback(uint8_t data)
 {
	// dmx olvasas 
	
	
	dmx_array[dmx_receive_num] = data;
	if(dmx_receive_num < 512)dmx_receive_num ++;  // t�mb t�lc�mz�s v�delem

 }

 void usart_rx_fe_callback(void)
  {
	 dmx_receive_num = 0; // frame error -->reset
	 //HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  }




 void dmx_usart_send(void)
 {
	// minden ciklusban lefut 
	// id�alapon k�ldi tov�bb a jelet 
	// saj�t bufferb�l dolgozik 

	static uint32_t current_time = 0;
	static uint32_t time_dmx_send = 0;
	static uint16_t interval_dmx_send_0 = DMX_SEND_TIME;	
	//static uint16_t interval_dmx_send_1 = DMX_RESET_TIME;
		
	static uint8_t dmx_send_buffer[16];	
	static uint8_t send_n = 0;

	current_time = HAL_GetTick();

	if ((uint32_t)(current_time - time_dmx_send)>= interval_dmx_send_0)		//fix 10ms enk�nt lefut 
	{
		time_dmx_send = current_time;
		//usart_transmit_poll(dmx_send_buffer[send_n]);
		//HAL_UART_Transmit(huart1, (uint8_t*)dmx_send_buffer[send_n], 1, 100);
		//usart_transmit();
		usart_transmit(&(dmx_send_buffer[send_n]));
		send_n++;
		interval_dmx_send_0 = DMX_SEND_TIME;	// reset id� 

		if(send_n > DMX_MAX_ADRESS) // max value ig k�ldj�k 
		{
			send_n = DMX_START_ADRESS;
			interval_dmx_send_0 = DMX_RESET_TIME;	// reset id� 

			for (int i = 0; i<15; i++)	  // buffr friss�t�se
			{
				dmx_send_buffer[i] = *(dmx_adress_pointer+i);
			}
		}
	}
 }
