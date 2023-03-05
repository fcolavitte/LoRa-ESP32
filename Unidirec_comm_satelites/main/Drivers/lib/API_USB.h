/*
 * API_USB.h
 *
 *  Created on: 15 feb. 2023
 *      Author: Facundo
 */

#ifndef MAIN_DRIVERS_LIB_API_USB_H_
#define MAIN_DRIVERS_LIB_API_USB_H_


#include "stdio.h"
#include "main.h"
#include "driver/uart.h"


#define USB_INPUT_LENGHT 30

typedef enum{
	menu_main 		= 0,
	menu_mensaje 	= 1,
	menu_config_web = 2,
	menu_lora_config 	 = 21,
	menu_wifi_config 	 = 22,
	menu_firebase_config = 23,
	menu_time_config 	 = 24
}pos_menu_t;


uint8_t USB_get_input(void);
void display_menu(void);

#endif /* MAIN_DRIVERS_LIB_API_USB_H_ */
