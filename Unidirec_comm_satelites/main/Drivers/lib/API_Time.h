/*
 * @file   : API_delay.h
 * @date   : Mar 04, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */
#ifndef MAIN_DRIVERS_LIB_API_TIME_H_
#define MAIN_DRIVERS_LIB_API_TIME_H_

/********************** inclusions *******************************************/

#include <stdint.h>
#include <esp_http_client.h>

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

/**
 * @brief	Inicializa una estructura delay
 * @param	Puntero a estructura delay
 * @param	Duración del retardo no bloqueante en segundos
 * @return	None
 */
void delayInit(delay_t *delay, uint32_t duration);

/**
 * @brief	Actualización y lectura del estado del delay. Reinicia el mismo si este ya finalizó
 * @param	Puntero a estructura delay
 * @return	True si terminó retardo, sino False
 */
bool_t delayRead(delay_t *delay);

/**
 * @brief	Cambio del valor de retardo del delay
 * @param	Puntero a estructura delay
 * @param	Nueva duración del retardo no bloqueante en segundos
 * @return	None
 */
void delayWrite(delay_t *delay, uint32_t duration);

/**
 * @brief	Solicitar hora actual a servidor UTP (Network Time Protocol)
 */
void get_UTP(void);


/**
 * @brief	Iniciar la función para control de hora global del dispositivo.
 */
void UTP_init(void);

/**
 * @brief	Función callback del servidor UTP (Network Time Protocol)
 */
esp_err_t client_event_UTP_handler(esp_http_client_event_handle_t evt);

#endif /* MAIN_DRIVERS_LIB_API_TIME_H_ */

/********************** end of file ******************************************/
