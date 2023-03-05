/*
 * API_delay.c
 *
 *  Created on: 4 mar. 2023
 *      Author: Facu
 */

#include "../lib/API_Time.h"
#include "freertos/timers.h"

#define DEBUG 0
#define compile 1

uint8_t fecha_actual[11];
uint32_t segundos_actuales=0;

TimerHandle_t xTimer;



void get_UTP(void) {
	printf(">>Iniciando solicitud de hora UTC...\n");
    esp_http_client_config_t config_get = {
        .port = 443,
        .url = "http://worldtimeapi.org/api/timezone/America/Argentina/Buenos_Aires",
        .method = HTTP_METHOD_GET,
        .event_handler = client_event_UTP_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config_get);
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

esp_err_t client_event_UTP_handler(esp_http_client_event_handle_t evt) {
    if(evt->event_id==HTTP_EVENT_ON_DATA) {
    	segundos_actuales=0;
#if DEBUG == 1
        printf(">> Respuesta del servidor UTP: %.*s\n", evt->data_len,(char *)evt->data);
#endif
        uint8_t *indexIn;
        uint8_t time[9] = {};	/*HH:MM:SS\0*/
        indexIn=(uint8_t*)strstr((char *)evt->data, "\"datetime\":\"");
        indexIn+=12;	/*Posicionarse al final de "\"datetime\":\""*/
        strncpy((char *)fecha_actual,(char *)indexIn,10);
        indexIn+=11;	/*Posicionarse donde comienza la hora*/
        strncpy((char *)time,(char *)indexIn,8);
        uint8_t _hora[3]={};
        strncpy((char *)_hora,(char *)time,2);
        uint8_t _min[3]={};
        strncpy((char *)_min,(char *)time+3,2);
        uint8_t _sec[3]={};
        strncpy((char *)_sec,(char *)time+6,2);
        segundos_actuales = (uint32_t)atoi((char *)_hora)*3600+(uint32_t)atoi((char *)_min)*60+(uint32_t)atoi((char *)_sec);
        printf(">> Fecha desde servidor UTP: %.*s (AAAA:MM:DD)\n", 10, (char *)fecha_actual);
        printf(">> Hora desde servidor UTP: %.*s\n", 8, (char *)time);
        printf(">> Segundos desde las 00:00: %lu\n", segundos_actuales);
    }
    return ESP_OK;
}

void vTimerCallback(TimerHandle_t pxTimer){
#if DEBUG == 1
	printf(">>Mensaje desde dentro del Timer\n");
#endif
	segundos_actuales++;
	if(segundos_actuales>86400){
		printf(">>Solicitando UTP por desvorde de segundos\n");
		get_UTP();
	}
}

void UTP_init(void){
	get_UTP();
	printf(">>Iniciando Timer\n");
	xTimer = xTimerCreate("Timer",100,pdTRUE,(void *)1, vTimerCallback);
	xTimerStart(xTimer,0);
}

#if compile == 1

/*
 * @brief	Inicializa una estructura delay
 * @param	Puntero a estructura delay
 * @param	Duración del retardo no bloqueante en segundos
 * @return	None
 */
void delayInit(delay_t * delay, uint32_t duration) {
	if(delay != 0 && duration > 0 && duration < MAX_DELAY){	//Comprobación de parámetros de entrada
		delay->startTime = segundos_actuales;
		delay->duration = duration;
		delay->running = 0;//false
	} else {
		//Control de error
	}

}

/*
 * @brief	Actualización y lectura del estado del delay. Reinicia el mismo si este ya finalizó
 * @param	Puntero a estructura delay
 * @return	True si terminó retardo, sino False
 */
bool_t delayRead(delay_t * delay) {
	if(delay!=0) {	//Comprobación de parámetro de entrada
		if(delay->running==0) {
			if(segundos_actuales - delay->startTime > delay->duration){
				delay->running=1;//true
			}
		} else {
			delay->running=0;
			delay->startTime=segundos_actuales;
		}
		return delay->running;
	} else {
		//Control de error
		return 0;
	}
}


/*
 * @brief	Cambio del valor de retardo del delay
 * @param	Puntero a estructura delay
 * @param	Nueva duración del retardo no bloqueante en segundos
 * @return	None
 */
void delayWrite(delay_t * delay, uint32_t duration) {
	if(delay != NULL && duration > 0 && duration < MAX_DELAY) {
		delay->duration=duration;
	} else {
		//Control de error
	}

}


#endif
