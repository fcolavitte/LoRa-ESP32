/*
 * @file   : API_Json.h
 * @date   : Mar 02, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

#ifndef MAIN_DRIVERS_LIB_API_JSON_H_
#define MAIN_DRIVERS_LIB_API_JSON_H_

/********************** inclusions *******************************************/

#include <stdint.h>

/********************** macros and definitions *******************************/

#define KEY_STRING_MAX_LENGHT 	(uint32_t)8 		/*sizeof de "Mensaje" incluyendo el '\0'			*/
#define KEY_UINT_MAX_LENGHT 	(uint32_t)18 		/*sizeof de "Pasar_a_modo_WiFi" incluyendo el '\0'	*/
#define INT_IN_STRING_SIZE		(uint8_t)20			/*Tamaño máximo del string para pasar a entero		*/
#define CANT_ELEMENTOS_DE_JSON_STRUCT_T		(uint32_t)5
#define VALUE_STRING_MAX_LENGHT (uint8_t)30

/********************** typedef **********************************************/

typedef struct{
	uint8_t *key;//key[KEY_STRING_MAX_LENGHT];
	uint8_t value[VALUE_STRING_MAX_LENGHT];
} Json_String_element_t;

typedef struct{
	uint8_t *key;//key[KEY_UINT_MAX_LENGHT];
	uint32_t value;
} Json_uint_element_t;

typedef struct{
	struct {
		Json_String_element_t Mensaje;
		Json_String_element_t Time_inicio_programado;
		Json_uint_element_t   Periodo_seconds;
		Json_uint_element_t   Ventana_minutes;
		Json_uint_element_t   Pass_to_WiFi;
	};
} Json_struct_t;

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

/**
 * @brief Actualiza los valores de la estructura Json a partir del String en formato Json
 * @param Puntero a la estructura Json a actualizar
 * @param Puntero al String con el Json a leer
 * @return 0 si hay un error, devuelve 1 si OK
 */
uint8_t analizar_json(Json_struct_t *Json_struct, uint8_t *Json_String);

/**
 * @brief Asigna valores por defecto a la estructura Json "Json_from_DB"
 */
void iniciar_Json_from_DB(void);

#endif /* MAIN_DRIVERS_LIB_API_JSON_H_ */

/********************** end of file ******************************************/
