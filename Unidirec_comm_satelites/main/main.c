/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "API_WiFi.h"
#include "API_Firebase.h"
#include "API_USB.h"
#include "main.h"



extern void display_menu(void);
extern void testJson(void);


void app_main(void) {
	WiFiConect();
	//Verificar WiFi
	vTaskDelay(5000/ portTICK_PERIOD_MS);
	print_MAC();
	printf("\n\n");
	star_MEF();
	display_menu();

	while(1){


		client_get_Json();

		//USB_get_input();
        //vTaskDelay(10/ portTICK_PERIOD_MS);

        //testJson();
        vTaskDelay(5000/ portTICK_PERIOD_MS);

	}
}
