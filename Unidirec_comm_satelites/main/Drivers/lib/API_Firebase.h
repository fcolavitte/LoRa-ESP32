/*
 * API_Firebase.h
 *
 *  Created on: 9 feb. 2023
 *      Author: Facundo
 */

/*
 *	https://prueba-iot-satellogic-default-rtdb.firebaseio.com/.json
 * 		> "Enviar":"Mensaje de prueba"		//Mensaje Beacon a tansmitir
 * 		> "Horario":"10-00"					//Hora de inicio de transmición
 * 		> "Modo":1							//Modo de funcionamiento del ESP (Beacon constante, en una hora particular o apagado)
 * 		> "Periodo":60						//Tiempo en segundos cada cuanto se va a enviar un mensaje
 * 		> "Ventana":10						//Tiempo en minutos de la ventana en que se va a transmitir de forma continua tras la hora de transmisión
 */

#ifndef MAIN_DRIVERS_LIB_API_FIREBASE_H_
#define MAIN_DRIVERS_LIB_API_FIREBASE_H_


#include "main.h"
#include "esp_http_client.h"
#include "API_Json.h"


#define PORT 443
//#define URL "https://prueba-iot-satellogic-default-rtdb.firebaseio.com/.json"
#define USERNAME ""		//credenciales de acceso a Firebase, no se necesitan porque está desactivada la autentificacion de usuarios.
#define PASSWORD ""		//credenciales de acceso a Firebase, no se necesitan porque está desactivada la autentificacion de usuarios.




void client_get_Json(void);
esp_err_t client_event_Json_handler(esp_http_client_event_handle_t evt);
void client_post_frecuencia_seteada(int f);
esp_err_t client_event_get_frecuencia_seteada_handler(esp_http_client_event_handle_t evt);


uint8_t *get_firebase_message(void);
uint8_t *get_firebase_Time_inicio_programado(void);
uint32_t get_firebase_Periodo_seconds(void);
uint32_t get_firebase_Ventana_minutes(void);
uint8_t  get_firebase_pass_to_WiFi(void);
void clear_firebase_pass_to_WiFi(void);
void print_firebase_dates(void);

#endif /* MAIN_DRIVERS_LIB_API_FIREBASE_H_ */
