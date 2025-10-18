/*
 * interrupt.c
 *
 * Created: 2025. 10. 09. 17:25:16
 *  Author: bagob
 */ 

#include "interrupt.h"
#include "port_config.h"
#include "stddef.h"

// PCINT pointers 
static void (*pcint_b_callback_pointer)(void) = NULL;
static void (*pcint_c_callback_pointer)(void) = NULL;
static void (*pcint_d_callback_pointer)(void) = NULL;
// TIMER int pointers
static void (*timer_0_callback_pointer)(void) = NULL;
static void (*timer_1_callback_pointer)(void) = NULL;
static void (*timer_2_callback_pointer)(void) = NULL;
// USART int pointers 
static void (*usart_rx_callback_pointer)(void) = NULL;


 void set_pcint_Callback(uint8_t port ,void (*Callback_function)(void) )
 {
	 
	switch(port)
	{
		case 0 : pcint_b_callback_pointer = Callback_function; break;
		case 1 : pcint_c_callback_pointer = Callback_function; break;
		case 2 : pcint_d_callback_pointer = Callback_function; break;
		default : break;
	} 
 }

 void pcint_init(uint8_t port, uint8_t mask)  //pcint init hal 
 {

	switch (port)
	{
	case 0 :											// B
				PCICR |= (1 << PCIE0);					// enable port B pcint 
				PCMSK0 |= PCMSK0_VALID_MASK & mask;		// port B pin mask 
				sei();	
				break;
	case 1 :											// C
				PCICR |= (1 << PCIE1);					// enable port B pcint
				PCMSK1 |= PCMSK1_VALID_MASK & mask;		// port B pin mask
				sei();
				break;
	case 2 :											// D
				PCICR |= (1 << PCIE2);					// enable port B pcint
				PCMSK2 |= PCMSK2_VALID_MASK & mask;		// port B pin mask
				sei();
				break;
	default:	break;
	}	 
 }



 // PCINT vectors 
 ISR(PCINT0_vect)
 {
	if(pcint_b_callback_pointer) pcint_b_callback_pointer(); // ha tettünk bele callbacket hívja meg 
 }
 ISR(PCINT1_vect)
 {
	if(pcint_c_callback_pointer) pcint_c_callback_pointer(); // ha tettünk bele callbacket hívja meg 
 }
 ISR(PCINT2_vect)
 {
	if(pcint_d_callback_pointer) pcint_d_callback_pointer(); // ha tettünk bele callbacket hívja meg 
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


void timer_int_init(uint8_t timer, uint8_t interrupt_type)  //pcint init hal
{
	switch (timer)
	{
		case 0 :
					TIMSK0 = 0;		
					TIMSK0 = interrupt_type;
					sei();
					break;
		case 1 :	break;
		case 2 :	break;
		default:	break;
	}
}



 //	TIM 0 vectors
 ISR(TIMER0_COMPA_vect)
 {
	if(timer_0_callback_pointer) timer_0_callback_pointer(); // ha tettünk bele callbacket hívja meg
 }

 ISR(TIMER0_OVF_vect)
 {
	if(timer_0_callback_pointer) timer_0_callback_pointer(); // ha tettünk bele callbacket hívja meg
 }

 // tim 1 vectors 
  ISR(TIMER1_OVF_vect)	// áttenni interrupt.c be
  {
	if(timer_1_callback_pointer) timer_1_callback_pointer(); // ha tettünk bele callbacket hívja meg
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

 ISR(USART_RX_vect)
 {
	if(usart_rx_callback_pointer) usart_rx_callback_pointer(); // ha tettünk bele callbacket hívja meg	
 }