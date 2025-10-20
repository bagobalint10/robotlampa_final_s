/*
 * interrupt.c
 *
 * Created: 2025. 10. 09. 17:25:16
 *  Author: bagob
 */ 

#include "interrupt.h"
//#include "port_config.h"
#include "stddef.h"

// PCINT pointers 
static void (*pcint_b_callback_pointer)(uint16_t GPIO_Pin) = NULL;
static void (*pcint_c_callback_pointer)(uint16_t GPIO_Pin) = NULL;
//static void (*pcint_d_callback_pointer)(void) = NULL;
// TIMER int pointers
static void (*timer_0_callback_pointer)(void) = NULL;
static void (*timer_1_callback_pointer)(void) = NULL;
static void (*timer_2_callback_pointer)(void) = NULL;
// USART int pointers 
static void (*usart_rx_callback_pointer)(void) = NULL;






/*//// ISR

 // PCINT vectors 
 ISR(PCINT0_vect)
 {
	if(pcint_b_callback_pointer) pcint_b_callback_pointer(); // ha tett�nk bele callbacket h�vja meg 
 }
 ISR(PCINT1_vect)
 {
	if(pcint_c_callback_pointer) pcint_c_callback_pointer(); // ha tett�nk bele callbacket h�vja meg 
 }
 ISR(PCINT2_vect)
 {
	if(pcint_d_callback_pointer) pcint_d_callback_pointer(); // ha tett�nk bele callbacket h�vja meg 
 }

 //	TIM 0 vectors
 ISR(TIMER0_COMPA_vect)
 {
	if(timer_0_callback_pointer) timer_0_callback_pointer(); // ha tett�nk bele callbacket h�vja meg
 }

 ISR(TIMER0_OVF_vect)
 {
	if(timer_0_callback_pointer) timer_0_callback_pointer(); // ha tett�nk bele callbacket h�vja meg
 }

 // tim 1 vectors
  ISR(TIMER1_OVF_vect)	// �ttenni interrupt.c be
  {
	if(timer_1_callback_pointer) timer_1_callback_pointer(); // ha tett�nk bele callbacket h�vja meg
  }

  ISR(USART_RX_vect)
   {
  	if(usart_rx_callback_pointer) usart_rx_callback_pointer(); // ha tett�nk bele callbacket h�vja meg
   }

  */

 void ISR_GPIO_EXTI_Callback(uint16_t GPIO_Pin) // meghívja a pointer függvényt
 {


		if(pcint_b_callback_pointer) pcint_b_callback_pointer(GPIO_Pin);	// hall
		if(pcint_c_callback_pointer) pcint_c_callback_pointer(GPIO_Pin); 	// gombok
 }




  void set_pcint_Callback(uint8_t port ,void (*Callback_function)(uint16_t) )
   {

  	switch(port)
  	{
  		case 0 : pcint_c_callback_pointer = Callback_function; break; // gombok int
  		case 1 : pcint_b_callback_pointer = Callback_function;  break;
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
			//case 1 : timer_1_callback_pointer = Callback_function; break;
			default : break;
		}
 }

