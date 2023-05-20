/*
 * @file   : main.c
 * @date   : Feb 17, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include "main.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

void app_main(void) {
	start_MEF();
	while(1){
		update_MEF();

        vTaskDelay(10/ portTICK_PERIOD_MS);
	}
}

/********************** end of file ******************************************/
