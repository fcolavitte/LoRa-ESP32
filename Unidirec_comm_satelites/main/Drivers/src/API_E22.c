/*
 * @file   : API_E22.c
 * @date   : Mar 11, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

#include "API_E22.h"
#include <API_Time.h>

/********************** inclusions *******************************************/

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** external functions declaration ***********************/

extern void HAL_SPI_write(uint8_t *SPI_message, uint8_t message_lenght);
extern void HAL_GPIO_write(uint8_t GPIO, uint8_t state);
extern void HAL_GPIO_read(uint8_t GPIO);

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

void _E22_write_command(uint8_t opcode, uint8_t *p_parameters, uint8_t n_parameters){
	/*PSEUDOCódigo: */
	//uint8_t *SPI_message
	//Spi_message[0] = opcode;
	//SPI_message += parameters[0:n-1];
	//HAL_SPI_write(SPI_message);	/*Declarar función en el E22_port_ESP32*/
}

/********************** external functions definition ************************/

void E22_setTx(uint32_t timeOut){
	uint8_t parameters[3];
	parameters[0] =(uint8_t)((timeOut >> 16) & 0xFF);
	parameters[1] =(uint8_t)((timeOut >> 8)  & 0xFF);
	parameters[2] =(uint8_t)((timeOut)       & 0xFF);

	uint8_t n_parameters = 3;

	_E22_write_command(OPCODE_SETTX, parameters, n_parameters);
}

void E22_reset(void){
	HAL_GPIO_write(GPIO_E22_RST, 0);
	vTaskDelay(10/ portTICK_PERIOD_MS);
	HAL_GPIO_write(GPIO_E22_RST, 1);
	vTaskDelay(10/ portTICK_PERIOD_MS);
}


/**
 *	@note	Con "payloadlength" se define la cantidad de bytes a mandar en modo Tx
 *			La función TxBaseAddr() define el inicio desde donde se comienza a transmitir o la función SetBufferBaseAddresses() que define también Rx
 */
void E22_SetPacketParams(void/* ??? */){

}

/**
 *	@note	El buffer de 256 bytes se borra al poner el E22 en modo SLEEP
 */
void E22_WriteBuffer(uint8_t offset, uint8_t *data_buffer, uint8_t data_lenght){
	uint8_t parameters[20];
	uint8_t n_parameters = 1+data_lenght;
	//parameters = 1 byte de offset + n bytes de data
	_E22_write_command(OPCODE_WRITEBUFFER, parameters, n_parameters);
}

void E22_ReadBuffer(uint8_t offset, uint8_t *data_buffer){
	uint8_t *parameters = &offset;
	uint8_t n_parameters = 1;
	_E22_write_command(OPCODE_SETTX, parameters, n_parameters);
	//Lo que devuelve, escribirlo en data_buffer
}

/**
 *	@note	Si el modulo E22 tiene el pin BUSY en bajo está preparado para recibir un comando.
 *			Si el pin busy está en alto se debe esperar para enviar un nuevo comando
 *			Al terminar de enviar un comando hay un retardo hasta que el pin busy se pone en alto para indicar que está procesando el comando
 *			El tiempo de retardo del BUSY es de hasta 126us, excepto cuando sale del modo SLEEP que puede tardar hasta 3,5ms
 */
bool E22_check_busy_pin_status(){
	return 1;
}

void E22_GetRxBufferStatus(){

}

/********************** end of file ******************************************/
