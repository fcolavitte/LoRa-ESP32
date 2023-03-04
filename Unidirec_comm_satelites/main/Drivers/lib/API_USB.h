/*
 * API_USB.h
 *
 *  Created on: 15 feb. 2023
 *      Author: Facundo
 */

#ifndef MAIN_DRIVERS_LIB_API_USB_H_
#define MAIN_DRIVERS_LIB_API_USB_H_

#include <stdio.h>
#include "driver/uart.h"

#define USB_INPUT_LENGHT 30

void USB_get_input(void);

#endif /* MAIN_DRIVERS_LIB_API_USB_H_ */
