/*
 * API_Json.c
 *
 *  Created on: 19 feb. 2023
 *      Author: Facu
 */

#include "API_Json.h"




/*Functions declaration*/
uint8_t get_Json_value(uint8_t *Json, uint8_t *key, uint8_t *value_return, uint8_t return_max_lenght);


/*Global variables*/
Json_struct_t Json_from_DB;





/* ------------------- PARA PRUEBA ------------------------- */

char *JsonString = "{\"Enviar\":\"Mensaje de prueba\",\"Horario\":\"12-15\",\"Pasar_a_modo_WiFi\":0,\"Periodo\":30,\"Ventana\":10}";
void testJson(void);

void testJson(void){
	if(analizar_json(&Json_from_DB,(uint8_t *)JsonString)==1){
		printf("analizar_json() OK!\n");
	} else {
		printf("FALLA en analizar_json()\n");
	}
	printf(">>Enviar: %s\n",Json_from_DB.Mensaje.value);
	printf(">>Horario: %s\n",Json_from_DB.Time_inicio_programado.value);
	printf(">>Periodo: %d\n",Json_from_DB.Periodo_seconds.value);
	printf(">>Ventana: %d\n",Json_from_DB.Ventana_minutes.value);
	printf(">>Pass_to_WiFi: %d\n",Json_from_DB.Pass_to_WiFi.value);
}






/*
 * @brief Asigna valores por defecto a la estructura Json "Json_from_DB"
 */
void iniciar_Json_from_DB(void){
	Json_from_DB.Mensaje.key				= (uint8_t*)"Enviar";
	strcpy((char*)Json_from_DB.Mensaje.value,"Mensaje de ejemplo");
	Json_from_DB.Time_inicio_programado.key					= (uint8_t*)"Horario";
	strcpy((char*)Json_from_DB.Time_inicio_programado.value,"10-00");
	Json_from_DB.Periodo_seconds.key 	= (uint8_t*)"Periodo";
	Json_from_DB.Periodo_seconds.value	= 60;
	Json_from_DB.Ventana_minutes.key 	= (uint8_t*)"Ventana";
	Json_from_DB.Ventana_minutes.value 	= 30;
	Json_from_DB.Pass_to_WiFi.key 	= (uint8_t*)"Pasar_a_modo_WiFi";
	Json_from_DB.Pass_to_WiFi.value = 0;
}

/*
 * @brief Actualiza los valores de la estructura Json a partir del String en formato Json
 * @param Puntero a la estructura Json a actualizar
 * @param Puntero al String con el Json a leer
 * @return 0 si hay un error, devuelve 1 si OK
 */
uint8_t analizar_json(Json_struct_t *Json_struct, uint8_t *Json_String){
	if(Json_struct==0||Json_String==0){
		printf(">>ERROR>analizar_json()>Json_struct==0||Json_String==0\n");
		return 0;
	}
	/*Variables locales*/
	uint8_t int_in_String[INT_IN_STRING_SIZE];
	uint8_t int_control_error = 0;
	/*Conversión de los Json_String_element_t*/
	int_control_error += get_Json_value(Json_String,Json_struct->Mensaje.key,
			Json_struct->Mensaje.value,
			VALUE_STRING_MAX_LENGHT);
	int_control_error += get_Json_value(Json_String,Json_struct->Time_inicio_programado.key,
			Json_struct->Time_inicio_programado.value,VALUE_STRING_MAX_LENGHT);
	/*Conversión de los Json_uint_element_t */
	/*Tomar valor uint32_t de Periodo_seconds*/
	int_control_error += get_Json_value(Json_String,Json_struct->Periodo_seconds.key,int_in_String,INT_IN_STRING_SIZE);
	int_in_String[INT_IN_STRING_SIZE-1]='\0';
	Json_struct->Periodo_seconds.value = (uint32_t)atoi((char *)int_in_String);
	/*Tomar valor uint32_t de Ventana_minutes*/
	int_control_error += get_Json_value(Json_String,Json_struct->Ventana_minutes.key,int_in_String,INT_IN_STRING_SIZE);
	int_in_String[INT_IN_STRING_SIZE-1]='\0';
	Json_struct->Ventana_minutes.value = (uint32_t)atoi((char *)int_in_String);
	/*Tomar valor uint32_t de Pass_to_WiFi*/
	int_control_error += get_Json_value(Json_String,Json_struct->Pass_to_WiFi.key,int_in_String,INT_IN_STRING_SIZE);
	int_in_String[INT_IN_STRING_SIZE-1]='\0';
	Json_struct->Pass_to_WiFi.value = (uint32_t)atoi((char *)int_in_String);
	if(int_control_error!=CANT_ELEMENTOS_DE_JSON_STRUCT_T){	/*get_Json_value devuelve 1 si la conversión fué correcta*/
		printf(">>ERROR>analizar_json()>int_control_error!=CANT_ELEMENTOS_DE_JSON_STRUCT_T\n");
		return 0;
	}
	if(Json_struct->Mensaje.value==0){
		printf(">>ERROR>analizar_json()>Json_struct->Mensaje.value==0\n");
		return 0;
	}if(Json_struct->Time_inicio_programado.value==0){
		printf(">>ERROR>analizar_json()>Json_struct->Time_inicio_programado.value==0\n");
		return 0;
	}
	return 1;
}



/**
 * @brief  Devuelve el valor del key solicitado del Json. En caso de no encontrarse devuelve 0. Si se encontró el valor devuelve 1.
 * @param  Puntero al String del Json
 * @param  Puntero al String del key a buscar
 * @param  Puntero a la posición de memoria en donde se guardará el valor encontrado
 * @param  Largo máximo del String del valor a devolver, por defecto 30.
 * @return Devuelve 1 si encontró el key y pudo obtener valor, 0 si no encontró el key o no se pudo obtener el valor
 *
 * @Note  Ejemplo de Json: {"Enviar":"Mensaje de prueba","Horario":"10-00","Periodo":30,"Ventana":10}
 */
uint8_t get_Json_value(uint8_t *Json, uint8_t *key, uint8_t *value_return, uint8_t return_max_lenght){
    if(Json==0||key==0||value_return==0||return_max_lenght==0){
    	return 0;
    }
    uint8_t *indexIn;								/*Inicio del value		*/
    uint8_t *indexOut;								/*Fin del value			*/
    indexIn=(uint8_t*)strstr((char*)Json, (char*)key);						/*Posicionarse al comienzo del key		*/
    if(indexIn==0){						/*Si no encontró el key, strstr() devuelve 0		*/
		printf(">>ERROR>get_Json_value()>KEY NO ENCONTRADO:%s\n",(char*)key);
    	return 0;
    }
    if(strlen((char*)indexIn)<strlen((char*)key)+3){	/*Si no posee largo suficiente para obtener valor devolver 0	*/
    	return 0;
    }
    indexIn+=strlen((char*)key)+2;				/*Se posiciona justo despues del caracter ':' que separa key de value		*/
    if(*indexIn=='"'){					/*Si después de ':' hay un '"', el value es un String y el value está entre comillas dobles*/
    	indexIn++;
        indexOut=(uint8_t*)strstr((char*)indexIn,"\"");
    } else {
        indexOut=(uint8_t*)strstr((char*)indexIn,",");	/*Si value no es un String, al final del value hay una ',' separandolo del siguiente key */
    	if(indexOut==0){ 	 	 	 	/*o hay un '}' indicando que es el fin del Json	 	 	 	 	 	 	 	 	 		 */
    		indexOut=(uint8_t*)strstr((char*)indexIn,"}");
    	}
    }
	if(indexOut==0){	/*Si no se encontro el caracter de cierre del value se devuelve 0*/
		return 0;
	}
	uint32_t value_lenght = indexOut-indexIn;
	if(value_lenght > return_max_lenght){
		return 0;
	}
	strncpy((char*)value_return,(char*)indexIn,value_lenght);
	value_return[value_lenght] = '\0';
    return 1;
}
