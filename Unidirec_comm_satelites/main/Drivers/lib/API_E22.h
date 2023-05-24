/*
 * @file   : API_E22.h
 * @date   : Mar 11, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

#ifndef MAIN_DRIVERS_LIB_API_E22_H_
#define MAIN_DRIVERS_LIB_API_E22_H_

/********************** inclusions *******************************************/

#include <stdint.h>
#include <stdbool.h>

/********************** macros and defines ***********************************/

#define INPUT	1
#define OUTPUT	0
#define HIGH	1
#define LOW		0

/*** OPCODES ***/
#define OPCODE_SETTX				(uint8_t)0x83
#define OPCODE_SETSTANDBY			(uint8_t)0x80
#define OPCODE_SETSLEEP				(uint8_t)0x84
#define OPCODE_WRITEREGISTER		(uint8_t)0x0D
#define OPCODE_READREGISTER			(uint8_t)0x1D
#define OPCODE_WRITEBUFFER			(uint8_t)0x0E
#define OPCODE_READBUFFER			(uint8_t)0x1E
#define OPCODE_GetRxBufferStatus	(uint8_t)0x13
#define OPCODE_SetBufferBaseAddress	(uint8_t)0x8F
#define OPCODE_SET_TX_MODE			(uint8_t)0x83
#define OPCODE_SET_RX_MODE			(uint8_t)0x82
#define OPCODE_SetPacketType		(uint8_t)0x8A	/* Cambia entre emisión LoRa y FSK - Usar antes que SetPacketParams		*/
#define OPCODE_SetPacketParams		(uint8_t)0x8C	/* Contiene un parámetro que define la cantidad de bytes a enviar		*/
#define OPCODE_SET_RF_FREC			(uint8_t)0x86
#define NOP 	(uint8_t)0x00

/*** GPIO ***/
#define GPIO_E22_BUSY 21
#define GPIO_E22_NRST  22
#define GPIO_TX_ENABLE -1
#define GPIO_RX_ENABLE -1

#define MAX_SIZE_SPI_BUFFERS	32

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

/**
 *	@brief	La función inicia el driver del E22 900M30S controlado por SPI
 */
void driver_E22_init(void);


/**
 *	@brief	Resetea el módulo E22 y vuelve a obtener los valores de configuración predeterminados.
 */
void driver_E22_reset(void);


/**
 *	@brief	Escribe una secuencia de caracteres en el buffer interno del E22.
 *	@param	[uint8_t] offset: Posición de inicio de escritura en el buffer circular de 256 bytes del E22.
 *	@param	[uint8_t *] tx_buffer: Posición del primer byte a escribir.
 *	@param	[uint8_t] tx_length: Cantidad de bytes a escribir en el buffer interno del E22.
 */
void driver_E22_write_in_buffer(uint8_t offset, uint8_t* tx_buffer, uint8_t tx_length);


/**
 *	@brief	Lee una secuencia de bytes desde el buffer interno del E22.
 *	@param	[uint8_t] offset: Posición de inicio de lectura desde el buffer circular de 256 bytes del E22.
 *	@param	[uint8_t *] rx_buffer: Posición de inicio donde guardar los valores leídos del buffer interno del E22.
 *	@param	[uint8_t] rx_length: Cantidad de bytes a leer desde el buffer interno del E22.
 */
void driver_E22_read_from_buffer(uint8_t offset, uint8_t* rx_buffer, uint8_t rx_length);




void driver_E22_write_in_registro(uint8_t *address, uint8_t* tx_buffer, uint8_t tx_length);
void driver_E22_read_from_registro(uint8_t *address, uint8_t* rx_buffer, uint8_t rx_length);
void driver_E22_GetRxBufferStatus(uint8_t* PayloadLengthRx, uint8_t* RxBufferPointer);
void driver_E22_SetBufferBaseAddress(uint8_t tx_base_adress, uint8_t rx_base_adress);

/* No contempla poner pin TX en alto */
void driver_E22_SetTx_poner_modulo_en_modo_tx(uint32_t timeout);

/* No contempla poner pin RX en alto */
void driver_E22_SetRx_poner_modulo_en_modo_rx(uint32_t timeout);
void driver_E22_SetPacketParams_con_modulo_en_modo_LoRa(uint16_t PreambleLength, bool Header_is_fixed_length, uint8_t bytes_a_enviar);
void driver_E22_SetPacketType(bool transmitir_en_modo_LoRa);
void driver_E22_SetRfFrequency(uint32_t frec_deseada_MHz);

void driver_E22_setear_pin_TX_salida_potencia(void);
void driver_E22_setear_pin_RX_entrada_aire(void);

#endif /* MAIN_DRIVERS_LIB_API_E22_H_ */

/********************** end of file ******************************************/
