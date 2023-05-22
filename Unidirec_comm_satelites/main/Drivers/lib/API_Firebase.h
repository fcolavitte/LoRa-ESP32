/*
 * @file   : API_Firebase.h
 * @date   : Feb 09, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
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

/********************** inclusions *******************************************/

#include <stdint.h>

/********************** macros and definitions *******************************/

#define PORT 443
//#define URL "https://prueba-iot-satellogic-default-rtdb.firebaseio.com/.json"
#define USERNAME ""		//credenciales de acceso a Firebase, no se necesitan porque está desactivada la autentificacion de usuarios.
#define PASSWORD ""		//credenciales de acceso a Firebase, no se necesitan porque está desactivada la autentificacion de usuarios.

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

/**
 * @brief Solicita el Json completo de la base de datos web de FireBase y lo analiza para obtener los valores de los keys.
 * @note  Los valores se guardan en la estructura  Json_struct_t Json_from_DB.
 * @note  Si se quiere usar los valores de Json_from_DB se debe usar las funciones get_firebase_*** de éste archivo.
 */
void client_get_Json(void);

/**
 * @brief Se envía por puerto serie los pares key-value devueltos por el json de Firebase.
 */
void print_firebase_dates(void);

/**
 * @brief Envía un POST que modifique el valor de Pass_to_WiFi de FireBase a 0
 */
void clear_firebase_pass_to_WiFi(void);

/* Funciones aún sin implementar */
uint8_t *get_firebase_message(void);
uint8_t *get_firebase_Time_inicio_programado(void);
uint32_t get_firebase_Periodo_seconds(void);
uint32_t get_firebase_Ventana_minutes(void);
uint8_t  get_firebase_pass_to_WiFi(void);


#endif /* MAIN_DRIVERS_LIB_API_FIREBASE_H_ */

/********************** end of file ******************************************/
