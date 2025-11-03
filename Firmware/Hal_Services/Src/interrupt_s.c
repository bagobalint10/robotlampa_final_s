/*
 * interrupt.c
 *
 * Created: 2025. 10. 09. 17:25:16
 *  Author: bagob
 */ 

 #include <interrupt_s.h>
 #include "stddef.h"

 // PCINT pointers
 static void (*pcint_b_callback_pointer)(uint16_t GPIO_Pin) = NULL;
 static void (*pcint_c_callback_pointer)(uint16_t GPIO_Pin) = NULL;
 // TIMER int pointers
 static void (*timer_0_callback_pointer)(void) = NULL;
 static void (*timer_1_callback_pointer)(void) = NULL;
 static void (*timer_2_callback_pointer)(void) = NULL;
 // USART int pointers
 static void (*usart_rx_callback_pointer)(void) = NULL;


 void ISR_GPIO_EXTI_Callback(uint16_t GPIO_Pin) // meghívja a pointer függvényt
 {
	 if(pcint_b_callback_pointer) pcint_b_callback_pointer(GPIO_Pin);	// hall
	 if(pcint_c_callback_pointer) pcint_c_callback_pointer(GPIO_Pin); 	// gombok
 }

 void set_pcint_Callback(uint8_t port ,void (*Callback_function)(uint16_t) )
 {
  	switch(port)
  	{
  		case 0 : pcint_c_callback_pointer = Callback_function; break; 	// gombok int
  		case 1 : pcint_b_callback_pointer = Callback_function;  break;	// hall int
  		case 2 :  break;
  		default : break;
  	}
 }

 //timer 
 void set_timer_int_Callback(uint8_t timer ,void (*Callback_function)(void) )
 {
	 switch(timer)
	 {
	 case 0 : timer_0_callback_pointer = Callback_function; break;
	 case 1 : timer_1_callback_pointer = Callback_function; break;
	 case 2 : timer_2_callback_pointer = Callback_function; break;
	 default : break;
	 }
 }
 // usart 

void set_usart_int_Callback(uint8_t type ,void (*Callback_function)(void) )
{
	switch(type)
	{
		case 0 : usart_rx_callback_pointer = Callback_function; break;
		default : break;
	}
}
