/*
 * @file   : E22_test_.h
 * @date   : May 23, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

#ifndef MAIN_E22_TEST_E22_TEST__H_
#define MAIN_E22_TEST_E22_TEST__H_

#define DELAY_MS 500	/* Si se pone un valor muy bajo no se aprecia salida por el puerto serie */

void test_probar_todos_los_tests(void);


void test_driver_E22_read_write_buffer_1byte(void);
void test_driver_E22_read_write_buffer_2bytes(void);

#endif /* MAIN_E22_TEST_E22_TEST__H_ */
