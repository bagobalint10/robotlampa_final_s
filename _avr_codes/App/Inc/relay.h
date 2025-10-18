/*
 * relay.h
 *
 * Created: 2025. 10. 12. 14:48:05
 *  Author: bagob
 */ 


#ifndef RELAY_H_
#define RELAY_H_
	#include <avr/io.h>


	void relay_init(void);
	void relay_set(void);
	void relay_reset(void);




#endif /* RELAY_H_ */