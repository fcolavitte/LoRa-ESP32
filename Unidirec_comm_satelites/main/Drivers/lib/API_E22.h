/*
 * @file   : API_E22.h
 * @date   : Mar 11, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

#ifndef MAIN_DRIVERS_LIB_API_E22_H_
#define MAIN_DRIVERS_LIB_API_E22_H_

/********************** inclusions *******************************************/

#include "stdio.h"

/********************** macros and defines ***********************************/

/*** OPCODES ***/
#define OPCODE_SETTX			(uint8_t)0x83
#define OPCODE_SETSTANDBY		(uint8_t)0x80
#define OPCODE_SETSLEEP			(uint8_t)0x84
#define OPCODE_WRITEREGISTER	(uint8_t)0x0D
#define OPCODE_READREGISTER		(uint8_t)0x1D
#define OPCODE_WRITEBUFFER		(uint8_t)0x0E
#define OPCODE_READBUFFER		(uint8_t)0x1E


/*** GPIO ***/
#define GPIO_SPI_MOSI 23
#define GPIO_SPI_MISO 19
#define GPIO_SPI_SCK  18
#define GPIO_SPI_NSS  05
#define GPIO_E22_BUSY 21
#define GPIO_E22_RST  22

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

#endif /* MAIN_DRIVERS_LIB_API_E22_H_ */

/********************** end of file ******************************************/
