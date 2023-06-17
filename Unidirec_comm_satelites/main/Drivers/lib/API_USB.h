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

#define USB_INPUT_LENGHT 40

/********************** typedef **********************************************/

typedef enum{
	menu_main 		= 0,
	menu_mensaje 	= 1,
	menu_config_web = 2,
	menu_mensaje_beacon 				= 11,	/* señal a emitir */
	menu_mensaje_signal_mode 			= 12,	/* Modo a emitir */
	menu_mensaje_periodo_emision		= 13,	/* Tiempo en segundos entre emisiones */
	menu_mensaje_ventana_emision		= 14,	/* Ventana de emisión en minutos */
	menu_mensaje_start_emision			= 15,	/* Inicio de emisión programada en segundos desde el inicio del día */
	menu_lora_config 	 = 21,
	menu_wifi_config 	 = 22,
	menu_time_config 	 = 23,
	menu_lora_frec_portadora		= 211,
	menu_lora_baudrate_air			= 212,	/* Velocidad de transmisión en aire */
	menu_lora_header_PreambleLength	= 213,	/* información del encabezado de la comunicación en aire */
	menu_lora_header_fixed_length	= 214,
	menu_lora_power_output			= 215,
	menu_wifi_SSD 	 = 221,
	menu_wifi_PASS	 = 222,
	menu_time_set_fecha	 = 231,
	menu_time_set_hora	 = 232
}pos_menu_t;


/* Mensajes programados */

typedef enum{
	null,		/*No envía mensaje, solo cuando se pone manualmente por USB con el caracter '>' */
	continuo,	/*Funciona en modo "Beacon"	repitiendo el mensaje continuamente cada un cierto periodo*/
	programado	/*Emite el mensaje continuamente a una hora determinada y durante una ventana de tiempo determinada*/
}message_mode_t;

/**
 * @brief Estructura de datos de configuración WiFi y guardados en DB
 */
typedef struct{
	message_mode_t message_mode;
	uint8_t Mensaje[41];
	uint32_t Time_inicio_programado_segundos; /* Segundos desde que inició el día */
	uint32_t Periodo_seconds;
	uint32_t Ventana_minutes;
} message_mode_config_t;

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

/**
 *	@brief	Getter de la estructura que define la configuración de los envios mensajes programados
 */
void get_estruct_message_mode_config(message_mode_config_t * estructura_donde_guardar_la_info);

/**
 *	@brief	Setter de la estructura que define la configuración de los envios mensajes programados
 */
void set_estructura_message_mode_config(message_mode_config_t * estructura_con_la_info);

/**
 *	@brief	Setear la posición del menú en main
 */
void set_posicion_menu_actual_main (void);

#endif /* MAIN_DRIVERS_LIB_API_USB_H_ */

/********************** end of file ******************************************/
