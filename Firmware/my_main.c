/*
 * my_main.c
 *
 *  Created on: Oct 18, 2025
 *      Author: bagob
 */

 #include <control_board_s.h>
#include <my_main.h>

 #include <dmx_usart_s.h>


 void my_main_init(void)
 {
	 control_board_init();
 }


 void my_main_loop(void)
 {
	 control_board_main();
	 dmx_usart_send();

 }


