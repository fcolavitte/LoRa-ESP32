/*
 * API_WiFi.c
 *
 *  Created on: 8 feb. 2023
 *      Author: Facundo
 */

#include "API_WiFi.h"

uint8_t mac[6]; 	/*Variable donde se lee la MAC del WiFi*//*Por defecto la MAC es 30:C6:F7:29:BA:D8*/

void WiFiConect(void) {
	nvs_flash_init();
    esp_netif_init();                    // TCP/IP initiation
    esp_event_loop_create_default();     // event loop
    esp_netif_create_default_wifi_sta(); // WiFi station
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    esp_wifi_set_mode(WIFI_MODE_STA);
    wifi_config_t wifi_configuration = {
        .sta = {
        .ssid = SSID,
        .password = PASS}
    };
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_start();
    esp_wifi_connect();
}

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
            vTaskDelay(1000/ portTICK_PERIOD_MS);
            break;
        case IP_EVENT_STA_GOT_IP:
            printf(">> El dispositivo obtuvo IP WiFi. \n\n");
            break;
        default:
            break;
    }
}


/**
 * @brief Imprime la MAC por serial en una línea de código de la forma ">> MAC: XX.XX.XX.XX.XX.XX"
 */
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
