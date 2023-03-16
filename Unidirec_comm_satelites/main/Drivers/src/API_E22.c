/*
 * API_E22.c
 *
 *  Created on: 11 mar. 2023
 *      Author: Facu
 */
#include "API_E22.h"
#include <API_Time.h>

extern void HAL_SPI_write(uint8_t *SPI_message, uint8_t message_lenght);
extern void HAL_GPIO_write(uint8_t GPIO, uint8_t state);
extern void HAL_GPIO_read(uint8_t GPIO);



void _E22_write_command(uint8_t opcode, uint8_t *p_parameters, uint8_t n_parameters){
	/*PSEUDOCódigo: */
	//uint8_t *SPI_message
	//Spi_message[0] = opcode;
	//SPI_message += parameters[0:n-1];
	//HAL_SPI_write(SPI_message);	/*Declarar función en el E22_port_ESP32*/
}

void E22_setTx(uint32_t timeOut){
	uint8_t parameters[3];
	parameters[0] =(uint8_t)((timeOut >> 16) & 0xFF);
	parameters[1] =(uint8_t)((timeOut >> 8)  & 0xFF);
	parameters[2] =(uint8_t)((timeOut)       & 0xFF);

	uint8_t n_parameters = 3;

	_E22_write_command(OPCODE_SETTX,parameters,n_parameters);
}

void E22_reset(void){
	HAL_GPIO_write(GPIO_E22_RST, 0);
	vTaskDelay(10/ portTICK_PERIOD_MS);
	HAL_GPIO_write(GPIO_E22_RST, 1);
	vTaskDelay(10/ portTICK_PERIOD_MS);
}

void E22_SetPacketParams(void/* ??? */){

}
