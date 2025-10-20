/*
 * relay.c
 *
 * Created: 2025. 10. 12. 14:48:17
 *  Author: bagob
 */ 

 #include "relay.h"




 /*void relay_init(void)
 {
	PORT_Init(&RELAY_DIR, RELAY_PIN, OUTPUT); 
 }*/
 void relay_set(void)
 {
	//PORT_Write(&RELAY_PORT, RELAY_PIN, 1);
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 1);
 }
 void relay_reset(void)
 {
	//PORT_Write(&RELAY_PORT, RELAY_PIN, 0);
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 0);
 }

