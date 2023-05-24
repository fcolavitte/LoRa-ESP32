/*
 * @file   : main.c
 * @date   : Feb 17, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include "MEF.h"
#include "API_E22.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "E22_test_.h"

/********************** macros and definitions *******************************/

#define test_E22 0

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

#if test_E22 == 0

void app_main(void) {
	start_MEF();
	driver_E22_init();
	while(1){
		update_MEF();
        vTaskDelay(10/ portTICK_PERIOD_MS);
	}
}
#endif


#if test_E22 == 1

void app_main(void) {
	start_MEF();
	driver_E22_init();

	while(1){
		/* Test Driver E22 */
		test_driver_E22_read_write_buffer_1byte();
	    test_driver_E22_read_write_buffer_2bytes();
        vTaskDelay(10000/ portTICK_PERIOD_MS);
	}
}
#endif

/********************** end of file ******************************************/
