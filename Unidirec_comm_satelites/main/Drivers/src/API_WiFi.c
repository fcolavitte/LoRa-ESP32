/*
 * @file   : API_WiFi.c
 * @date   : Feb 08, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include "API_WiFi.h"
#include <esp_wifi.h>
#include <stdint.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_partition.h>
#include <stdint.h>
#include <string.h>

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/


static uint8_t mac[6]; 	/* Variable donde se lee la MAC del WiFi*//*Por defecto la MAC es 30:C6:F7:29:BA:D8*/

/********************** internal functions declaration ***********************/

void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

/********************** internal data definition *****************************/

uint8_t SSID[32] = "Carina 2.4GHz";
uint8_t PASS[32] = "0142563815";
uint8_t contador_WiFi_conexiones_fallidas=0;

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/**
 * @brief	Función Callback de eventos Wi-Fi
 */
void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    switch (event_id) {
        case WIFI_EVENT_STA_START:
            printf(">> Conectando WiFi... \n");
            break;
        case WIFI_EVENT_STA_CONNECTED:
            printf(">> WiFi conectado. Esperando IP... \n");
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            printf(">> Se perdión la conexión WiFi. Intentando reconexión... \n");
            esp_wifi_connect();
            contador_WiFi_conexiones_fallidas++;
            vTaskDelay(500/ portTICK_PERIOD_MS);
            if (16 <= contador_WiFi_conexiones_fallidas) {
            	reconectar_WiFi();
            	contador_WiFi_conexiones_fallidas = 0;
            }
            break;
        case IP_EVENT_STA_GOT_IP:
            printf(">> El dispositivo obtuvo IP WiFi. \n\n");
            contador_WiFi_conexiones_fallidas=0;
            break;
        default:
            break;
    }
}

/********************** external functions definition ************************/

void WiFiConect(void) {
	nvs_flash_init();					 /* Iniciar nvs non-volatile storage */
	WiFi_get_SSID_and_PASS_from_nvs();
	esp_netif_init();                    /* TCP/IP initiation */
	esp_event_loop_create_default();     /* event loop */
	esp_netif_create_default_wifi_sta(); /* WiFi station */
	wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&wifi_initiation);
	esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
	esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
	esp_wifi_set_mode(WIFI_MODE_STA);
	wifi_config_t wifi_configuration = {};
	strncpy((char*)wifi_configuration.sta.ssid, (char*)SSID, 32);
	strncpy((char*)wifi_configuration.sta.password, (char*)PASS, 32);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_start();
    esp_wifi_connect();
}


void print_MAC(void){
    esp_wifi_get_mac(WIFI_IF_STA, &mac[0]);
    printf("\n>> MAC: ");
    for(int i=0;i<6;i++){
        uint8_t char_izq = (mac[i]/16)+'0';
        uint8_t char_der = (mac[i]%16)+'0';
        if(char_izq>'9'){char_izq=char_izq+('A'-'0')-10;}
        if(char_der>'9'){char_der=char_der+('A'-'0')-10;}
        printf("%c%c",char_izq,char_der);
        if(i!=5){printf(":");}
        else{printf("\n");}
    }
}




void reconectar_WiFi(void) {
	esp_wifi_disconnect();
	esp_wifi_stop();
	WiFi_get_SSID_and_PASS_from_nvs();
	esp_wifi_set_mode(WIFI_MODE_STA);
	wifi_config_t wifi_configuration = {};
	strncpy((char*)wifi_configuration.sta.ssid, (char*)SSID, 32);
	strncpy((char*)wifi_configuration.sta.password, (char*)PASS, 32);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_start();
    esp_wifi_connect();
}



void WiFi_set_SSID(uint8_t *new_SSID) {
	nvs_handle_t nvs_handler;
	printf("\nNuevo SSID guardado en nvs\n");
	nvs_open("storage", NVS_READWRITE, &nvs_handler);
	nvs_set_str(nvs_handler, "SSID", (char*)new_SSID);
	if(nvs_commit(nvs_handler) != ESP_OK){
		printf("nvs commit falló.\n");
	}
	nvs_close(nvs_handler);
	WiFi_get_SSID_and_PASS_from_nvs();
	printf("SSID: %s\n", SSID);
}

void WiFi_set_PASS(uint8_t *new_PASS) {
	nvs_handle_t nvs_handler;
	printf("\nNuevo PASS guardado en nvs\n");
	nvs_open("storage", NVS_READWRITE, &nvs_handler);
	nvs_set_str(nvs_handler, "PASS", (char*)new_PASS);
	if(nvs_commit(nvs_handler) != ESP_OK){
		printf("nvs commit falló.\n");
	}
	nvs_close(nvs_handler);
	WiFi_get_SSID_and_PASS_from_nvs();
	/* printf("PASS: %s\n", PASS); */
}


void WiFi_get_SSID_and_PASS_from_nvs(void) {
	nvs_handle_t nvs_handler;
	uint8_t string_readed[32];
	esp_err_t err;
	size_t length = 31;
	/* SSID */
	nvs_open("storage", NVS_READWRITE, &nvs_handler);
	err = nvs_get_str(nvs_handler, "SSID", (char*)string_readed, &length);
	if (ESP_OK == err) {
		string_readed[31] = '\0';
		printf("\n>> SSID: %s\n", string_readed);
		strcpy((char*)SSID, (char*)string_readed);
	} else {

	}
	nvs_close(nvs_handler);

	/* PASS */
	nvs_open("storage", NVS_READWRITE, &nvs_handler);
	err = nvs_get_str(nvs_handler, "PASS", (char*)string_readed, &length);
	if (ESP_OK == err) {
		string_readed[31] = '\0';
		/* printf("\n>> PASS: %s\n", string_readed); */
		strcpy((char*)PASS, (char*)string_readed);
	}
	nvs_close(nvs_handler);
}


uint8_t* get_SSID_pointer(void) {
	return SSID;
}

/********************** end of file ******************************************/
