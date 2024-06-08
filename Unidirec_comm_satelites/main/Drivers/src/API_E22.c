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

struct config_E22_s{
	uint16_t PreambleLength;
	bool Header_is_fixed_length;
	uint32_t frec_deseada_MHz;
};
struct config_E22_s config_E22 = {
		.PreambleLength = 12,
		.Header_is_fixed_length = 1,
		.frec_deseada_MHz = 915
};

/********************** internal functions declaration ***********************/

/********************** external functions declaration ***********************/

extern void driver_HAL_spi_init(void);
extern uint8_t driver_HAL_transaction(uint8_t *tx_buffer, uint8_t tx_length, uint8_t *rx_buffer, uint8_t rx_length);
extern void driver_HAL_gpio_init(uint8_t GPIO_num, bool GPIO_is_input);
extern void driver_HAL_GPIO_write(uint8_t GPIO, bool state);
extern bool driver_HAL_GPIO_read(uint8_t GPIO);

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
	if(MAX_SIZE_SPI_BUFFERS - 2 < tx_length) {
		tx_length = MAX_SIZE_SPI_BUFFERS - 2;
	}
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	uint8_t MOSI_buffer[MAX_SIZE_SPI_BUFFERS] = {};
	MOSI_buffer[0] = OPCODE_WRITEBUFFER;
	MOSI_buffer[1] = offset;
	memcpy(&(MOSI_buffer[2]), tx_buffer, tx_length);
	driver_HAL_transaction(MOSI_buffer, tx_length+2, NULL, 0);
}


void driver_E22_read_from_buffer(uint8_t offset, uint8_t* rx_buffer, uint8_t rx_length) {
	if(MAX_SIZE_SPI_BUFFERS - 3 < rx_length) {
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


void driver_E22_write_in_registro(uint8_t *address, uint8_t* tx_buffer, uint8_t tx_length) {
	if(MAX_SIZE_SPI_BUFFERS - 3 < tx_length) {
		tx_length = MAX_SIZE_SPI_BUFFERS - 3;
	}
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	uint8_t MOSI_buffer[MAX_SIZE_SPI_BUFFERS] = {};
	MOSI_buffer[0] = OPCODE_WRITEREGISTER;
	MOSI_buffer[1] = address[1];
	MOSI_buffer[2] = address[0];
	memcpy(&(MOSI_buffer[3]), tx_buffer, tx_length);
	driver_HAL_transaction(MOSI_buffer, tx_length+3, NULL, 0);
}


void driver_E22_read_from_registro(uint8_t *address, uint8_t* rx_buffer, uint8_t rx_length) {
	if(MAX_SIZE_SPI_BUFFERS - 4 < rx_length) {
		rx_length = MAX_SIZE_SPI_BUFFERS - 4;
	}
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	uint8_t MOSI_buffer[MAX_SIZE_SPI_BUFFERS] = {};
	MOSI_buffer[0] = OPCODE_READREGISTER;
	MOSI_buffer[1] = address[0];
	MOSI_buffer[2] = address[1];
	MOSI_buffer[3] = NOP;
	driver_HAL_transaction(MOSI_buffer, 4, rx_buffer, rx_length);
}

void driver_E22_set_standBy(void){
	uint8_t MOSI_buffer[1] = {};
	MOSI_buffer[0] = OPCODE_SETSTANDBY;
	driver_HAL_transaction(MOSI_buffer, 1, NULL, 0);
}

void driver_E22_GetRxBufferStatus(uint8_t* PayloadLengthRx, uint8_t* RxBufferPointer) {
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	uint8_t MISO_buffer[2] = {};
	uint8_t MOSI_buffer[2] = {};
	MOSI_buffer[0] = OPCODE_GetRxBufferStatus;
	MOSI_buffer[1] = NOP;
	driver_HAL_transaction(MOSI_buffer, 2, MISO_buffer, 2);
	*PayloadLengthRx = MISO_buffer[0];
	*RxBufferPointer = MISO_buffer[1];
}


void driver_E22_SetBufferBaseAddress(uint8_t tx_base_adress, uint8_t rx_base_adress) {
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	uint8_t MOSI_buffer[3] = {};
	MOSI_buffer[0] = OPCODE_SetBufferBaseAddress;
	MOSI_buffer[1] = tx_base_adress;
	MOSI_buffer[2] = rx_base_adress;
	driver_HAL_transaction(MOSI_buffer, 3, NULL, 0);
}


void driver_E22_SetTx_poner_modulo_en_modo_tx(uint32_t timeout) {
	if(timeout > 0xFFFFFF) {
		timeout = 0xFFFFFF;
	}
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	timeout=timeout<<6;
	uint8_t MOSI_buffer[4] = {};
	MOSI_buffer[0] = OPCODE_SET_TX_MODE;
	MOSI_buffer[1] = (uint8_t)(timeout >> 16);
	MOSI_buffer[2] = (uint8_t)(timeout >> 8);
	MOSI_buffer[3] = (uint8_t)(timeout >> 0);
	driver_HAL_transaction(MOSI_buffer, 4, NULL, 0);
}


void driver_E22_SetRx_poner_modulo_en_modo_rx(uint32_t timeout) {
	if(timeout > 0xFFFFFF) {
		timeout = 0xFFFFFF;
	}
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	uint8_t MOSI_buffer[4] = {};
	MOSI_buffer[0] = OPCODE_SET_RX_MODE;
	MOSI_buffer[1] = (uint8_t)((timeout >> 16) & 0xFF);
	MOSI_buffer[2] = (uint8_t)((timeout >> 8)  & 0xFF);
	MOSI_buffer[3] = (uint8_t)((timeout >> 0)  & 0xFF);
	driver_HAL_transaction(MOSI_buffer, 4, NULL, 0);
}

void driver_E22_SetPaConfig(uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSe){
	uint8_t MOSI_buffer[5] = {};
	MOSI_buffer[0] = OPCODE_SetPaConfig;
	MOSI_buffer[1] = paDutyCycle;	/*0x02 para SX1262*/
	MOSI_buffer[2] = hpMax;			/*0x03 para SX1262*/
	MOSI_buffer[3] = deviceSe;		/*0 para SX1262*/
	MOSI_buffer[4] = 0x01;
	driver_HAL_transaction(MOSI_buffer, 5, NULL, 0);
}

void driver_E22_SetTxParams(uint8_t power, uint8_t rampTime){
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	driver_E22_SetPaConfig(0x02, 0x03, 0);
	vTaskDelay(1);
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	if(rampTime>7){
		rampTime=7;
	}
	uint8_t MOSI_buffer[3] = {};
	MOSI_buffer[0] = OPCODE_SetTxParams;
	MOSI_buffer[1] = power;
	MOSI_buffer[2] = rampTime;
	driver_HAL_transaction(MOSI_buffer, 3, NULL, 0);
}


void driver_E22_SetModulationParams(uint8_t SF, uint8_t BW, uint8_t CR, uint8_t LDRO) {
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	if(SF<7){SF=7;}
	if(SF>12){SF=12;}
	if(BW>9){BW=9;}
	if(CR<1){CR=1;}
	if(CR>4){CR=4;}
	uint8_t MOSI_buffer[5] = {};
	MOSI_buffer[0] = OPCODE_SetModulationParams;
	MOSI_buffer[1] = SF;
	MOSI_buffer[2] = BW;
	MOSI_buffer[3] = CR;
	MOSI_buffer[4] = LDRO;
	driver_HAL_transaction(MOSI_buffer, 5, NULL, 0);
	driver_E22_fix_modulation_quality(BW);
}

void driver_E22_fix_modulation_quality(uint8_t BW){
	uint8_t address[2]={};
	address[0] = (uint8_t)((ADDRES_TX_MODULATION >> 0)  & 0xFF);
	address[1] = (uint8_t)((ADDRES_TX_MODULATION >> 8)  & 0xFF);
	uint8_t value [1] = {};
	driver_E22_read_from_registro(address, value, 1);
	if(7 == BW){	// BW = 7 -> 500 kHz
		value [0] = value [0] & 0xFB;
	} else {
		value [0] = value [0] | 0x04;
	}
	driver_E22_write_in_registro(address, value, 1);
}

void driver_E22_SetSyncWord(uint16_t sync) {
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	uint8_t address[2]={};
	address[0] = (uint8_t)((ADDRES_LORA_SYNC_WORD_MSB >> 0)  & 0xFF);
	address[1] = (uint8_t)((ADDRES_LORA_SYNC_WORD_MSB >> 8)  & 0xFF);
	uint16_t aux_sync =(uint8_t)((sync >> 8) & 0xFF);
	aux_sync += (sync & 0xFF)<<8;
	sync = aux_sync;
	driver_E22_write_in_registro(address, (uint8_t *)&sync, 2);
}

void driver_E22_SetPacketParams_con_modulo_en_modo_LoRa(uint16_t PreambleLength, bool Header_is_fixed_length, uint8_t bytes_a_enviar) {
	if(Header_is_fixed_length) {
		Header_is_fixed_length = 1;
	}
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	uint8_t MOSI_buffer[7] = {};
	MOSI_buffer[0] = OPCODE_SetPacketParams;					/* OPCODE */
	MOSI_buffer[1] = (uint8_t)((PreambleLength >> 8) & 0xFF);	/* preambulo 1 - PreambleLength		*/
	MOSI_buffer[2] = (uint8_t)((PreambleLength >> 0) & 0xFF);	/* preambulo 2 - PreambleLength		*/
	//MOSI_buffer[3] = Header_is_fixed_length;					/* preambulo 3 - HeaderType			*/
	MOSI_buffer[3] = 0;					/* preambulo 3 - HeaderType			*/
	MOSI_buffer[4] = bytes_a_enviar/8;							/* preambulo 4 - PayloadLength		*/
	MOSI_buffer[5] = 0;											/* preambulo 5 - CRC - Configurado como OFF */
	MOSI_buffer[6] = 0;											/* preambulo 6 - InvertIQ - Configurado como Standard IQ setup */
	driver_HAL_transaction(MOSI_buffer, 7, NULL, 0);
	driver_E22_fix_invertedIQ_register(1);
}

void driver_E22_fix_invertedIQ_register(bool is_standard_IQ){
	uint8_t address[2]={};
	address[0] = (uint8_t)((ADDRES_IQ_POLARITY_SETUP >> 0)  & 0xFF);
	address[1] = (uint8_t)((ADDRES_IQ_POLARITY_SETUP >> 8)  & 0xFF);
	uint8_t value [1] = {};
	driver_E22_read_from_registro(address, value, 1);
	if(0 == is_standard_IQ){
		//QI invertido:
		value [0] = value [0] & 0xFB;
	} else {
		value [0] = value [0] | 0x04;
	}
	driver_E22_write_in_registro(address, value, 1);
}

void driver_E22_SetPacketType(bool transmitir_en_modo_LoRa) {
	if(transmitir_en_modo_LoRa) {
		transmitir_en_modo_LoRa = 1;
	}
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	uint8_t MOSI_buffer[2] = {};
	MOSI_buffer[0] = OPCODE_SetPacketType;
	MOSI_buffer[1] = transmitir_en_modo_LoRa;
	driver_HAL_transaction(MOSI_buffer, 2, NULL, 0);
}


void driver_E22_SetRfFrequency(uint32_t frec_deseada_MHz) {
	/* 1,048576 de 2^25 / F_XTAL siendo F_XTAL=32MHz y * 10^6 porque la frecuencia se ingresa en MHz a ésta función */
	uint32_t param_RfFreq = frec_deseada_MHz * 1048576;
	if(driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
		vTaskDelay(1);
	}
	uint8_t MOSI_buffer[5] = {};
	MOSI_buffer[0] = OPCODE_SET_RF_FREC;
	MOSI_buffer[1] = (uint8_t)((param_RfFreq >> 24) & 0xFF);
	MOSI_buffer[2] = (uint8_t)((param_RfFreq >> 16) & 0xFF);
	MOSI_buffer[3] = (uint8_t)((param_RfFreq >> 8)  & 0xFF);
	MOSI_buffer[4] = (uint8_t)((param_RfFreq >> 0)  & 0xFF);
	driver_HAL_transaction(MOSI_buffer, 5, NULL, 0);
}


void driver_E22_setear_pin_TX_salida_potencia(bool estado) {
	if(-1 != GPIO_TX_ENABLE){
		driver_HAL_GPIO_write(GPIO_TX_ENABLE, estado);
	}
}


void driver_E22_setear_pin_RX_entrada_aire(bool estado) {
	if(-1 != GPIO_RX_ENABLE){
		driver_HAL_GPIO_write(GPIO_RX_ENABLE, estado);
	}
}


void driver_E22_print_configuracion(void) {
	printf("\n>> Configuración actual del módulo E22:\n");
	printf("        PreambleLength = %u\n", (unsigned int)config_E22.PreambleLength);
	printf("        Header_is_fixed_length = %s\n", (config_E22.Header_is_fixed_length ? "true" : "false"));
	printf("        Frecuencia de la portadora = %03d MHz\n", config_E22.frec_deseada_MHz);
}


void driver_E22_print_hexadecimal_ring_buffer(void) {
	printf("Contenido del buffer del E22:");
	for (uint16_t offset = 0; offset <256; offset++) {
		if(0 == offset%8){
			printf("\noffset %3d:",offset);
		}
		uint8_t rx_buffer;
		driver_E22_read_from_buffer((uint8_t)offset, &rx_buffer, 1);
		printf("0x%02x ", rx_buffer);
		vTaskDelay(10/ portTICK_PERIOD_MS);
	}
	printf("\n");
}


void driver_E22_print_caracteres_ring_buffer(void) {
	printf("Contenido del buffer del E22:");
	for (uint16_t offset = 0; offset <256; offset++) {
		if(0 == offset%8){
			printf("\noffset %3d:",offset);
		}
		uint8_t rx_buffer;
		driver_E22_read_from_buffer((uint8_t)offset, &rx_buffer, 1);
		printf("%c ", rx_buffer);
		vTaskDelay(10/ portTICK_PERIOD_MS);
	}
	printf("\n");
}

void driver_E22_SetDIO3asTCXOCtrl(void){
	uint8_t tcxoVoltage = 0x07;
	uint32_t delay_of_bits = 0x20;
	uint8_t MOSI_buffer[5] = {};
	MOSI_buffer[0] = OPCODE_SetDIO3asTCXOCtrl;
	MOSI_buffer[1] = tcxoVoltage;
	MOSI_buffer[2] = (uint8_t)((delay_of_bits >> 16) & 0xFF);
	MOSI_buffer[3] = (uint8_t)((delay_of_bits >> 8)  & 0xFF);
	MOSI_buffer[4] = (uint8_t)((delay_of_bits >> 0)  & 0xFF);
	driver_HAL_transaction(MOSI_buffer, 5, NULL, 0);
}

void driver_E22_Calibrate(void){
	uint8_t MOSI_buffer[2] = {};
	MOSI_buffer[0] = OPCODE_CalibrateFunction;
	MOSI_buffer[1] = 0b01111111;
	driver_HAL_transaction(MOSI_buffer, 2, NULL, 0);
}

void driver_E22_CalibrateImage(void){
	uint8_t MOSI_buffer[3] = {};
	MOSI_buffer[0] = OPCODE_CalibrateImage;
	MOSI_buffer[1] = 0xE1;
	MOSI_buffer[2] = 0xE9;
	driver_HAL_transaction(MOSI_buffer, 3, NULL, 0);
}

void driver_E22_send_message(uint8_t * p_message, uint8_t length) {
	/* Poner pin de not_reset en alto */
	driver_HAL_GPIO_write(GPIO_E22_NRST, HIGH);
    vTaskDelay(100);

	/* Set Stand By */
	driver_E22_set_standBy();
	vTaskDelay(1);

	/* Setear modo de transmisión como LoRa */
	bool transmitir_en_modo_LoRa = 1;
	driver_E22_SetPacketType(transmitir_en_modo_LoRa);
    vTaskDelay(1);

	/* Configurar pin DIO3 interno del módulo como control de XTAL */
	driver_E22_SetDIO3asTCXOCtrl();
	vTaskDelay(10);

	/* Set Stand By */
	driver_E22_set_standBy();
	vTaskDelay(1);

	/* Calibrar */
	driver_E22_Calibrate();
	vTaskDelay(1);

	/* Setear Capacitor de XTAL */

	/* Calibrar imágen */
	driver_E22_CalibrateImage();
	vTaskDelay(1);

	/* Setear frecuencia a utilizar para envío de datos */
	driver_E22_SetRfFrequency(config_E22.frec_deseada_MHz);
    vTaskDelay(1);

    /* Setear potencia de salida */
    driver_E22_SetTxParams(0x16, 0x05);
    vTaskDelay(1);

	/* Escribir en el buffer interno del E22 el string a enviar en la posición cero */
	driver_E22_write_in_buffer(0, p_message, length);
    vTaskDelay(1);

    /* Setear los parametros de modulaci�n */
    driver_E22_SetModulationParams(7, 4, 1, 0);	//(7, 4, 1, 0) por defecto, con (7,8,1,0) y (9,8,1,0) se visualiza mejor en el LDR
	vTaskDelay(1);

	/* Avisar al E22 el largo de bytes a enviar */
	driver_E22_SetPacketParams_con_modulo_en_modo_LoRa(12, config_E22.Header_is_fixed_length, length);
    vTaskDelay(1);

    /* Palabra de sincronizaci�n */
    driver_E22_SetSyncWord(0x3444);
    vTaskDelay(1);

	/* Establecer que Rx y Tx en aire comiencen desde el byte 0 del buffer circular interno del E22 */
	driver_E22_SetBufferBaseAddress(0, 32);
    vTaskDelay(1);

	/* Pasar módulo a modo Tx con timeout=0 */
	driver_E22_setear_pin_TX_salida_potencia(HIGH);
	vTaskDelay(10);

	/* Corrección de calidad de la modulación "fix modulation quality" */
	//Ya está tras hacer setModulationParams


	/* Comenzar a enviar mensaje */
    driver_E22_SetTx_poner_modulo_en_modo_tx(300);//300 //mandar -> timeout*(2^6) = "tiempo>>6"

    /* Tras retorno de pin busy del E22 a low, poner pin de Tx en low*/
    for (int i=0; i<10;i++) {
        vTaskDelay(10/ portTICK_PERIOD_MS);
        if(9 == i) {
            printf(">> ERROR: El mensaje no se envi�!\n");
        }
        if (0 == driver_HAL_GPIO_read(GPIO_E22_BUSY)) {
        	i = 10;
            printf("\n>> Mensaje enviado (E22).\n");
        }
    }
    driver_E22_setear_pin_TX_salida_potencia(LOW);
}


void driver_E22_recive_message(void) {
	/* Poner pin de not_reset en alto */
	driver_HAL_GPIO_write(GPIO_E22_NRST, HIGH);
    vTaskDelay(100);

	/* Set Stand By */
	driver_E22_set_standBy();
	vTaskDelay(1);

	/* Setear modo de transmisión como LoRa */
	bool transmitir_en_modo_LoRa = 1;
	driver_E22_SetPacketType(transmitir_en_modo_LoRa);
    vTaskDelay(1);

	/* Configurar pin DIO3 interno del módulo como control de XTAL */
	driver_E22_SetDIO3asTCXOCtrl();
	vTaskDelay(10);

	/* Set Stand By */
	driver_E22_set_standBy();
	vTaskDelay(1);

	/* Calibrar */
	driver_E22_Calibrate();
	vTaskDelay(1);

	/* Calibrar imágen */
	driver_E22_CalibrateImage();
	vTaskDelay(1);

	/* Setear frecuencia a utilizar para envío de datos */
	driver_E22_SetRfFrequency(config_E22.frec_deseada_MHz);
    vTaskDelay(1);

    /* Setear los parametros de modulaci�n */
    driver_E22_SetModulationParams(7, 4, 1, 0);
    vTaskDelay(1);

	/* Avisar al E22 el largo de bytes a enviar */
	//driver_E22_SetPacketParams_con_modulo_en_modo_LoRa(config_E22.PreambleLength, config_E22.Header_is_fixed_length, 120);
	driver_E22_SetPacketParams_con_modulo_en_modo_LoRa(12, config_E22.Header_is_fixed_length, 120);
    vTaskDelay(1);

    /* Palabra de sincronizaci�n */
    driver_E22_SetSyncWord(0x3444);
    vTaskDelay(1);

	/* Establecer que Rx y Tx en aire comiencen desde el byte 0 del buffer circular interno del E22 */
	driver_E22_SetBufferBaseAddress(0, 32);
    vTaskDelay(1);

	/* Habilitar el modo Rx de la potencia */
    driver_E22_setear_pin_RX_entrada_aire(HIGH);
	vTaskDelay(1);
	/* Timeout 0xFFFFFF hace que esté en modo recepción continua hasta que se cambia el
	 * modo de recepción (con otro timeout) o hasta que se fuerza por opcode a it a idle o tx*/
	driver_E22_SetRx_poner_modulo_en_modo_rx(0x1000);//0xFFFFFF//0x1000
}



/*
 * Setter y getters para la estructura de control del E22
 */

void driver_E22_set_config_PreambleLength(uint16_t PreambleLength){
	if (PreambleLength > 0) {
		config_E22.PreambleLength = PreambleLength;
	}
}


void driver_E22_set_config_Header_is_fixed_length(bool Header_is_fixed_length){
	config_E22.Header_is_fixed_length = Header_is_fixed_length;
}


void driver_E22_set_config_frec_deseada_MHz(uint32_t frec_deseada_MHz){
	if ((frec_deseada_MHz > 850) && (frec_deseada_MHz < 930)) {
		config_E22.frec_deseada_MHz = frec_deseada_MHz;
	}
}



/********************** end of file ******************************************/
