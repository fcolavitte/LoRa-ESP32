/*
 * @file   : API_USB.c
 * @date   : Feb 15, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include "API_Time.h"
#include "API_USB.h"
#include <stdio.h>
#include <stdint.h>
#include <driver/uart.h>

/********************** macros and definitions *******************************/

typedef enum{
	USB_mode,
	WiFi_mode
}MEF_mode_t;

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/



/*		----- Funciones externas -----		*/
extern void reset_config_dataBase(void);



void analizar_input_USB(void);
void read_command(void);
void send_message(void);
void text_lower(uint8_t *text);
void display_tree(void);
pos_menu_t read_cd_ID(void);
void move_menu(uint8_t numero_ingresado);
void display_help(void);
void display_comds_list(void);
void display_help_main(void);

/********************** internal data definition *****************************/

uint8_t USB_input[USB_INPUT_LENGHT+1] = {};
int i_USB_input = 0;
uint8_t pass_to_WiFi=0;

/********************** external data definition *****************************/

extern pos_menu_t pos_menu_actual;
extern uint8_t fecha_actual[11];
extern uint32_t segundos_actuales;

/********************** internal functions definition ************************/

/**
 * @brief Analiza el String almacenado en USB_input[].
 *        Toma acciones dependiendo si es un comando, un mensaje a enviar o un número para navegar por el menú USB.
 *        Al finalizar limpia el String USB_input[].
 */
void analizar_input_USB(void){
	i_USB_input=0;
	if(USB_input[0]=='$'){
		read_command();
	}
	if(USB_input[0]=='>'){
		send_message();
	}
	if(USB_input[0]>='0' && USB_input[0]<='9'){
	    printf("Ingreso: %s\n", USB_input);
		USB_input[1]='\0';
		move_menu(USB_input[0]);
	}
	USB_input[0]='\0';
}


/**
 * @brief Revisa qué comando hay en el String USB_input[] y actua en consecuencia.
 */
void read_command(void){
	text_lower(&USB_input[1]);
	if(strstr((char *)(USB_input+1),"help")!=0){
		printf(">> Comando $help ingresado.\n");
		display_help_main();
	}
	else if(strstr((char *)(USB_input+1),"ls")!=0||strstr((char *)(USB_input+1),"list")!=0||strstr((char *)(USB_input+1),"tree")!=0){
		printf(">> Comando $ls ingresado.\n");
		display_tree();
	}
	else if(strstr((char *)(USB_input+1),"cd")!=0){
		printf(">> Comando $cd ingresado.\n");
		pos_menu_actual = read_cd_ID();
		display_menu();
	}
	else if(strstr((char *)(USB_input+1),"wifi")!=0||strstr((char *)(USB_input+1),"wifi-mode")!=0){
		printf(">> Comando $wifi ingresado.\n");
		pass_to_WiFi=1;
	}
	else if(strstr((char *)(USB_input+1),"menu")!=0||strstr((char *)(USB_input+1),"main")!=0){
		printf(">> Comando $menu ingresado.\n");
		pos_menu_actual = menu_main;
		display_menu();
	}
	else {
		printf(">> Comando no reconocido.\n");
	}
}


/**
 * @brief	Decodifica la cadena de texto recibida por serie según el comando $cd <ID>
 * @retval	Devuelve la posición del menú correspondiente según el salto generado con el comando $cd
 * @note	Si no se recibió el comando $cd y se ejecuta esta función se regresa al menú principal
 */
pos_menu_t read_cd_ID(void){
	uint8_t *indexIn = (uint8_t *)strstr((char *)(USB_input+1),"cd")+2;
	if(strlen((char *)indexIn)==0){
		return menu_main;
	}
	return atoi((char *)indexIn);
}


/**
 * @brief	Envía un mensaje por LoRa
 */
void send_message(void){
	printf("Enviando \"%s\" por LoRa...\n",USB_input+1);
}



/**
 * @brief Convierte todos los caracteres alfabéticos de un String en minúscula
 * @param Puntero a la dirección de inicio del String
 * @note  Largo máximo del String a analizar = USB_INPUT_LENGHT
 */
void text_lower(uint8_t *text){
	for(int i=0;i<USB_INPUT_LENGHT;i++){
		if(text[i]>='A'&&text[i]<='Z'){
			text[i]-='A'-'a';
		}
		if(text[i]==0){i=USB_INPUT_LENGHT;}
	}
}


/**
 * @brief Muestra por USB el tree de opciones de los menús partiendo desde el menú principal.
 * @note  Delante de cada elemento muestra el ID necesario para moverse a él usando el comando $cd
 */
void display_tree(void){
	printf("\n Menu principal\n");
	printf("  ├─ 1. Mensaje\n");
	printf("  │   ├─ 1.1. Señal Beacon\n");
	printf("  │   ├─ 1.2. Modo de Señal\n");
	printf("  │   ├─ 1.3. Periodo de emisión\n");
	printf("  │   ├─ 1.4. Ventana\n");
	printf("  │   └─ 1.5. Inicio de emisión\n");
	printf("  └─ 2. Configuración\n");
	printf("       ├─ 2.1. Módulo LoRa E22\n");
	printf("       │   ├─ 2.1.1. Network y Address del E22\n");
	printf("       │   ├─ 2.1.2. Velocidad de transmisión\n");
	printf("       │   ├─ 2.1.3. Potencia de transmisión\n");
	printf("       │   └─ 2.1.4. Frecuencia de transmisión\n");
	printf("       ├─ 2.2. WiFi Config\n");
	printf("       │   ├─ 2.2.1. SSD\n");
	printf("       │   └─ 2.2.2. WiFi PASS\n");
	printf("       ├─ 2.3. FireBase config\n");
	printf("       │   ├─ 2.3.1. Link\n");
	printf("       │   └─ 2.3.2. Reestablecer\n");
	printf("       └─ 2.4. Fecha y Hora\n");
	printf("            ├─ 2.4.1. Fecha\n");
	printf("            ├─ 2.4.2. Hora\n");
	printf("            └─ 2.4.3. Actualización automática\n");
}


/**
 * @brief	Cambia la posición actual del menú navegando según las opciones mostradas
 * @note	La navegación por el menú es a travéz de números
 * @param	Número recibido por serial para mueverse a travéz del menú
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
					pos_menu_actual = menu_lora_config;
					display_menu();
				break;
				case '2':
					pos_menu_actual = menu_wifi_config;
					display_menu();
				break;
				case '3':
					pos_menu_actual = menu_firebase_config;
					display_menu();
				break;
				case '4':
					pos_menu_actual = menu_time_config;
					display_menu();
				break;
				case '5':
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
			case menu_lora_config:
				switch(numero_ingresado){
					case '0':
						pos_menu_actual = menu_config_web;
						display_menu();
					break;
					case '1':
						printf("Ingrese el network al que pertenece y el Address del E22:\n");
					break;
					case '2':
						printf("Ingrese la velocidad de transmisión por aire del E22:\n");
					break;
					case '3':
						printf("Ingrese la potencia de transmisión del E22:\n");
					break;
					case '4':
						printf("Ingrese la frecuencia de la portadora del E22 con la que se transmitirá:\n");
					break;
					case '5':
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
					printf("Ingrese fecha:\n");
				break;
				case '2':
					printf("Ingrese hora:\n");
				break;
				case '3':
					get_UTP();
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


/**
 * @brief Muestra un mensaje de ayuda en función de la posición del menu en que se encuentra
 */
void display_help(void){
	switch (pos_menu_actual){
		case menu_main:
			display_help_main();
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
 * @brief Muestra el mensaje de ayuda para la posición del menu inicial
 */
void display_help_main(void){
	printf("\n----------------------------------------------------------------------------------------\n");
	printf("\nPara navegar por el menú envíe números de 0 al 9.\n");
	printf("Si quiere envier un mensaje por única vez por LoRa escriba el caracter \">\" y a continuación el mensaje.\n");
	display_comds_list();
	printf("En la opción \"mensaje\" puede realizar configuraciones del envío de mensajes por LoRa.\n");
	printf("En la opción \"configuración web\" puede realizar configuraciones del Wi-Fi y la base de datos.\n");
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
	printf("		Muestra la lista completa del menu en forma de archivos e indica la posición actual.\n\n");
	printf("	$cd <menu ID>:\n");
	printf("		Se mueve a la posición del ID del menu ingresado.\n");
	printf("		El ID se debe introducir sin puntos ni espacios dentro del número.\n");
	printf("		El ID de cada menu se puede ver con los comandos ls, list y tree.\n\n");
	printf("	$wifi y $wifi-mode:\n");
	printf("		Cambia el modo de funcionamiento a WiFi.\n");
	printf("		Cualquier interacción por USB realizada despues de este comando regresa el modo a USB.\n\n");
	printf("	$menu y $main:\n");
	printf("		Muestra el menu principal.\n");
	printf("\n----------------------------------------------------------------------------------------\n\n");
}


/********************** external functions definition ************************/

uint8_t USB_get_input(void){
    char ch[2] = {0,0};
    fgets(ch, 2, stdin); // @suppress("Field cannot be resolved") // @suppress("Symbol is not resolved")
    if(ch[0]==0){	/*Si no se recibe ningun caracter la función devuelve 0*/
    	return 0;
    }
    if(ch[0]=='\r'||ch[0]==';'){
    	ch[0]='\n';
    }
    if((ch[0]!=0)&&(i_USB_input<USB_INPUT_LENGHT)&&((ch[0]!='\n'))){
    	USB_input[i_USB_input]=ch[0];
    	USB_input[i_USB_input+1]='\0';
        i_USB_input++;
    }
    if((i_USB_input>=USB_INPUT_LENGHT)||(ch[0]=='\n')){
    	//Hacer un flush del buffer stdin
        analizar_input_USB();
    }
    return 1;
}


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
			printf("0. Atras\n1. LoRa Config\n2. WiFi Config\n3. FireBase config\n4. Fecha y Hora\n5. Ayuda\n");
		break;
		case menu_lora_config:
			printf("0. Atras\n1. Network y Address del E22\n2. Velocidad de transmisión"
					"\n3. Potencia de transmisión\n4. Frecuencia de transmisión\n5. Ayuda\n");
		break;
		case menu_wifi_config:
			printf("0. Atras\n1. SSD\n2. WiFi PASS\n3. Ayuda\n");
		break;
		case menu_firebase_config:
			printf("0. Atras\n1. Link\n2. Restablecer\n3. Ayuda\n");
		break;
		case menu_time_config:
			uint8_t _Hora[9]={};
			itoa(segundos_actuales/3600,(char *)_Hora,10);
			if(_Hora[1]==0){_Hora[1]=_Hora[0];_Hora[0]='0';}
			_Hora[2]=':';
			itoa((segundos_actuales/60)%60,(char *)_Hora+3,10);
			if(_Hora[4]==0){_Hora[4]=_Hora[3];_Hora[3]='0';}
			_Hora[5]=':';
			itoa(segundos_actuales%60,(char *)_Hora+6,10);
			if(_Hora[7]==0){_Hora[7]=_Hora[6];_Hora[6]='0';}
			_Hora[8]='\0';
			printf("0. Atras\n1. Fecha %s (AAA-MM-DD)\n2. Hora %s\n3. Actualizar automáticamente\n4. Ayuda\n",(char *)fecha_actual,(char *)_Hora);
		break;
		default:
			pos_menu_actual = menu_main;
			printf("1. Mensaje\n2. Configuración Web\n3. Ayuda\n");
		break;
	}
}

/********************** end of file ******************************************/
