/*
 * timer.h
 *
 * Created: 2025. 10. 09. 23:32:37
 *  Author: bagob
 */ 


#ifndef TIMER_H_
#define TIMER_H_

		#include <avr/io.h>
		#include <avr/interrupt.h>
		
		uint32_t millis(void);
		void timers_init(void);
		void timer_1_init(void);
		void set_timer_1_ovf_value(uint64_t ovf_value);
		uint16_t get_timer_1_value(void);
		void enable_timer_1_out(void);
		void disable_timer_1_out(void);

		


#endif /* TIMER_H_ */