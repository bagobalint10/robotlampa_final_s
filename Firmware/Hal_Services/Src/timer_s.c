/*
 * timer.c
 *
 * Created: 2025. 10. 09. 23:33:02
 *  Author: bagob
 */ 

 #include <timer_s.h>
 #include "interrupt.h"

////  public

  void set_timer_1_ovf_value(uint64_t ovf_value)
  {
	  //ICR1 = ovf_value;

  }

  uint16_t get_timer_1_value(void)
  {
	//return ((uint16_t)(TCNT1H << 8)) | ((uint16_t) TCNT1L);		// kiolvasni a timer �rt�k�t
	  return 0; //ideiglenes
  }


  void enable_timer_1_out(void)
  {
	 //TCCR1A |= (1 << COM1A1) | (0 << COM1A0);					// PWM kimenet enged�lyez�se
  }

  void disable_timer_1_out(void)
  {
	  //TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));				// PWM lev�laszt�sa
  }

