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
#define ADDRES_RxGainConfiguration  (uint16_t)0x08AC
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
 *  @note   MOSI: [0x0E 0x00 char1 char2 char3 char4 ...]
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
 * @brief   Llena parte del ring buffer con el caracter indicado.
 * @param   [uint8_t] offset: Posición de inicio del ring buffer a sobreescribir.
 * @param   [uint8_t] length: Largo en bytes a sobreescribir del buffer.
 * @param   [uint8_t] characters: Caracter a utilizar para sobreescribir el buffer.
 */
void driver_E22_clear_buffer(uint8_t offset, uint8_t length, uint8_t characters);

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
 *  @note   MOSI: [0x80 0x00]
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
 *  @note   MOSI: [0x8F <offset de TX> <offset de RX>]
 */
void driver_E22_SetBufferBaseAddress(uint8_t tx_base_adress, uint8_t rx_base_adress);

/**
 *	@brief	Configura el SX1262 del E22 en modo Tx.
 *  @param [uint32_t] timeout: timeout en milisegundos.
 *	@note	Esta función no pone pin TXEN en alto.
 *  @note   MOSI: [0x83 0x1D 0x4C 0x00] (Depende del timeout).
 */
void driver_E22_SetTx_poner_modulo_en_modo_tx(uint32_t timeout);

/**
 *	@brief	Configura el SX1262 del E22 en modo Rx
 *  @param  [uint32_t] timeout: Dependiendo el valor del timeout es el tiempo a esperar el mensaje.
 *          Valor recomendado por defecto 0x1D4C00.
 *          El valor a ingresar debe ser el timeout deseado multiplecado por 2^6.
 *  @note   Si timeout es 0 el modo RX pasa a ser de recepción única sin timeout.
 *  @note   Si el timeout es 0xFFFFFF se pone en modo RX continuo, pudiendo guardar múltiples mensajes.
 *	@note	No contempla poner pin RX en alto necesario para que el módulo E22 reciba.
 */
void driver_E22_SetRx_poner_modulo_en_modo_rx(uint32_t timeout);

/**
 *	@brief Segun los parámetros recibidos se configura para el SX1262 o 1261
 *	@param [uint8_t] paDutyCycle: valor por defecto 0x02.
 *	@param [uint8_t] hpMax: valor por defecto 0x03.
 *	@param [uint8_t] deviceSe: Debe ser 0.
 *  @note  Para el SX1262 que tiene el E22-900M30S los parámetros deben ser (0x02,0x03,0)
 *  @note  Para (0x02,0x03,0) se envía MOSI: [0x95 0x02 0x03 0x00 0x01]
 */
void driver_E22_SetPaConfig(uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSe);

/**
 *	@brief Establece la potencia de salida y tiempo de rampa del dispositivo emisor
 *	@param [uint8_t] power: valor por defecto 0x16.
 *	@param [uint8_t] rampTime: valor por defecto 0x05.
 *  @note  Para los valores por defecto MOSI: [0x8E 0x16 0x05]
 */
void driver_E22_SetTxParams(uint8_t power, uint8_t rampTime);

/**
 *	@brief  Configura los parametros de comunicación, deben coincidir en el módulo Tx y Rx
 *	@param [uint8_t] SpringFactor: Valor por defecto 7. Puede llevarse hasta 10 para visualizar en SDR.
 *	@param [uint8_t] BandWidth: Valor por defecto 4 (31,25 kHz).
 *	@param [uint8_t] CodingRate: Valor por defecto 1.
 *	@param [uint8_t] LowDataRateOptimization: Valor por defecto 0.
 *                   Para paquetedes de larga duración se recomienda que esté en 1.
 *  @note  Para los valores por defecto (7,4,1,0) MOSI: [0x8B 0x07 0x04 0x01 0x00 0x00 0x00 0x00 0x00]
 */
void driver_E22_SetModulationParams(uint8_t SF, uint8_t BW, uint8_t CR, uint8_t LDRO);

/**
 *	@brief  Corrige el registro 0x0889 si tiene un valor erroneo tras reiniciar el dispositivo.
 *  @param  [uint8_t] BW: Ancho de banda seleccionado según tabla de SX1262.
 *  @note   MOSI: [0x1D 0x08 0x89 0x00] [0x0D 0x08 0x89 <Parametro de corrección, por defecto 0x04>]
 */
void driver_E22_fix_modulation_quality(uint8_t BW);

/**
 * @brief   Setea la palabra de sincronización.
 * @param   [uint16_t] sync: Palabra de sincronización utilizada en procolo LoRa.
 *          Tanto el módulo emisor como receptor deben tener igual palabra de sincronización.
 *          Valor por defecto 3444.
 * @note    MOSI: [0x0D 0x07 0x40 0x34 0x44]
 */
void driver_E22_SetSyncWord(uint16_t sync);

/**
 *	@brief	Configura el dataframe de la comunicación LoRa
 *  @param  [uint16_t] PreambleLength: Largo del preámbulo LoRa, debe coincidir en módulo emisor y receptor.
 *          Valor por defecto 12.
 *  @param  [bool] Header_is_fixed_length: Debe coincidir en módulo emisor y receptor. Por defecto 0 para explicit header.
 *  @param  [uint8_t] bytes_a_enviar: Número de bytes a enviar desde el buffer, o cantidad de bytes máxima a recibir.
 *	@note	El parámetro más importante es "bytes_a_enviar". En modo Rx define la cantidad máxima de bytes a recibir por mensaje.
 *  @note   Para un preámbulo de 12, 9 bytes a enviar y encabezado fijo:
 *          MOSI: [0x8C 0x00 0x0C 0x00 0x09 0x01 0x00 0x00 0x00 0x00]
 */
void driver_E22_SetPacketParams_con_modulo_en_modo_LoRa(uint16_t PreambleLength, bool Header_is_fixed_length, uint8_t bytes_a_enviar);

/**
 *	@brief  Corrige el registro 0x0736 si tiene un valor erroneo tras reiniciar el dispositivo.
 *  @param [bool] is_standar_IQ: 1 si se usa configuración IQ estandar, 0 si se usa invertida.
 *  @param [bool] is_TX: 1 si se va a emitir, 0 si se va a recibir.
 *  @note   MOSI:[0x1D 0x07 0x36 0x00], configura pines TXEN y RXEN, MOSI:[0x0D 0x07 0x36 0x0D]
 */
void driver_E22_fix_invertedIQ_register_and_eneable_power_module(bool is_standard_IQ, bool is_TX);

/**
 *	@brief	Setea el modo de transmisión del E22. Puede ser LoRa o FSK.
 *  @param  [bool] transmitir_en_modo_LoRa: 1 para transmitir en modo LoRa.
 *          Esta versión de driver solo está preparada para transmitir en modo LoRa.
 *	@note	Se recomienda que el parametro sea 1 (emitir en modo LoRa)
 *  @note   MOSI: [0x8A 0x01]
 */
void driver_E22_SetPacketType(bool transmitir_en_modo_LoRa);

/**
 *	@brief	Setea la frecuencia de la portadora
 *  @note   Para una frecuencia de 915MHz se envía:
 *          MOSI: [0x86 0x39 0x30 0x00 0x00]
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
 * @brief	Muestra por terminal serie la configuración del módulo E22.
 */
void driver_E22_print_configuracion(void);

/**
 * @brief   Configura los calores de los capacitores A y B del XTAL.
 * @note    MOSI: [0x80 0x01] [0x0D 0x09 0x11 0x12] [0x0D 0x09 0x12 0x12 ] [0x80 0x00]
 * @note    Al usarse va a modo standBy.
 */
void driver_E22_set_Cap(void);

/**
 * @brief   Limpia los Flags de las interrupciones.
 * @note    MOSI: [0x02 0x43 0xFF]
 */
void driver_E22_ClearIrqStatus(void);

/**
 * @brief  Setea las interrupciones.
 * @note   MOSI: [0x08 0x02 0x01 0x02 0x01 0x00 0x00 0x00 0x00]
 */
void driver_E22_SetDioIrqParams(void);

/**
 * @brief Hace un Get del estado del E22
 * @note MOSI: [0x12 0x00 0x00 0x00]
 */
void driver_E22_GetIrqStatus(void);

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


void driver_E22_print_hexadecimal_from_register(void);


/**
 * @brief	imprime por serie lo que se encuentra en el ring buffer interno del E22
 * @note	Imprime los bytes caracteres ascii
 */
void driver_E22_print_caracteres_ring_buffer(void);

/**
 * @brief	Configura el pin interno DIO3 del módulo como control del TCXO
 * @note    MOSI: [0x97 0x02 0x00 0x02 0x80]
 */
void driver_E22_SetDIO3asTCXOCtrl(void);

/**
 * @brief	Calibración del módulo
 * @note    Al usarse la función va al modo standby
 * @note    MOSI: [0x89 0x7F]
 */
void driver_E22_Calibrate(void);

/**
 * @brief   Calibra la salida respecto a la frecuencia a utilizar
 * @note    La función calibra para utilizar la frecuencia de 915 MHz
 * @note    MOSI: [0x98 0xE1 0xE9]
 */
void driver_E22_CalibrateImage(void);

/**
 * @brief   Configura la ganancia en modo RX
 * @note    MOSI: [0x0D 0x08 0xAC 0x96]
 */
void driver_E22_SetRxGain(void);

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
