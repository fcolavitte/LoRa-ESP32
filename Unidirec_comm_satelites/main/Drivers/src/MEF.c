/*
 * @file   : MEF.c
 * @date   : Feb 17, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include "MEF.h"
#include "API_Time.h"
#include "API_Json.h"
#include "API_WiFi.h"
#include "API_Firebase.h"
#include "API_USB.h"
#include "API_E22.h"
#include <stdint.h>
#include <stdio.h>

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

typedef enum{
	USB_mode,
	WiFi_mode
}MEF_mode_t;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/* Interfaz USB y Wi-Fi */
MEF_mode_t MEF_mode = WiFi_mode;
MEF_mode_t MEF_mode_previo = WiFi_mode;

/* Emisión de mensajes programados */
static message_mode_config_t message_mode_config;
static uint8_t mensaje_de_prueba[18] = "Mensaje de prueba";

/* Retardo entre consultas a firebase */
delay_t delay_get_json;
delay_t delay_entre_envio_de_mensajes;

/********************** external data definition *****************************/

extern uint8_t pass_to_WiFi;

/********************** internal functions definition ************************/

/**
 *	@brief	Actualizar MEF referida a la emisión de mensajes programados
 */
void update_MEF_message(void) {
	get_estruct_message_mode_config(&message_mode_config); //No está funcionando
	if (continuo == message_mode_config.message_mode) {
		/* Modo Beacon */
		delayWrite(&delay_entre_envio_de_mensajes, message_mode_config.Periodo_seconds);
		if(delayRead(&delay_entre_envio_de_mensajes)){
			uint8_t length = strlen((char*)message_mode_config.Mensaje);
			if (length > 40) {
				length = 40;
			}
			driver_E22_send_message(message_mode_config.Mensaje, length);
			printf("\n>> Se envió un mensaje programado tipo beacon.\n");
			printf("\n>> Mensaje enviado: %s\n", message_mode_config.Mensaje);
		}
	} else if(programado == message_mode_config.message_mode) {
		/* Modo Emisión de mensaje a hora programada */
		/* Cálculo de segundos desde que comenzó el día */
		time_t unixtime_now = get_time_unix_system();
		struct tm* struct_tm_now = localtime(&unixtime_now);
		uint32_t segundos_desde_inicio_del_dia = struct_tm_now->tm_hour*3600 + struct_tm_now->tm_min*60 + struct_tm_now->tm_sec;

		/* Verificación si se está en la ventana de emisión */
		if (message_mode_config.Time_inicio_programado_segundos < segundos_desde_inicio_del_dia) {
			if ((message_mode_config.Time_inicio_programado_segundos + message_mode_config.Ventana_minutes*60) > segundos_desde_inicio_del_dia ) {
				/* Se está dentro de la ventana de emisión */
				delayWrite(&delay_entre_envio_de_mensajes, message_mode_config.Periodo_seconds);
				if(delayRead(&delay_entre_envio_de_mensajes)){
					uint8_t length = strlen((char*)message_mode_config.Mensaje);
					driver_E22_send_message(message_mode_config.Mensaje, length);
					printf("\n>> Se envió un mensaje programado a la hora específica.\n");
					printf("\n>> Mensaje enviado: %s\n", message_mode_config.Mensaje);
				}
			}
		}
	}
}


/**
 *	@brief	Actualizar MEF referida a la interacción USB y Wi-Fi del dispositivo
 */
void update_MEF_USB_WiFi(void) {
	if(USB_get_input()){	/* Si se recibe algo por USB se debe pasar a USB_mode */
		MEF_mode = USB_mode;
		if(pass_to_WiFi==1){
			MEF_mode = WiFi_mode;
			pass_to_WiFi = 0;
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
		if(MEF_mode_previo == USB_mode){
			MEF_mode_previo = WiFi_mode;
			printf(">> Se pasó al modo WiFi\n");
		}
		break;
	case USB_mode:
		if(MEF_mode_previo == WiFi_mode){
			MEF_mode_previo = USB_mode;
			printf(">> Se pasó al modo USB\n");
			display_menu();
		}
		break;
	default:
		start_MEF();
	}
}

/********************** external functions definition ************************/

void start_MEF(void){
	/* Cargar estados de la MEF */
	MEF_mode = WiFi_mode;
	MEF_mode_previo = WiFi_mode;
	set_posicion_menu_actual_main();

	/* Conectividad */
	WiFiConect();

	/* Verificar conexión WiFi */
	vTaskDelay(5000/ portTICK_PERIOD_MS);
	print_MAC();
	printf("\n\n");

	/* Cargar datos desde FireBase */
	iniciar_Json_from_DB();
	client_get_Json();
	print_firebase_dates();

	/* Configurar modo de emisión de mensajes programados */
	message_mode_config.message_mode = null;
	message_mode_config.Mensaje[0] = '\0';
	strcpy((char*)message_mode_config.Mensaje, (char*)mensaje_de_prueba);
	message_mode_config.Time_inicio_programado_segundos = 15*3600;
	message_mode_config.Periodo_seconds = 30;	/* Un mensaje cada 30 segundos*/
	message_mode_config.Ventana_minutes = 15;	/* Se emite durante 15 minutos */
	set_estructura_message_mode_config(&message_mode_config);

	/* Cargando hora del sistema */
	UTP_init();
	display_menu();
	delayInit(&delay_get_json, 10);
	delayInit(&delay_entre_envio_de_mensajes, message_mode_config.Periodo_seconds);

	printf("\n>> El dispositivo se inició en modo WiFi.\n");
	printf(">> Envíe cualquier línea o caracter por USB para pasarlo a modo USB.\n");
	printf(">> Luego se puede regresar al modo WiFi con el comando \"$wifi\",");
	printf("o desde la base de datos haciendo 1 la variable \"Pasar_a_modo_WiFi\".\n\n");
}

void update_MEF(void){
	update_MEF_USB_WiFi();
	update_MEF_message();
}




