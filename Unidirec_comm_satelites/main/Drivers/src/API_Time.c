/*
 * @file   : API_Time.c
 * @date   : Mar 04, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include "API_Time.h"
#include <stdint.h>
#include <freertos/timers.h>
#include <time.h>
#include <string.h>

/********************** macros and definitions *******************************/

#define DEBUG 0
#define USE_RTC_INTERNO 1

/********************** internal data declaration ****************************/

uint8_t fecha_actual[11];
TimerHandle_t xTimer;
uint8_t retvalue_string_fecha[11]={};
uint8_t retvalue_string_hora[9]={};

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

uint32_t segundos_actuales=0;
struct timeval time_now;

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/**
 * @brief	Devuelve el tiempo UTP en unix (segundos desde 1900) del sistema.
 * @note	Para cargar el tiempo del sistema se tiene que usar get_UTP para que se cargue vía
 * 			web automáticamente desde servidor NTC o manualmente.
 * @note2	El tiempo en unix devuelto es para la zona horaria Argentina (-10800 (-3hs))
 */
time_t get_time_unix_system (void) {
	gettimeofday(&time_now, NULL);
	time_t raw_offset = -10800;
	time_t unixtime_now = time_now.tv_sec + raw_offset;
	return unixtime_now;
}

/********************** external functions definition ************************/

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
#if DEBUG == 1
        printf(">> Respuesta del servidor UTP: %.*s\n", evt->data_len,(char *)evt->data);
#endif
#if USE_RTC_INTERNO == 0
        segundos_actuales=0;
        uint8_t *indexIn;
        uint8_t _time[9] = {};	/* HH:MM:SS\0 */
        indexIn=(uint8_t*)strstr((char *)evt->data, "\"datetime\":\"");
        indexIn+=12;	/* Posicionarse al final de "\"datetime\":\"" */
        strncpy((char *)fecha_actual,(char *)indexIn,10);
        indexIn+=11;	/* Posicionarse donde comienza la hora */
        strncpy((char *)_time,(char *)indexIn,8);
        uint8_t _hora[3]={};
        strncpy((char *)_hora,(char *)_time,2);
        uint8_t _min[3]={};
        strncpy((char *)_min,(char *)_time+3,2);
        uint8_t _sec[3]={};
        strncpy((char *)_sec,(char *)_time+6,2);
        segundos_actuales = (uint32_t)atoi((char *)_hora)*3600+(uint32_t)atoi((char *)_min)*60+(uint32_t)atoi((char *)_sec);
        printf(">> Fecha desde servidor UTP: %.*s (AAAA:MM:DD)\n", 10, (char *)fecha_actual);
        printf(">> Hora desde servidor UTP: %.*s\n", 8, (char *)_time);
        printf(">> Segundos desde las 00:00: %lu\n", segundos_actuales);
#endif
#if USE_RTC_INTERNO == 1
    	uint8_t *indexIn;
    	time_t unixtime = 0;
    	uint8_t string_unixtime[11] = {};
    	indexIn=(uint8_t*)strstr((char *)evt->data, "\"unixtime\"");
    	if(indexIn>0){
            indexIn+=11;
        	strncpy((char *)string_unixtime,(char *)indexIn,10);
        	string_unixtime[10]='\0';
        	unixtime = atoi((void *)string_unixtime);
        	struct timeval start_time = { .tv_sec = unixtime, .tv_usec=0 };
        	settimeofday(&start_time, NULL);

        	time_t unixtime_now = get_time_unix_system();
        	struct tm* struct_tm_now;
        	struct_tm_now = localtime(&unixtime_now);
        	printf(">> Fecha desde servidor UTP: %d/%d/%d\n", struct_tm_now->tm_mday, struct_tm_now->tm_mon+1, struct_tm_now->tm_year+1900);
        	printf(">> Hora desde servidor UTP: %d:%d:%d\n", struct_tm_now->tm_hour, struct_tm_now->tm_min, struct_tm_now->tm_sec);
    	}
#endif
    }
    return ESP_OK;
}


uint8_t* get_string_fecha(void){
	time_t unixtime_now = get_time_unix_system();
	struct tm* struct_tm_now;
	struct_tm_now = localtime(&unixtime_now);
	retvalue_string_fecha[9] = '\0';
	sprintf((char *)retvalue_string_fecha, "%d/%d/%d", struct_tm_now->tm_mday, struct_tm_now->tm_mon+1, struct_tm_now->tm_year+1900);
	retvalue_string_fecha[10] = '\0';
	return retvalue_string_fecha;
}


uint8_t* get_string_hora(void){
	time_t unixtime_now = get_time_unix_system();
	struct tm* struct_tm_now;
	struct_tm_now = localtime(&unixtime_now);
	retvalue_string_hora[7] = '\0';
	sprintf((char *)retvalue_string_hora, "%2d:%2d:%2d", struct_tm_now->tm_hour, struct_tm_now->tm_min, struct_tm_now->tm_sec);
	retvalue_string_hora[8] = '\0';
	return retvalue_string_hora;
}


#if USE_RTC_INTERNO == 0
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
#endif

void UTP_init(void){
	get_UTP();
#if USE_RTC_INTERNO == 0
	printf(">>Iniciando Timer\n");
	xTimer = xTimerCreate("Timer",100,pdTRUE,(void *)1, vTimerCallback);
	xTimerStart(xTimer,0);
#endif
}


/*********** Retardos no bloqueantes ************/

void delayInit(delay_t * delay, uint32_t duration) {
	if(delay != 0 && duration > 0 && duration < MAX_DELAY){		/* Comprobación de parámetros de entrada */
		delay->startTime = segundos_actuales;
		delay->duration = duration;
		delay->running = 0;	/* false */
	} else {
		/* Control de error */
	}

}


bool_t delayRead(delay_t * delay) {
	if(delay!=0) {	/* Comprobación de parámetro de entrada */
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
		/* Control de error */
		return 0;
	}
}


void delayWrite(delay_t * delay, uint32_t duration) {
	if(delay != NULL && duration > 0 && duration < MAX_DELAY) {
		delay->duration=duration;
	} else {
		//Control de error
	}

}

/********************** end of file ******************************************/
