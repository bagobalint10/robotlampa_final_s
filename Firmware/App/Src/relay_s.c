/*
 * relay.c
 *
 * Created: 2025. 10. 12. 14:48:17
 *  Author: bagob
 */ 

#include <relay_s.h>


void relay_set(void)
{
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 1);
}
void relay_reset(void)
{
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 0);
}

