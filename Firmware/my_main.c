/*
 * my_main.c
 *
 *  Created on: Oct 18, 2025
 *      Author: bagob
 */

 #include <control_board_s.h>
 #include <my_main.h>

 #include <dmx_usart_s.h>
 #include <motor_drive.h>

 uint16_t dmx_pos = 0;
 static void tmp_timer(void);

 void my_main_init(void)
 {
	 control_board_init();
 }


 void my_main_loop(void)
 {
	 control_board_main();
	 dmx_usart_send();
	 motor_1_main(dmx_pos);
	 tmp_timer();

 }

 static void tmp_timer(void)
 {
    // timer
    static uint32_t tmp_current_time = 0;
    tmp_current_time = HAL_GetTick();

    static uint32_t tmp_time_0 = 0;
    static uint16_t tmp_interval_0 = 5000;	//10mp

    if ((uint32_t)(tmp_current_time - tmp_time_0)>= tmp_interval_0)
    {
 	   tmp_time_0 = tmp_current_time;

 	   // dmx pozíció változtatása
 	   if(dmx_pos) dmx_pos = 0;
 	   else  dmx_pos = 1000;
 	   //
    }
 }


