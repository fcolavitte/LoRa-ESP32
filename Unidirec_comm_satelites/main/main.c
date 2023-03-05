/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <API_Time.h>
#include "main.h"


void app_main(void) {
	star_MEF();
	while(1){
		update_MEF();

        vTaskDelay(10/ portTICK_PERIOD_MS);
	}
}

