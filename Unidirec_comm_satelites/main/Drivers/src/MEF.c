/*
 * MEF.c
 *
 *  Created on: 17 feb. 2023
 *      Author: Facundo
 */
#include "main.h"
#include "API_Firebase.h"



/*		----- Typedef -----		*/

/*typedef struct{
	char SSD[20],
	char WiFi_PASS[20]
} WiFi_config_t;*/

/**
 * @brief Estructura de datos de configuración WiFi y guardados en DB
 */
typedef struct{
	uint8_t MEF_mode;	/*<WiFi ON/OFF><2 bits para modo>*/
	//WiFi_config_t WiFi_config
} WiFi_data_t;


typedef enum{
	menu_main 		= 0,
	menu_mensaje 	= 1,
	menu_config_web = 2,
	menu_wifi_config 	 = 21,
	menu_firebase_config = 22,
	menu_time_config 	 = 23
}pos_menu_t;


/*		----- Variables Globales privadas -----		*/
pos_menu_t pos_menu_actual;
WiFi_data_t WiFi_data;

/*		----- Funciones privadas -----		*/
void display_menu(void);
void move_menu(uint8_t numero_ingresado);
void display_help(void);
void display_comds_list(void);
void reset_config_dataBase(void);

void star_MEF(void){
	pos_menu_actual = menu_main;
	iniciar_Json_from_DB();
	client_get_Json();
}


/**
 * @brief Se muestra las opciones del menú actual
 */
void display_menu(void){
	printf("\n\n    ----  MENU  ----\n");
	switch (pos_menu_actual){
		case menu_main:
			printf("1. Mensaje\n2. Configuración Web\n3. Ayuda\n");
		break;
		case menu_mensaje:
			printf("0. Atras\n1. Señal beacon\n2. Modo de señal\n3. Periodo de emisión [segundos]\n");
			printf("4. Ventana [minutos]\n5. Inicio de emisión\n6. Ayuda\n");
		break;
		case menu_config_web:
			printf("0. Atras\n1. WiFi Config\n2. FireBase config\n3. Fecha y Hora\n4. Ayuda\n");
		break;
		case menu_wifi_config:
			printf("0. Atras\n1. SSD\n2. WiFi PASS\n3. Ayuda\n");
		break;
		case menu_firebase_config:
			printf("0. Atras\n1. Link\n2. Restablecer\n3. Ayuda\n");
		break;
		case menu_time_config:
			printf("0. Atras\n1. UTC\n2. Fecha\n3. Hora\n");
		break;
		default:
			pos_menu_actual = menu_main;
			printf("1. Mensaje\n2. Configuración Web\n3. Ayuda\n");
		break;
	}
}


/**
 * @brief Si se presionó un número, se mueve a travéz del menu
 */
void move_menu(uint8_t numero_ingresado){
	switch (pos_menu_actual){
		case menu_main:
			switch(numero_ingresado){
				case '1':
					pos_menu_actual = menu_mensaje;
					display_menu();
				break;
				case '2':
					pos_menu_actual = menu_config_web;
					display_menu();
				break;
				case '3':
					display_help();
					display_menu();
				break;
				default:
					printf("\n\nOpcion no valida. Intente nuevamente.\n");
					printf("Si quiere ingresar un comando ingrese \"$\" antes del texto.\n");
					printf("Si quiere enviar un mensaje por LoRa ingrese \">\" antes del mensaje.\n\n");
					display_menu();
				break;
			}
		break;
		case menu_mensaje:
			switch(numero_ingresado){
				case '0':
					pos_menu_actual = menu_main;
					display_menu();
				break;
				case '1':
					printf("Ingrese señal beacon:\n");
				break;
				case '2':
					printf("Ingrese el modo de señal (puede ser repetido <0> o programado <1>):\n");
				break;
				case '3':
					printf("Ingrese el periodo de emisión [seg]:\n");
				break;
				case '4':
					printf("Ingrese el valor de la ventana [min]:\n");
				break;
				case '5':
					printf("Ingrese la hora de inicio de emisión:\n");
				break;
				case '6':
					display_help();
					display_menu();
				break;
				default:
					printf("\n\nOpcion no valida. Intente nuevamente.\n");
					printf("Si quiere ingresar un comando ingrese \"$\" antes del texto.\n");
					printf("Si quiere enviar un mensaje por LoRa ingrese \">\" antes del mensaje.\n\n");
					display_menu();
				break;
			}
		break;
		case menu_config_web:
			switch(numero_ingresado){
				case '0':
					pos_menu_actual = menu_main;
					display_menu();
				break;
				case '1':
					pos_menu_actual = menu_wifi_config;
					display_menu();
				break;
				case '2':
					pos_menu_actual = menu_firebase_config;
					display_menu();
				break;
				case '3':
					pos_menu_actual = menu_time_config;
					display_menu();
				break;
				case '4':
					display_help();
					display_menu();
				break;
				default:
					printf("\n\nOpcion no valida. Intente nuevamente.\n");
					printf("Si quiere ingresar un comando ingrese \"$\" antes del texto.\n");
					printf("Si quiere enviar un mensaje por LoRa ingrese \">\" antes del mensaje.\n\n");
					display_menu();
				break;
			}
		break;
		case menu_wifi_config:
			switch(numero_ingresado){
				case '0':
					pos_menu_actual = menu_config_web;
					display_menu();
				break;
				case '1':
					printf("Ingrese el SSD del Wi-Fi al que conectarse:\n");
				break;
				case '2':
					printf("Ingrese la contraseña del Wi-Fi al que conectarse:\n");
				break;
				case '3':
					display_help();
					display_menu();
				break;
				default:
					printf("\n\nOpcion no valida. Intente nuevamente.\n");
					printf("Si quiere ingresar un comando ingrese \"$\" antes del texto.\n");
					printf("Si quiere enviar un mensaje por LoRa ingrese \">\" antes del mensaje.\n\n");
					display_menu();
				break;
			}
		break;
		case menu_firebase_config:
			switch(numero_ingresado){
				case '0':
					pos_menu_actual = menu_config_web;
					display_menu();
				break;
				case '1':
					printf("Ingrese el Link de la base de datos al que conectarse:\n");
				break;
				case '2':
					printf("Reestableciendo configuración de la base de datos...\n");
					reset_config_dataBase();
				break;
				case '3':
					display_help();
					display_menu();
				break;
				default:
					printf("\n\nOpcion no valida. Intente nuevamente.\n");
					printf("Si quiere ingresar un comando ingrese \"$\" antes del texto.\n");
					printf("Si quiere enviar un mensaje por LoRa ingrese \">\" antes del mensaje.\n\n");
					display_menu();
				break;
			}
		break;
		case menu_time_config:
			switch(numero_ingresado){
				case '0':
					pos_menu_actual = menu_config_web;
					display_menu();
				break;
				case '1':
					printf("Ingrese fecha y hora en formato UTC:\n");
				break;
				case '2':
					printf("Ingrese fecha:\n");
				break;
				case '3':
					printf("Ingrese hora:\n");
				break;
				case '4':
					display_help();
					display_menu();
				break;
				default:
					printf("\n\nOpcion no valida. Intente nuevamente.\n");
					printf("Si quiere ingresar un comando ingrese \"$\" antes del texto.\n");
					printf("Si quiere enviar un mensaje por LoRa ingrese \">\" antes del mensaje.\n\n");
					display_menu();
				break;
			}
		break;
		default:
			pos_menu_actual = menu_main;
			display_menu();
		break;
	}
}

void reset_config_dataBase(void){

}

/**
 * @brief Muestra un mensaje de ayuda en función de la posición del menu en que se encuentra
 */
void display_help(void){
	switch (pos_menu_actual){
		case menu_main:
			printf("\n----------------------------------------------------------------------------------------\n");
			printf("\nPara navegar por el menú envíe números de 0 al 9.\n");
			printf("Si quiere envier un mensaje por única vez por LoRa escriba el caracter \">\" y a continuación el mensaje.\n");
			display_comds_list();
			printf("En la opción \"mensaje\" puede realizar configuraciones del envío de mensajes por LoRa.\n");
			printf("En la opción \"configuración web\" puede realizar configuraciones del Wi-Fi y la base de datos.\n");
		break;
		case menu_mensaje:
		break;
		case menu_config_web:
		break;
		case menu_wifi_config:
		break;
		case menu_firebase_config:
		break;
		case menu_time_config:
		break;
		default:
			printf("Error interno en posicionamiento del menu. Regresando a menu inicial...\n");
			pos_menu_actual = menu_main;
		break;
	}
}


/**
 * @brief Muestra la lista de comandos
 */
void display_comds_list(void){
	printf("\n----------------------------------------------------------------------------------------\n");
	printf("Los comandos se anteponen con un signo \"$\".\n\n");
	printf("Lista de comandos:\n");
	printf("	$help:\n");
	printf("		Describe los elementos mostrados en pantalla.\n\n");
	printf("	$ls $list y $tree:\n");
	printf("		Muestra la lista completa del menu en forma de archivos e indica la osición actual.\n\n");
	printf("	$cd <menu ID>:\n");
	printf("		Se mueve a la posición del ID del menu ingresado.\n");
	printf("		El ID de cada menu se puede ver con los comandos ls, list y tree.\n\n");
	printf("	$wifi y $wifi-mode:\n");
	printf("		Cambia el modo de funcionamiento a WiFi.\n");
	printf("		Cualquier interacción por USB realizada despues de este comando regresa el modo a USB.\n\n");
	printf("	$menu y $main:\n");
	printf("		Muestra el menu principal.\n");
	printf("\n----------------------------------------------------------------------------------------\n\n");
}



