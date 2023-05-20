/*
 * @file   : main.h
 * @date   : Mar 03, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

#ifndef MAIN_DRIVERS_LIB_MAIN_H_
#define MAIN_DRIVERS_LIB_MAIN_H_

/********************** inclusions *******************************************/

#include "sdkconfig.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "esp_wifi.h"

/********************** macros and definitions *******************************/

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

void start_MEF(void);
void update_MEF(void);

#endif /* MAIN_DRIVERS_LIB_MAIN_H_ */

/********************** end of file ******************************************/
