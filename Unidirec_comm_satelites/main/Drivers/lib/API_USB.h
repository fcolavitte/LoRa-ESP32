/*
 * @file   : API_USB.h
 * @date   : Feb 15, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

#ifndef MAIN_DRIVERS_LIB_API_USB_H_
#define MAIN_DRIVERS_LIB_API_USB_H_

/********************** inclusions *******************************************/

#include <driver/uart.h>
#include <stdint.h>

/********************** macros and definitions *******************************/

#define USB_INPUT_LENGHT 30

/********************** typedef **********************************************/

typedef enum{
	menu_main 		= 0,
	menu_mensaje 	= 1,
	menu_config_web = 2,
	menu_lora_config 	 = 21,
	menu_wifi_config 	 = 22,
	menu_time_config 	 = 23
}pos_menu_t;

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

/**
 * @brief Revisa si hay caracteres de entrada por USB. Con cada llamada a la función toma un caracter adicional.
 * 		  Guarda los caracteres recibidos en el String USB_input[].
 * 		  Al alcanzarse el largo máximo o detectar un caracter de finalización llama a la función analizar_input_USB()
 * @return Devuelve 1 si se recibió un caracter por USB. En caso de que no haya nada en el buffer devuelve 0.
 */
uint8_t USB_get_input(void);

/**
 * @brief Se muestra las opciones del menú actual por el puerto serie.
 */
void display_menu(void);

#endif /* MAIN_DRIVERS_LIB_API_USB_H_ */

/********************** end of file ******************************************/
