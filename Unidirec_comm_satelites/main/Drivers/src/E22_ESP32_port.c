/*
 * @file   : E22_ESP32_port.c
 * @date   : Mar 15, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include <driver/spi_master>
#include "hal/spi_hal.h"

/********************** macros and definitions *******************************/

#define SPI_MISO_GPIO	2	/*Revisar pines*/
#define SPI_MOSI_GPIO	7
#define SPI_SCLK_GPIO	6
#define SPI_NSS_GPIO	10

/********************** internal data declaration ****************************/

spi_device_handle_t hspi;

/********************** internal functions declaration ***********************/

void HAL_SPI_init(void);

void HAL_SPI_write(uint8_t *SPI_message, uint8_t message_lenght);

void HAL_GPIO_write(uint8_t GPIO, uint8_t state);

void HAL_GPIO_read(uint8_t GPIO);

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

void HAL_SPI_init(void){
	spi_bus_config_t spi_bus_config = {};
	spi_bus_config.mosi_io_num = SPI_MOSI_GPIO;
	spi_bus_config.miso_io_num = SPI_MISO_GPIO;
	spi_bus_config.sclk_io_num = SPI_SCLK_GPIO;
	spi_bus_config.quadwp_io_num = -1;
	spi_bus_config.quadhd_io_num = -1;
	spi_bus_config.max_transfer_sz = 32;
	spi_bus_initialize(SPI2_HOST, &spi_bus_config, SPI_DMA_DISABLED);

	spi_device_interface_config_t spi_device_interface_config = {};
	/*completar spi_device_interface_config*/
	spi_bus_add_device(SPI2_HOST, &spi_device_interface_config, hspi);
}

void HAL_SPI_write(uint8_t *SPI_message, uint8_t message_lenght){

}

void HAL_GPIO_write(uint8_t GPIO, uint8_t state){

}

void HAL_GPIO_read(uint8_t GPIO){

}

/********************** end of file ******************************************/
