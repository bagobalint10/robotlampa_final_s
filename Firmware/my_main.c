/*
 * my_main.c
 *
 *  Created on: Oct 18, 2025
 *      Author: bagob
 */

 #include <my_main.h>

 #include<control_board.h>


 void my_main_init(void)
 {
	 control_board_init();
 }


 void my_main_loop(void)
 {
	 control_board_main();
 }


