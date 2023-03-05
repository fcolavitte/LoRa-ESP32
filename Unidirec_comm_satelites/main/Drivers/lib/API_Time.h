/*
 * API_delay.h
 *
 *  Created on: 4 mar. 2023
 *      Author: Facu
 */

#ifndef MAIN_DRIVERS_LIB_API_TIME_H_
#define MAIN_DRIVERS_LIB_API_TIME_H_

#include "stdint.h"
#include "esp_http_client.h"


#define MAX_DELAY 120	//2 minutos. Variable en segundos

//--------- typedefs -----------
typedef uint8_t bool_t;

typedef struct{
	uint32_t startTime;
	uint32_t duration;
	bool_t running;
} delay_t;

//--------- Global Variables ---------



//--------- Functions -----------
void delayInit( delay_t *delay, uint32_t duration );
bool_t delayRead( delay_t *delay );
void delayWrite( delay_t *delay, uint32_t duration );
void get_UTP(void);
esp_err_t client_event_UTP_handler(esp_http_client_event_handle_t evt);
void UTP_init(void);


#endif /* MAIN_DRIVERS_LIB_API_TIME_H_ */
