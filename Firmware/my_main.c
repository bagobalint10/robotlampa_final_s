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


 //uint16_t dmx_pos = 0;
 //uint8_t tmp_speed = 150;
 static void tmp_timer(void);

 #define	PAN_CH 11
 #define	PAN_F_CH 12
 #define	TILT_CH 13
 #define	TILT_F_CH 14
 #define	PT_SPEED_CH 15

 static uint16_t dmx_pan_value = 0;
 static uint16_t dmx_tilt_value = 0;
 static uint8_t  dmx_speed_value = 0;

 void dmx_channel_map(void);



 void my_main_init(void)
 {
	 control_board_init();
 }


 void my_main_loop(void)
 {
	 control_board_main();
	// dmx_usart_send();
	 motor_1_main(dmx_pan_value,dmx_speed_value); // dmx pos*2 bemenő uint32t
	 tmp_timer();
	 dmx_channel_map();




	 // dmx mapping
	 //dmx_pos = ((uint16_t)dmx_array[4]<<8)  | ((uint16_t)dmx_array[5]);//+5
	 //dmx_pos = ;//+5
	 //tmp_speed = 255- dmx_array[3];
	 //dmx_pos = ((uint16_t)(*(dmx_adress_pointer + 3))<<8) & (uint16_t)*(dmx_adress_pointer + 4);
	 /*extern uint8_t *dmx_adress_pointer;
	 extern uint8_t dmx_array[512];*/

 }

 void dmx_channel_map(void)
 {

	dmx_pan_value = ((uint16_t)*(dmx_adress_pointer + (PAN_CH-1))<<8)  | ((uint16_t)*(dmx_adress_pointer + (PAN_F_CH-1)));
	dmx_tilt_value = ((uint16_t)*(dmx_adress_pointer + (TILT_CH-1))<<8)  | ((uint16_t)*(dmx_adress_pointer + (TILT_F_CH-1)));
	dmx_speed_value = *(dmx_adress_pointer + (PT_SPEED_CH-1));
 }





 static void tmp_timer(void)
 {
    // timer
    static uint32_t tmp_current_time = 0;
    tmp_current_time = HAL_GetTick();

    static uint32_t tmp_time_0 = 0;
    static uint16_t tmp_interval_0 = 12000;	//10mp

    if ((uint32_t)(tmp_current_time - tmp_time_0)>= tmp_interval_0)
    {
 	   tmp_time_0 = tmp_current_time;

 	   // dmx pozíció változtatása
 	   /*if(dmx_pos) dmx_pos = 0,tmp_speed = 255;
 	   else  dmx_pos = 11000, tmp_speed = 255; // 33000 max == int !!!*/
 	   //
    }
 }


