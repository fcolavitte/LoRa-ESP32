/*
 * API_Json.h
 *
 *  Created on: 2 mar. 2023
 *      Author: Facundo
 */

#ifndef MAIN_DRIVERS_LIB_API_JSON_H_
#define MAIN_DRIVERS_LIB_API_JSON_H_

#include "main.h"




#define KEY_STRING_MAX_LENGHT 	(uint32_t)8 		/*sizeof de "Mensaje" incluyendo el '\0'			*/
#define KEY_UINT_MAX_LENGHT 	(uint32_t)18 		/*sizeof de "Pasar_a_modo_WiFi" incluyendo el '\0'	*/
#define INT_IN_STRING_SIZE		(uint8_t)20			/*Tamaño máximo del string para pasar a entero		*/
#define CANT_ELEMENTOS_DE_JSON_STRUCT_T		(uint32_t)5





/*
 * @brief Actualiza los valores de la estructura Json a partir del String en formato Json
 * @param Puntero a la estructura Json a actualizar
 * @param Puntero al String con el Json a leer
 * @return 0 si hay un error, 1 todo OK
 */
uint8_t analizar_json(Json_struct_t *Json_struct, uint8_t *Json_String);

/*
 * @brief Asigna valores por defecto a la estructura Json "Json_from_DB"
 */
void iniciar_Json_from_DB(void);

#endif /* MAIN_DRIVERS_LIB_API_JSON_H_ */
