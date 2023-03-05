/*
 * API_Firebase.c
 *
 *  Created on: 9 feb. 2023
 *      Author: Facundo
 */

#include "API_Firebase.h"


#define DEBUG 0

uint8_t Json_String[140];
extern Json_struct_t Json_from_DB;

esp_err_t client_event_Json_handler(esp_http_client_event_handle_t evt);
esp_err_t client_event_POST_response_handler(esp_http_client_event_handle_t evt);


/**
 * @brief Solicita el Json completo de la base de datos web de FireBase y lo analiza para obtener los valores de los keys.
 * @note  Los valores se guardan en la estructura  Json_struct_t Json_from_DB.
 * @note  Si se quiere usar los valores de Json_from_DB se debe usar las funciones get_firebase_*** de éste archivo.
 */
void client_get_Json(void) {
    esp_http_client_config_t config_get = {
        .port = PORT,
        .url = "https://prueba-iot-satellogic-default-rtdb.firebaseio.com/.json",
        .method = HTTP_METHOD_GET,
        .username = USERNAME,
        .password = PASSWORD,
        .event_handler = client_event_Json_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config_get);
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}


/**
 * @brief Handler de la respuesta del GET en que se solicita el Json completo de FireBase
 */
esp_err_t client_event_Json_handler(esp_http_client_event_handle_t evt) {
    if(evt->event_id==HTTP_EVENT_ON_DATA) {
    	strncpy((char *)Json_String,(char *)evt->data,evt->data_len);
    	Json_String[139]=0;
#if DEBUG == 1
        printf(">> Json desde FireBase: %s\n", Json_String);
#endif
        analizar_json(&Json_from_DB, Json_String);
#if DEBUG == 1
        print_firebase_dates();
#endif
    }
    return ESP_OK;
}

void print_firebase_dates(void){
	printf(">> Datos recibidos desde FireBase:\n");
	printf("	>> Enviar: %s\n",Json_from_DB.Mensaje.value);
	printf("	>> Horario: %s\n",Json_from_DB.Time_inicio_programado.value);
	printf("	>> Periodo: %d\n",Json_from_DB.Periodo_seconds.value);
	printf("	>> Ventana: %d\n",Json_from_DB.Ventana_minutes.value);
	printf("	>> Pass_to_WiFi: %d\n",Json_from_DB.Pass_to_WiFi.value);
}


uint8_t *get_firebase_message(void){
	return Json_from_DB.Mensaje.value;
}
uint8_t *get_firebase_Time_inicio_programado(void){
	return Json_from_DB.Time_inicio_programado.value;
}
uint32_t get_firebase_Periodo_seconds(void){
	return Json_from_DB.Periodo_seconds.value;
}
uint32_t get_firebase_Ventana_minutes(void){
	return Json_from_DB.Ventana_minutes.value;
}
uint8_t get_firebase_pass_to_WiFi(void){
	return (uint8_t)Json_from_DB.Pass_to_WiFi.value;
}


/**
 * @brief Envía un POST que modifique el valor de Pass_to_WiFi de FireBase a 0
 */
void clear_firebase_pass_to_WiFi(void){
    esp_http_client_config_t config_post = {
        .port = PORT,
        .url = "https://prueba-iot-satellogic-default-rtdb.firebaseio.com/Pasar_a_modo_WiFi.json",
        .method = HTTP_METHOD_PUT,
        .username = USERNAME,
        .password = PASSWORD,
        .event_handler = client_event_POST_response_handler
    };
    esp_http_client_handle_t client = esp_http_client_init(&config_post);
    esp_http_client_set_post_field(client, "0", strlen("0"));
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}


/**
 * @brief Handler de la respuesta del POST que hace 0 el valor de "Pasar_a_modo_WiFi"
 */
esp_err_t client_event_POST_response_handler(esp_http_client_event_handle_t evt) {
    switch (evt->event_id) {
        case HTTP_EVENT_ON_DATA:
#if DEBUG == 1
            printf("\n>> FireBase: Respuesta del método POST: %.*s\n", evt->data_len, (char *)evt->data);
#endif
            break;
        default:
            break;
    }
    return ESP_OK;
}

