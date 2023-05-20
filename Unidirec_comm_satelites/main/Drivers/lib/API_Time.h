/*
 * @file   : API_delay.h
 * @date   : Mar 04, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */
#ifndef MAIN_DRIVERS_LIB_API_TIME_H_
#define MAIN_DRIVERS_LIB_API_TIME_H_

/********************** inclusions *******************************************/

#include "stdint.h"
#include "esp_http_client.h"

/********************** macros and definitions *******************************/

#define MAX_DELAY 120	//2 minutos. Variable en segundos

/********************** typedef **********************************************/

typedef uint8_t bool_t;

typedef struct{
	uint32_t startTime;
	uint32_t duration;
	bool_t running;
} delay_t;

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

void delayInit( delay_t *delay, uint32_t duration );
bool_t delayRead( delay_t *delay );
void delayWrite( delay_t *delay, uint32_t duration );
void get_UTP(void);
esp_err_t client_event_UTP_handler(esp_http_client_event_handle_t evt);
void UTP_init(void);

#endif /* MAIN_DRIVERS_LIB_API_TIME_H_ */

/********************** end of file ******************************************/
