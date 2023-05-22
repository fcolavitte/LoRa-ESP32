/*
 * @file   : API_E22.c
 * @date   : Mar 11, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

#include "API_E22.h"
#include <stdint.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>

/********************** inclusions *******************************************/

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** external functions declaration ***********************/

extern void driver_HAL_spi_init(void);
extern uint8_t driver_HAL_transaction(uint8_t *tx_buffer, uint8_t tx_length, uint8_t *rx_buffer, uint8_t rx_length);
extern void driver_HAL_gpio_init(uint8_t GPIO_num, bool_t GPIO_is_input);
extern void driver_HAL_GPIO_write(uint8_t GPIO, bool_t state);
extern bool_t driver_HAL_GPIO_read(uint8_t GPIO);

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

void driver_E22_init(void) {
	driver_HAL_spi_init();
	driver_HAL_gpio_init(GPIO_E22_BUSY, INPUT);
	driver_HAL_gpio_init(GPIO_E22_NRST, OUTPUT);
	driver_HAL_gpio_init(GPIO_TX_ENABLE, OUTPUT);
	driver_HAL_gpio_init(GPIO_RX_ENABLE, OUTPUT);

	/* Cargar estados iniciales de los pines */
	driver_HAL_GPIO_write(GPIO_E22_NRST, HIGH);
	driver_HAL_GPIO_write(GPIO_TX_ENABLE, LOW);
	driver_HAL_GPIO_write(GPIO_RX_ENABLE, LOW);
}


void driver_E22_reset(void) {
	driver_HAL_GPIO_write(GPIO_E22_NRST, LOW);
	vTaskDelay(10/ portTICK_PERIOD_MS);
	driver_HAL_GPIO_write(GPIO_E22_NRST, HIGH);
	vTaskDelay(10/ portTICK_PERIOD_MS);
}


void driver_E22_write_in_buffer(uint8_t offset, uint8_t* tx_buffer, uint8_t tx_length) {
	if(MAX_SIZE_SPI_BUFFERS - 2 > tx_length) {
		tx_length = MAX_SIZE_SPI_BUFFERS - 2;
	}
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	uint8_t MOSI_buffer[MAX_SIZE_SPI_BUFFERS] = {};
	MOSI_buffer[0] = OPCODE_WRITEBUFFER;
	MOSI_buffer[1] = offset;
	memcpy(&(MOSI_buffer[2]), tx_buffer, tx_length);
	driver_HAL_transaction(MOSI_buffer, tx_length, NULL, 0);
}


void driver_E22_read_from_buffer(uint8_t offset, uint8_t* rx_buffer, uint8_t rx_length) {
	if(MAX_SIZE_SPI_BUFFERS - 3 > rx_length) {
		rx_length = MAX_SIZE_SPI_BUFFERS - 3;
	}
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	uint8_t MOSI_buffer[MAX_SIZE_SPI_BUFFERS] = {};
	MOSI_buffer[0] = OPCODE_READBUFFER;
	MOSI_buffer[1] = offset;
	MOSI_buffer[2] = NOP;
	driver_HAL_transaction(MOSI_buffer, 3, rx_buffer, rx_length);
}













void driver_E22_setTx(uint32_t timeOut){
	uint8_t parameters[3];
	parameters[0] =(uint8_t)((timeOut >> 16) & 0xFF);
	parameters[1] =(uint8_t)((timeOut >> 8)  & 0xFF);
	parameters[2] =(uint8_t)((timeOut)       & 0xFF);

	uint8_t n_parameters = 3;

	_E22_write_command(OPCODE_SETTX, parameters, n_parameters);
}

/**
 *	@note	Con "payloadlength" se define la cantidad de bytes a mandar en modo Tx
 *			La función TxBaseAddr() define el inicio desde donde se comienza a transmitir o la función SetBufferBaseAddresses() que define también Rx
 */
void driver_E22_SetPacketParams(void/* ??? */){

}

void driver_E22_GetRxBufferStatus(){

}

/********************** end of file ******************************************/
