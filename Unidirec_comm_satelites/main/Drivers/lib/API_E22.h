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
#define OPCODE_SetModulationParams	(uint8_t)0x8B
#define OPCODE_SetPacketParams		(uint8_t)0x8C	/* Contiene un parámetro que define la cantidad de bytes a enviar		*/
#define OPCODE_SetTxParams			(uint8_t)0x8E
#define OPCODE_SET_RF_FREC			(uint8_t)0x86
#define OPCODE_CalibrateFunction    (uint8_t)0x89
#define OPCODE_SetPaConfig			(uint8_t)0x95
#define OPCODE_SetDIO3asTCXOCtrl    (uint8_t)0x97
#define OPCODE_CalibrateImage       (uint8_t)0x98
#define ADDRES_LORA_SYNC_WORD_MSB	(uint16_t)0x0740
#define ADDRES_TX_MODULATION    	(uint16_t)0x0889
#define ADDRES_IQ_POLARITY_SETUP  	(uint16_t)0x0736
#define NOP 	(uint8_t)0x00

/*** GPIO ***/
#define GPIO_E22_BUSY 21
#define GPIO_E22_NRST  22
#define GPIO_TX_ENABLE 26
#define GPIO_RX_ENABLE 25

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



/**
 *	@brief	Escribe en un registro interno del E22
 *	@param	[uint8_t *] address: Dirección del registro a escribir
 *	@param	[uint8_t *] tx_buffer: Puntero al contenido a escribir en el registro
 *	@param	[uint8_t] tx_length: Longitud en bytes a escribir en el registro
 */
void driver_E22_write_in_registro(uint8_t *address, uint8_t* tx_buffer, uint8_t tx_length);

/**
 *	@brief	Lee desde un registro interno del E22
 *	@param	[uint8_t *] address: Dirección del registro a leer
 *	@param	[uint8_t *] rx_buffer: Puntero donde guardar lo que se lee del registro
 *	@param	[uint8_t] rx_length: Longitud en bytes a leer desde el registro
 */
void driver_E22_read_from_registro(uint8_t *address, uint8_t* rx_buffer, uint8_t rx_length);

/**
 *	@brief	Póne el módulo en modo Stand By
 */
void driver_E22_set_standBy(void);

/**
 *	@brief	Recupera la cantidad de bytes recibidos por aire y la posición del
 *			buffer circular interno del E22 donde comienza
 *	@param [uint8_t *] PayloadLengthRx: Posición de memoria donde guardar el largo en bytes recibidos
 *	@param [uint8_t *] RxBufferPointer: Posición de memoria donde guardar el offset donde comienza
 *										el mensaje recibido
 */
void driver_E22_GetRxBufferStatus(uint8_t* PayloadLengthRx, uint8_t* RxBufferPointer);

/**
 *	@brief	Setea las posiciones de inicio de Rx y Tx del E22 en su buffer circular interno
 *	@param [uint8_t] tx_base_adress: offset del Tx. Posición en la que se comienza a emitir el mensaje.
 *	@param [uint8_t] rx_base_adress: offset del Rx. Posición en la que se comienza a guardar el mensaje recibido.
 */
void driver_E22_SetBufferBaseAddress(uint8_t tx_base_adress, uint8_t rx_base_adress);

/**
 *	@brief	Configura el SX1262 del E22 en modo Tx
 *	@note	No contempla poner pin TX en alto necesario para que el módulo E22 emita
 */
void driver_E22_SetTx_poner_modulo_en_modo_tx(uint32_t timeout);

/**
 *	@brief	Configura el SX1262 del E22 en modo Rx
 *	@note	No contempla poner pin RX en alto necesario para que el módulo E22 reciba
 */
void driver_E22_SetRx_poner_modulo_en_modo_rx(uint32_t timeout);

/**
 *	@brief Segun los parámetros recibidos se configura para el SX1262 o 1261
 *	@param [uint8_t] paDutyCycle: valor por defecto 0x02.
 *	@param [uint8_t] hpMax: valor por defecto 0x03.
 *	@param [uint8_t] deviceSe: Debe ser 0.
 *  @note  Para el SX1262 que tiene el E22-900M30S los parámetros deben ser (0x02,0x03,0)
 */
void driver_E22_SetPaConfig(uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSe);

/**
 *	@brief Establece la potencia de salida y tiempo de rampa del dispositivo emisor
 *	@param [uint8_t] power: valor por defecto 0x16.
 *	@param [uint8_t] rampTime: valor por defecto 0x05.
 */
void driver_E22_SetTxParams(uint8_t power, uint8_t rampTime);

/**
 *	@brief  Configura los parametros de comunicación, deben coincidir en el módulo Tx y Rx
 *	@param [uint8_t] SpringFactor: Valor por defecto 7. Puede llevarse hasta 10 para visualizar en SDR.
 *	@param [uint8_t] BandWidth: Valor por defecto 4 (31,25 kHz).
 *	@param [uint8_t] CodingRate: Valor por defecto 1.
 *	@param [uint8_t] LowDataRateOptimization: Valor por defecto 0.
 *                   Para paquetedes de larga duración se recomienda que esté en 1.
 */
void driver_E22_SetModulationParams(uint8_t SF, uint8_t BW, uint8_t CR, uint8_t LDRO);

/**
 *	@brief  Corrige el registro 0x0889 si tiene un valor erroneo tras reiniciar el dispositivo.
 */
void driver_E22_fix_modulation_quality(uint8_t BW);

void driver_E22_SetSyncWord(uint16_t sync);

/**
 *	@brief	Configura el dataframe de la comunicación LoRa
 *	@note	El parámetro más importante es "bytes_a_enviar". En modo Rx define la cantidad máxima de bytes a recibir por mensaje.
 */
void driver_E22_SetPacketParams_con_modulo_en_modo_LoRa(uint16_t PreambleLength, bool Header_is_fixed_length, uint8_t bytes_a_enviar);

/**
 *	@brief  Corrige el registro 0x0736 si tiene un valor erroneo tras reiniciar el dispositivo.
 */
void driver_E22_fix_invertedIQ_register(bool is_standard_IQ);

/**
 *	@brief	Setea el modo de transmisión del E22. Puede ser LoRa o FSK.
 *	@note	Se recomienda que el parametro sea 1 (emitir en modo LoRa)
 */
void driver_E22_SetPacketType(bool transmitir_en_modo_LoRa);

/**
 *	@brief	Setea la frecuencia de la portadora
 */
void driver_E22_SetRfFrequency(uint32_t frec_deseada_MHz);

/**
 *	@brief	Pone en 1 o 0 el estado del pin Tx del módulo E22 para habilitar
 *			la salida de potencia a la antena.
 */
void driver_E22_setear_pin_TX_salida_potencia(bool estado);

/**
 *	@brief	Pone en 1 o 0 el estado del pin Rx del módulo E22 para habilitar
 *			la entrada de la señal desde la antena.
 */
void driver_E22_setear_pin_RX_entrada_aire(bool estado);


/**
 * @brief	Muestra por terminal serie la configuración del módulo E22
 */
void driver_E22_print_configuracion(void);


/**
 * @brief	Envía el string por aire.
 */
void driver_E22_send_message(uint8_t * p_message, uint8_t length);


/**
 * @brief	Recive un string por aire
 */
void driver_E22_recive_message(void);


/**
 * @brief	imprime por serie lo que se encuentra en el ring buffer interno del E22
 * @note	Imprime los bytes en hexadecimal
 */
void driver_E22_print_hexadecimal_ring_buffer(void);

/**
 * @brief	imprime por serie lo que se encuentra en el ring buffer interno del E22
 * @note	Imprime los bytes caracteres ascii
 */
void driver_E22_print_caracteres_ring_buffer(void);

/**
 * @brief	Configura el pin interno DIO3 del módulo como control del TCXO
 */
void driver_E22_SetDIO3asTCXOCtrl(void);

/**
 * @brief	Calibración del módulo
 * @note    Se debe llamar estando en modo standby
 */
void driver_E22_Calibrate(void);

void driver_E22_CalibrateImage(void);

/* ----- Métodos setter y getter para la configuración LoRa ----- */

/**
 *	@brief	Método setter definir el header (preámbulo) de la transmición por aire
 *	@note	Este valor se guarda en la estructura que se utiliza cuando se envía un mensaje
 */
void driver_E22_set_config_PreambleLength(uint16_t PreambleLength);

/**
 *	@brief	Método setter para tener un header (preámbulo) de longitud fija al transmitir por aire
 *	@note	Este valor se guarda en la estructura que se utiliza cuando se envía un mensaje
 */
void driver_E22_set_config_Header_is_fixed_length(bool Header_is_fixed_length);

/**
 *	@brief	Método setter para definir la frecuencia de la portadora
 *	@note	Este valor se guarda en la estructura que se utiliza cuando se envía un mensaje
 */
void driver_E22_set_config_frec_deseada_MHz(uint32_t frec_deseada_MHz);


#endif /* MAIN_DRIVERS_LIB_API_E22_H_ */

/********************** end of file ******************************************/
