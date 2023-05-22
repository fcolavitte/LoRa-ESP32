/*
 * @file   : E22_ESP32_port.c
 * @date   : Mar 15, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include <driver/spi_master.h>
#include <soc/spi_pins.h>
#include <stdint.h>
#include <stdbool.h>
#include "API_E22.h"

/********************** macros and definitions *******************************/

#define SPI_MISO_GPIO   SPI3_IOMUX_PIN_NUM_MISO /* GPIO N°19 */
#define SPI_MOSI_GPIO   SPI3_IOMUX_PIN_NUM_MOSI /* GPIO N°23 */
#define SPI_SCLK_GPIO   SPI3_IOMUX_PIN_NUM_CLK  /* GPIO N°18 */
#define SPI_NSS_GPIO    SPI3_IOMUX_PIN_NUM_CS   /* GPIO N°05 */

/********************** internal data declaration ****************************/

spi_device_handle_t hspi_device;

/********************** internal functions declaration ***********************/

void driver_HAL_spi_init(void);
uint8_t driver_HAL_transaction(uint8_t *tx_buffer, uint8_t tx_length, uint8_t *rx_buffer, uint8_t rx_length);
void driver_HAL_gpio_init(uint8_t GPIO_num, bool_t GPIO_is_input);
void driver_HAL_GPIO_write(uint8_t GPIO, bool_t state);
bool_t driver_HAL_GPIO_read(uint8_t GPIO);

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/**
 *	@brief	La función inicia el periférico SPI
 */
void driver_HAL_spi_init(void) {
    spi_bus_config_t spi_bus_config = {};
    spi_bus_config.mosi_io_num = SPI_MOSI_GPIO;
    spi_bus_config.miso_io_num = SPI_MISO_GPIO;
    spi_bus_config.sclk_io_num = SPI_SCLK_GPIO;
    spi_bus_config.quadwp_io_num = -1;
    spi_bus_config.quadhd_io_num = -1;
    spi_bus_config.max_transfer_sz = MAX_SIZE_SPI_BUFFERS;
    spi_bus_initialize(SPI3_HOST, &spi_bus_config, SPI_DMA_DISABLED);

    spi_device_interface_config_t spi_device_interface_config = {};
    spi_device_interface_config.mode = 0;
    spi_device_interface_config.duty_cycle_pos = 128;
    spi_device_interface_config.clock_speed_hz = 1000000; /* 1MHz */
    spi_device_interface_config.spics_io_num = SPI_NSS_GPIO;
    spi_device_interface_config.flags = SPI_DEVICE_HALFDUPLEX;
    spi_device_interface_config.queue_size = 1;
    spi_device_interface_config.pre_cb = NULL;
    spi_device_interface_config.post_cb = NULL;
    spi_bus_add_device(SPI3_HOST, &spi_device_interface_config, &hspi_device);
}


/**
 * @brief   Función para realizar transacción por el puerto SPI
 * @note    Al módulo E22 solo se le pueden enviar mensajes y esperar respuestas,
 *          por ende tx debe estar para que la transacción con el E22 sea correcta.
 *          Rx puede no estar, si algún parámetro de rx es cero no se espera respuesta.
 *          Los byte NOP (0x00) después del opcode y antes de las respuestas hay que cargarlos como
 *          parte del tx. Los NOP (0x00) de tx en el momento de la respuesta no hay que cargarlos.
 *          Ejemplo: Leer desde el buffer:
 *              MOSI:  0x1E, 0x00, 0x00
 *              MISO:  undefined, undefined, respuesta
 *          El primer 0x00 en MOSI no se ejecuta al momento de la respuesta sino que es un byte de
 *          tiempo para que el E22 pueda procesar la respuesta a enviar. Por ende la configuración
 *          debe ser:
 *              tx_buffer = {0x1E,0x00}, tx_length = 2, rx_length = 1.
 * @retval  [uint8_t] Devuelve 1 si la transacción fue correcta, sino devuelve 0.
 * @param   [uint8_t *] tx_buffer: Puntero al buffer a transmitir.
 * @param   [uint8_t]   tx_length: Longitud en bytes a transmitir.
 * @param   [uint8_t *] rx_buffer: Puntero a la posición de memoria donde comenzar a escribir la respuesta.
 * @param   [uint8_t]   rx_length: Longitud en bytes a recibir.
*/
uint8_t driver_HAL_transaction(uint8_t *tx_buffer, uint8_t tx_length, uint8_t *rx_buffer, uint8_t rx_length) {
    /* Verificación de valores válidos, no se puede recibir sin transmitir en el SPI con el E22 */
    if(0==tx_buffer || 0==tx_length) {
        return 0;
    }
    /* Si algún parametro de rx es cero no se recibe nada */
    if(0==rx_buffer || 0==rx_length) {
        rx_length = 0;
        rx_length = 0;
    }
    spi_transaction_t spi_transaction = {};
    spi_transaction.length = tx_length*8;
    spi_transaction.rxlength = rx_length*8;
    spi_transaction.tx_buffer = tx_buffer;
    spi_transaction.rx_buffer = rx_buffer;
    spi_device_transmit(hspi_device, &spi_transaction);
    return 1;
}


/**
 *	@brief	La función inicializa los GPIO necesarios para el control del E22
 *	@param	[uint8_t] GPIO_num: Número del GPIO.
 *	@param	[bool_t] GPIO_is_input: 1 si el pin es INPUT, 0 si el pin es OUTPUT.
 */
void driver_HAL_gpio_init(uint8_t GPIO_num, bool_t GPIO_is_input) {
	gpio_config_t pGPIOConfig;
	pGPIOConfig.pin_bit_mask = (1 << GPIO_num);
	if(GPIO_is_input) {
		pGPIOConfig.mode = GPIO_MODE_INPUT;
	} else {
		pGPIOConfig.mode = GPIO_MODE_INPUT_OUTPUT;
	}
	pGPIOConfig.pull_up_en = GPIO_PULLUP_DISABLE;
	pGPIOConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
	pGPIOConfig.intr_type = GPIO_INTR_DISABLE;
	gpio_config(&pGPIOConfig);
}


/**
 *	@brief	Establecer un valor de salida lógico en un GPIO.
 *	@param	[uint8_t] GPIO: Número de GPIO a controlar.
 *	@param	[bool_t] state: 0 para valor lógico bajo y 1 para valor lógico alto.
 */
void driver_HAL_GPIO_write(uint8_t GPIO, bool_t state) {
	gpio_set_level(GPIO, state);
}


/**
 *	@brief	Devuelve el valor lógico de un pin.
 *	@param	[uint8_t] GPIO: Número de GPIO a leer.
 *	@retval	[bool_t]: 0 para valor lógico bajo y 1 para valor lógico alto.
 */
bool_t driver_HAL_GPIO_read(uint8_t GPIO) {
	uint8_t retvalue = 0;
	retvalue = gpio_get_level(GPIO);
	return retvalue;
}


/********************** external functions definition ************************/

/********************** end of file ******************************************/
