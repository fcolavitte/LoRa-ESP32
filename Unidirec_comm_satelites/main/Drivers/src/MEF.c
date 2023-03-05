/*
 * MEF.c
 *
 *  Created on: 17 feb. 2023
 *      Author: Facundo
 */
#include <API_Time.h>
#include "main.h"
#include "API_WiFi.h"
#include "API_Firebase.h"
#include "API_USB.h"

#define DEBUG 0



/*		----- Typedef -----		*/


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
	uint8_t *Mensaje;
	uint8_t *Time_inicio_programado;
	uint32_t Periodo_seconds;
	uint32_t Ventana_minutes;
} device_config_t;


typedef enum{
	menu_main 		= 0,
	menu_mensaje 	= 1,
	menu_config_web = 2,
	menu_wifi_config 	 = 21,
	menu_firebase_config = 22,
	menu_time_config 	 = 23
}pos_menu_t;

typedef enum{
	USB_mode,
	WiFi_mode
}MEF_mode_t;



extern uint8_t pass_to_WiFi;


/*		----- Variables Globales privadas -----		*/
pos_menu_t pos_menu_actual;
MEF_mode_t MEF_mode = WiFi_mode;
MEF_mode_t MEF_mode_previo = WiFi_mode;
delay_t delay_get_json;


/*		----- Funciones privadas -----		*/
void reset_config_dataBase(void);


void star_MEF(void){
	/*Cargar estados de la MEF*/
	MEF_mode = WiFi_mode;
	MEF_mode_previo = WiFi_mode;
	pos_menu_actual = menu_main;
	/*Conectividad*/
	WiFiConect();
	//Verificar conexión WiFi
	vTaskDelay(5000/ portTICK_PERIOD_MS);
	print_MAC();
	printf("\n\n");
	/*Cargar datos desde FireBase*/
	iniciar_Json_from_DB();
	client_get_Json();
	print_firebase_dates();
	/*Cargando hora del sistema*/
	UTP_init();
	display_menu();
	delayInit(&delay_get_json,10);
	printf("\n>> El dispositivo se inició en modo WiFi.\n");
	printf(">> Envíe cualquier línea o caracter por USB para pasarlo a modo USB.\n");
	printf(">> Luego se puede regresar al modo WiFi con el comando \"$wifi\",");
	printf("o desde la base de datos haciendo 1 la variable \"Pasar_a_modo_WiFi\".\n\n");
}

void update_MEF(void){
	if(USB_get_input()){	/*Si se recibe algo por USB se debe pasar a USB_mode*/
		MEF_mode = USB_mode;
		if(pass_to_WiFi==1){
			MEF_mode = WiFi_mode;
		}
	}
	if(delayRead(&delay_get_json)){
		client_get_Json();
		if(get_firebase_pass_to_WiFi()){
			MEF_mode = WiFi_mode;
			clear_firebase_pass_to_WiFi();
		}
		if(MEF_mode == WiFi_mode){
			print_firebase_dates();
		}
	}

	switch(MEF_mode){
	case WiFi_mode:
#if DEBUG == 1
		printf("WIFI MODE\n");
#endif
		if(MEF_mode_previo == USB_mode){
			MEF_mode_previo = WiFi_mode;
			printf(">> Se pasó al modo WiFi\n");
		}
		break;
	case USB_mode:
#if DEBUG == 1
		printf("USB MODE\n");
#endif
		if(MEF_mode_previo == WiFi_mode){
			MEF_mode_previo = USB_mode;
			printf(">> Se pasó al modo USB\n");
			display_menu();
		}
		break;
	default:
		star_MEF();
	}
}






void reset_config_dataBase(void){

}



