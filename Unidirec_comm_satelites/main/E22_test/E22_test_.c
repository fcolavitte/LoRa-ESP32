/*
 * @file   : E22_test_.c
 * @date   : May 23, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

#include "API_E22.h"
#include "E22_test_.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void test_probar_todos_los_tests(void) {
	test_driver_E22_read_write_buffer_1byte();
    test_driver_E22_read_write_buffer_2bytes();
}


void test_driver_E22_read_write_buffer_1byte(void) {
	printf("\n|*** Inicio Test 'read_write_buffer_1byte' ***|\n");
	uint8_t buffer_rx = 0;
	for(uint8_t i=0;i<20;i++) {
		driver_E22_read_from_buffer(i, &buffer_rx, 1);
		vTaskDelay(DELAY_MS/ portTICK_PERIOD_MS);
		printf("Se leyó la posición %u y el valor leído es %u\n", i, buffer_rx);
	}
	for(uint8_t i=0;i<20;i++) {
		driver_E22_write_in_buffer(i, &i, 1);
		vTaskDelay(DELAY_MS/ portTICK_PERIOD_MS);
		printf("Se escribió en la posición %u el valor %u\n", i, i);
	}
	for(uint8_t i=0;i<20;i++) {
		driver_E22_read_from_buffer(i, &buffer_rx, 1);
		vTaskDelay(DELAY_MS/ portTICK_PERIOD_MS);
		printf("Se leyó la posición %u y el valor leído es %u\n", i, buffer_rx);
	}
}

void test_driver_E22_read_write_buffer_2bytes(void) {
	printf("\n|*** Inicio Test 'read_write_buffer_2bytes' ***|\n");
	uint8_t buffer_rx[2];
	uint8_t buffer_tx[2];
	for(uint8_t i=0;i<20;i++) {
		driver_E22_read_from_buffer(i*2, buffer_rx, 2);
		vTaskDelay(DELAY_MS/ portTICK_PERIOD_MS);
		printf("Se leyó la posición %u dos bytes, el primero es %u y el segundo es %u\n", i*2, buffer_rx[0], buffer_rx[1]);
	}
	for(uint8_t i=0;i<20;i++) {
		buffer_tx[0]=i;
		buffer_tx[1]=i*2;
		driver_E22_write_in_buffer(i*2, buffer_tx, 2);
		vTaskDelay(DELAY_MS/ portTICK_PERIOD_MS);
		printf("Se escribió en la posición %u el valor %u y en la siguiente posición el valor %u\n", i*2, buffer_tx[0], buffer_tx[1]);
	}
	for(uint8_t i=0;i<20;i++) {
		driver_E22_read_from_buffer(i*2, buffer_rx, 2);
		vTaskDelay(DELAY_MS/ portTICK_PERIOD_MS);
		printf("Se leyó la posición %u dos bytes, el primero es %u y el segundo es %u\n", i*2, buffer_rx[0], buffer_rx[1]);
	}
}


