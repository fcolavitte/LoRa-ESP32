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


/**
 * @brief	Función getter paraobtener la fecha actual en string en formato DD/MM/AAAA
 */
uint8_t* get_string_fecha(void);


/**
 * @brief	Función getter paraobtener la hora actual en string en formato HH:MM:SS
 */
uint8_t* get_string_hora(void);


/**
 * @brief	Permite actualizar la hora del sistema (del RTC) estableciendola manualmente
 * @param	Recive una cadena de caracteres de la forma "HH_MM_SS", por ejemplo "16:30:05"
 * @note	Actualiza la hora pero no modifica la fecha, para eso usar set_fecha_system_manualmente()
 */
void set_hora_system_manualmente(uint8_t *HH_MM_SS);


/**
 * @brief	Permite actualizar la fecha del sistema (del RTC) estableciendola manualmente
 * @param	Recive una cadena de caracteres de la forma "DD_MM_AAAA", por ejemplo "14/06/2023"
 * @note	Actualiza la fecha pero no modifica la hora, para eso usar set_hora_system_manualmente()
 * @note	Respetar que el año esté compuesto de cuatro caracteres y no solo los últimos dos.
 */
void set_fecha_system_manualmente(uint8_t *DD_MM_AAAA);

#endif /* MAIN_DRIVERS_LIB_API_TIME_H_ */

/********************** end of file ******************************************/
