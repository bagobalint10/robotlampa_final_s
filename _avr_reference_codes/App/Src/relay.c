/*
 * relay.c
 *
 * Created: 2025. 10. 12. 14:48:17
 *  Author: bagob
 */ 

 #include "relay.h"

 #include "gpio.h"
 #include "port_config.h"



 void relay_init(void)
 {
	PORT_Init(&RELAY_DIR, RELAY_PIN, OUTPUT); 
 }
 void relay_set(void)
 {
	PORT_Write(&RELAY_PORT, RELAY_PIN, 1);
 }
 void relay_reset(void)
 {
	PORT_Write(&RELAY_PORT, RELAY_PIN, 0); 
 }

