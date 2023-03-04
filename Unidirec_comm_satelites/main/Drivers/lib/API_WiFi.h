/*
 * API_WiFi.h
 *
 *  Created on: 8 feb. 2023
 *      Author: Facundo
 */

#ifndef MAIN_DRIVERS_LIB_API_WIFI_H_
#define MAIN_DRIVERS_LIB_API_WIFI_H_


#include "esp_wifi.h"
#include "nvs_flash.h"
#include "stdint.h"

/*
#define SSID "mastropiero"
#define PASS "Elchavodelocho"
*/
#define SSID "Carina 2.4GHz"
#define PASS "0142563815"


void WiFiConect(void);

void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

/**
 * @brief Imprime la MAC por serial en una línea de código de la forma ">> MAC: XX.XX.XX.XX.XX.XX"
 */
void print_MAC(void);




#endif /* MAIN_DRIVERS_LIB_API_WIFI_H_ */
