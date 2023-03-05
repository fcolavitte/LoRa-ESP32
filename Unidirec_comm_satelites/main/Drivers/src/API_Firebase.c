/*
 * API_Firebase.c
 *
 *  Created on: 9 feb. 2023
 *      Author: Facundo
 */

#include "API_Firebase.h"


#define DEBUG 1

uint8_t Json_String[140];
extern Json_struct_t Json_from_DB;


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
	printf(">>Datos recibidos desde FireBase:\n");
	printf("	>>Enviar: %s\n",Json_from_DB.Mensaje.value);
	printf("	>>Horario: %s\n",Json_from_DB.Time_inicio_programado.value);
	printf("	>>Periodo: %d\n",Json_from_DB.Periodo_seconds.value);
	printf("	>>Ventana: %d\n",Json_from_DB.Ventana_minutes.value);
	printf("	>>Pass_to_WiFi: %d\n",Json_from_DB.Pass_to_WiFi.value);
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
void clear_firebase_pass_to_WiFi(void){
	/*Enviar un POST que modifique el valor de Pass_to_WiFi de FireBase*/
}





//Post f_a_seteada
void client_post_frecuencia_seteada(int f) {
    esp_http_client_config_t config_post = {
        .port = PORT,
        .url = "https://esp32-utnfrlp-2022-default-rtdb.firebaseio.com/frecuencia/frecuencia_seteada.json",
        .method = HTTP_METHOD_PUT,
        .username = USERNAME,
        .password = PASSWORD,
        .event_handler = client_event_get_frecuencia_seteada_handler
    };
    esp_http_client_handle_t client = esp_http_client_init(&config_post);
    char  post_data[3];
    snprintf(post_data, 3, "%d", f);    //Int to String
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

esp_err_t client_event_get_frecuencia_seteada_handler(esp_http_client_event_handle_t evt) {
    switch (evt->event_id) {
        case HTTP_EVENT_ON_DATA:
            printf("\n>> Client: HTTP_EVENT_ON_DATA: %.*s", evt->data_len, (char *)evt->data);
            break;
        default:
            break;
    }
    return ESP_OK;
}



