/*
 * @file   : API_WiFi.h
 * @date   : Feb 08, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

#ifndef MAIN_DRIVERS_LIB_API_WIFI_H_
#define MAIN_DRIVERS_LIB_API_WIFI_H_

/********************** inclusions *******************************************/

#include <stdint.h>
#include <esp_wifi.h>

/********************** macros and definitions *******************************/

/*
#define SSID "mastropiero"
#define PASS "Elchavodelocho"
*/
#define SSID "Carina 2.4GHz"
#define PASS "0142563815"

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

/**
 * @brief Conecta el dispositivo al Wi-Fi con el SSID y PASS usados en el define
 */
void WiFiConect(void);

/**
 * @brief Imprime la MAC por serial en una línea de código de la forma ">> MAC: XX.XX.XX.XX.XX.XX"
 */
void print_MAC(void);

#endif /* MAIN_DRIVERS_LIB_API_WIFI_H_ */

/********************** end of file ******************************************/
