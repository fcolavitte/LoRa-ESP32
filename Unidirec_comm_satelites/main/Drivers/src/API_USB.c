/*
 * @file   : API_USB.c
 * @date   : Feb 15, 2023
 * @author : Colavitte Facundo G. <facundocolavitte@gmail.com>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include "API_Time.h"
#include "API_USB.h"
#include "API_E22.h"
#include "API_WiFi.h"
#include <stdio.h>
#include <stdint.h>
#include <driver/uart.h>

/********************** macros and definitions *******************************/

#define compile_firebase_menu 0

typedef enum{
	USB_mode,
	WiFi_mode
}MEF_mode_t;

/********************** internal data declaration ****************************/

pos_menu_t pos_menu_actual;

/********************** internal functions declaration ***********************/

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
void print_config_mensaje_programado (void);

/********************** internal data definition *****************************/

uint8_t USB_input[USB_INPUT_LENGHT+1] = {};
int i_USB_input = 0;
uint8_t pass_to_WiFi=0;
static message_mode_config_t _message_mode_config;

/********************** external data definition *****************************/

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
	else if(strstr((char *)(USB_input+1),"reset")!=0||strstr((char *)(USB_input+1),"reiniciar")!=0||strstr((char *)(USB_input+1),"restart")!=0){
		printf(">> Comando $reiniar ingresado.\n");
		printf(">> Reiniciando dispositivo...\n\n\n");
        vTaskDelay(2000/ portTICK_PERIOD_MS);
		/*Reinicio*/
        esp_restart();
	}else if(strstr((char *)(USB_input+1),"reconect")!=0||strstr((char *)(USB_input+1),"reconectar")!=0||strstr((char *)(USB_input+1),"conect")!=0){
		printf(">> Comando $reconect ingresado.\n");
		printf(">> Intentando reconexión de Wi-Fi.\n");
        reconectar_WiFi_manualmente();
	}else if(strstr((char *)(USB_input+1),"rx")!=0||strstr((char *)(USB_input+1),"rec")!=0||strstr((char *)(USB_input+1),"escuchar")!=0){
		printf(">> Comando $rx ingresado.\n");
		printf(">> Se pasa el módulo E22 a modo Rx continuo. Recibe constantemente por aire y lo guarda en su buffer interno.\n");
		printf(">> Para detener el modo Rx continuo se debe forzar el pasaje a IDLE o Tx o reiniciar el módulo E22.\n");
		driver_E22_recive_message();
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
	driver_E22_send_message(USB_input+1, sizeof(USB_input+1));
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
	printf("  └─ 2. Configuración del dispositivo\n");
	printf("       ├─ 2.1. Módulo LoRa E22\n");
	printf("       │   ├─ 2.1.1. Frecuencia de la portadora\n");
	printf("       │   ├─ 2.1.2. Velocidad de transmisión\n");
	printf("       │   ├─ 2.1.3. header_preambulo\n");
	printf("       │   ├─ 2.1.4. header_fixed_length\n");
	printf("       │   ├─ 2.1.5. Potencia de transmisión\n");
	printf("       ├─ 2.2. WiFi Config\n");
	printf("       │   ├─ 2.2.1. SSID\n");
	printf("       │   └─ 2.2.2. WiFi PASS\n");
	printf("       └─ 2.3. Fecha y Hora\n");
	printf("            ├─ 2.3.1. Fecha\n");
	printf("            ├─ 2.3.2. Hora\n");
	printf("            └─ 2.3.3. Actualización automática\n");
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
					printf("\nIngrese señal beacon. Largo máximo de 40 caracteres.\n");
					printf("Antes del mensaje se debe colocar el caracter 1 para confirmar.\n");
					pos_menu_actual = menu_mensaje_beacon;
				break;
				case '2':
					printf("\nIngrese el modo de señal (puede estar manual <1>, repetido(beacon) <2> o programado <3>):\n");
					pos_menu_actual = menu_mensaje_signal_mode;
				break;
				case '3':
					printf("\nIngrese el periodo de emisión [seg].\n");
					printf("El periodo debe estar entre 5 y 120 segundos.\n");
					pos_menu_actual = menu_mensaje_periodo_emision;
				break;
				case '4':
					printf("\nIngrese el valor de la ventana [min].\n");
					printf("La ventana debe estar entre 1 y 20 minutos.\n");
					pos_menu_actual = menu_mensaje_ventana_emision;
				break;
				case '5':
					printf("\nIngrese la hora de inicio de emisión.\n");
					printf("La hora debe estar en formato HH:MM:SS.\n");
					pos_menu_actual = menu_mensaje_start_emision;
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
			case menu_lora_config:
				switch(numero_ingresado){
					case '0':
						pos_menu_actual = menu_config_web;
						display_menu();
					break;
					case '1':
						printf("\nIngrese la frecuencia de la portadora del E22 con la que se transmitirá.\n");
						printf("La frecuencia debe estar en MHz, expresada en tres dígitos y ser un número entero.\n");
						printf("La frecuencia debe estar entre 850 y 930 MHz.\n");
						pos_menu_actual = menu_lora_frec_portadora;
					break;
					case '2':
						printf("\nIngrese la velocidad de transmisión por aire del E22.\n");
						printf("\n>> No implementado en este prototipo.\n");
						pos_menu_actual = menu_lora_baudrate_air;
					break;
					case '3':
						printf("\nIngrese el preambulo del mensaje LoRa.\n");
						printf("\nEl número debe ser entero y de hasta 4 caracteres.\n");
						pos_menu_actual = menu_lora_header_PreambleLength;
					break;
					case '4':
						printf("\nEl header del dataframe LoRa es de largo fijo?\n");
						printf("Opciones: 1 Sí | 2 No | 0 Cancelar\n");
						pos_menu_actual = menu_lora_header_fixed_length;
					break;
					case '5':
						printf("\nIngrese la potencia de transmisión del E22.\n");
						printf("\n>> No implementado en este prototipo.\n");
						pos_menu_actual = menu_lora_power_output;
					break;
					case '6':
						driver_E22_print_hexadecimal_ring_buffer();
						display_menu();
					break;
					case '7':
						driver_E22_print_caracteres_ring_buffer();
						display_menu();
					break;
					case '8':
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
					printf("\nIngrese el SSID del Wi-Fi al que conectarse.\n");
					printf("Debe haber un 1 antes del SSID para confirmar.\n");
					pos_menu_actual = menu_wifi_SSID;
				break;
				case '2':
					printf("\nIngrese la contraseña del Wi-Fi al que conectarse.\n");
					printf("Debe haber un 1 antes del PASS para confirmar.\n");
					pos_menu_actual = menu_wifi_PASS;
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
					printf("Ingrese la fecha.\n");
					printf("El formato debe ser DD/MM/AAAA.\n");
					pos_menu_actual = menu_time_set_fecha;
				break;
				case '2':
					printf("Ingrese la hora:\n");
					printf("El formato debe ser HH:MM:SS.\n");
					pos_menu_actual = menu_time_set_hora;
				break;
				case '3':
					get_UTP();
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
		/* ---------- Inicio menues de último orden: ---------- */
		/* -------- Esta sección no debe incluir printf ------- */
		/* Sub-menú mensajes programados */
			case menu_mensaje_beacon:
				if ('1' == USB_input[0]) {
					strcpy((char*)_message_mode_config.Mensaje, (char*)(&(USB_input[1])));
				}
				_message_mode_config.Mensaje[41] = '\0';
				pos_menu_actual = menu_mensaje;
				display_menu();
			break;
			case menu_mensaje_signal_mode:
				if ('1' == USB_input[0]) {
					_message_mode_config.message_mode = null;
				}
				if ('2' == USB_input[0]) {
					_message_mode_config.message_mode = continuo;
				}
				if ('3' == USB_input[0]) {
					_message_mode_config.message_mode = programado;
				}
				pos_menu_actual = menu_mensaje;
				display_menu();
			break;
			case menu_mensaje_periodo_emision:
				;
				uint8_t string_periodo[4];
				string_periodo[0] = USB_input[0];
				string_periodo[1] = USB_input[1];
				string_periodo[2] = USB_input[2];
				string_periodo[3] = '\0';
				uint32_t periodo = atoi((char*)string_periodo);
				if (5 <= periodo && 120 >= periodo) {
					_message_mode_config.Periodo_seconds = periodo;
				}
				pos_menu_actual = menu_mensaje;
				display_menu();
			break;
			case menu_mensaje_ventana_emision:
				;
				uint8_t string_ventana[3];
				string_ventana[0] = USB_input[0];
				string_ventana[1] = USB_input[1];
				string_ventana[2] = '\0';
				uint32_t ventana = atoi((char*)string_ventana);
				if (1 <= ventana && 20 >= ventana) {
					_message_mode_config.Ventana_minutes = ventana;
				}
				pos_menu_actual = menu_mensaje;
				display_menu();
			break;
			case menu_mensaje_start_emision:
				/* Formato esperado: HH:MM:SS */
				;
				uint32_t segundos_to_start_mensaje = 0;
				uint8_t string_aux[3];
				string_aux[2] = '\0';
				/* Horas */
				string_aux [0] = USB_input[0];
				string_aux [1] = USB_input[1];
				segundos_to_start_mensaje += atoi((char*)string_aux)*3600;
				/* Minutos */
				string_aux [0] = USB_input[3];
				string_aux [1] = USB_input[4];
				segundos_to_start_mensaje += atoi((char*)string_aux)*60;
				/* segundos*/
				string_aux [0] = USB_input[6];
				string_aux [1] = USB_input[7];
				segundos_to_start_mensaje += atoi((char*)string_aux);
				if (24*3600 > segundos_to_start_mensaje) {
					_message_mode_config.Time_inicio_programado_segundos = segundos_to_start_mensaje;
				}
				pos_menu_actual = menu_mensaje;
				display_menu();
			break;
		/* Sub-menú config LoRa */
		case menu_lora_frec_portadora:
			USB_input[3] = '\0';	/* Se deben ingresar 3 caracteres numéricos */
			uint32_t frec_deseada_MHz = (uint32_t)atoi((char*)USB_input);
			driver_E22_set_config_frec_deseada_MHz(frec_deseada_MHz);
			pos_menu_actual = menu_lora_config;
			display_menu();
		break;
		case menu_lora_baudrate_air:
			pos_menu_actual = menu_lora_config;
			display_menu();
		break;
		case menu_lora_header_PreambleLength:
			USB_input[4] = '\0';	/* Se deben ingresar 4 caracteres numéricos */
			uint16_t PreambleLength = (uint16_t)atoi((char*)USB_input);
			driver_E22_set_config_PreambleLength(PreambleLength);
			pos_menu_actual = menu_lora_config;
			display_menu();
		break;
		case menu_lora_header_fixed_length:
			if ('1' == USB_input[0]) {
				driver_E22_set_config_Header_is_fixed_length(1);
			}
			if ('2' == USB_input[0]) {
				driver_E22_set_config_Header_is_fixed_length(0);
			}
			pos_menu_actual = menu_lora_config;
			display_menu();
		break;
		case menu_lora_power_output:
			pos_menu_actual = menu_lora_config;
			display_menu();
		break;
		/* Sub-menú config Wi-Fi */
		case menu_wifi_SSID:
			if('1' == USB_input[0]){
				WiFi_set_SSID(&(USB_input[1]));
			}
			pos_menu_actual = menu_wifi_config;
			display_menu();
		break;
		case menu_wifi_PASS:
			if('1' == USB_input[0]){
				WiFi_set_PASS(&(USB_input[1]));
			}
			pos_menu_actual = menu_wifi_config;
			display_menu();
		break;
		/* Sub-menú config time */
		case menu_time_set_fecha:
			set_fecha_system_manualmente(USB_input);
			pos_menu_actual = menu_time_config;
			display_menu();
		break;
		case menu_time_set_hora:
			set_hora_system_manualmente(USB_input);
			pos_menu_actual = menu_time_config;
			display_menu();
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
	printf("	$reset y $reiniciar:\n");
	printf("		Reinicia el dispositivo.\n");
	printf("	$reconectar $reconect y $conectar:\n");
	printf("		Fuerza reconexión a Wi-Fi con las nuevas credenciales.\n");
	printf("	$rx $rec y $escuchar:\n");
	printf("		Pone al módulo E22 en modo Rx continuo.\n");
	printf("		El módulo guarda en el buffer interno todo lo que recibe de forma constante.\n");
	printf("\n----------------------------------------------------------------------------------------\n\n");
}


/**
 *	@brief	Muestra en pantalla la configuración actual de los mensajes programados
 */
void print_config_mensaje_programado (void) {
	printf("\n>> Configuración actual de los mensajes programados:\n");
	printf("    Mensaje a enviar: %s\n", _message_mode_config.Mensaje);
	printf("    Modo de emisión: ");
	if (null == _message_mode_config.message_mode) {
		printf("Manual (modo envío configurado OFF)\n");
	}
	if (continuo == _message_mode_config.message_mode) {
		printf("Continuo/Beacon\n");
	}
	if (programado == _message_mode_config.message_mode) {
		printf("Programado\n");
	}
	printf("    Periodo de emisión: %d\n", _message_mode_config.Periodo_seconds);
	printf("    Ventana de emisión: %d\n", _message_mode_config.Ventana_minutes);
	printf("    Inicio de emisión: ");//, _message_mode_config.Time_inicio_programado_segundos);
	uint32_t hora = _message_mode_config.Time_inicio_programado_segundos/3600;
	printf("%02d:", hora);
	uint32_t minutos = (_message_mode_config.Time_inicio_programado_segundos-hora*3600)/60;
	printf("%02d:", minutos);
	uint32_t segundos = _message_mode_config.Time_inicio_programado_segundos-hora*3600-minutos*60;
	printf("%02d\n", segundos);
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
			printf("1. Mensaje\n2. Configuración del dispositivo\n3. Ayuda\n");
		break;
		case menu_mensaje:
			printf("0. Atras                        Valores actuales:\n");
			printf("1. Grabar mensaje               ■ %s\n", _message_mode_config.Mensaje);
			printf("2. Modo de emisión automática   ■ ");
			if (null == _message_mode_config.message_mode) {
				printf("Manual (modo envío configurado OFF)\n");
			}
			if (continuo == _message_mode_config.message_mode) {
				printf("Continuo/Beacon\n");
			}
			if (programado == _message_mode_config.message_mode) {
				printf("Programado\n");
			}
			printf("3. Periodo de emisión           ■ %d\n", _message_mode_config.Periodo_seconds);
			printf("4. Ventana de emisión           ■ %d\n",_message_mode_config.Ventana_minutes);
			printf("5. Inicio de emisión            ■ ");
			uint32_t hora = _message_mode_config.Time_inicio_programado_segundos/3600;
			printf("%02d:", hora);
			uint32_t minutos = (_message_mode_config.Time_inicio_programado_segundos-hora*3600)/60;
			printf("%02d:", minutos);
			uint32_t segundos = _message_mode_config.Time_inicio_programado_segundos-hora*3600-minutos*60;
			printf("%02d\n", segundos);
			printf("6. Ayuda\n");
			/* Mostrar configuración actual de los mensajes grabados */
			/* print_config_mensaje_programado(); */
		break;
		case menu_config_web:
			printf("0. Atras\n1. LoRa Config\n2. WiFi Config\n3. Fecha y Hora\n4. Ayuda\n");
		break;
		case menu_lora_config:
			printf("0. Atras\n1. Frecuencia de transmisión\n2. Velocidad de transmisión\n"
					"3. PreambleLength\n4. Header_is_fixed_length\n5. Potencia de transmisión\n"
					"6. Mostrar contenido del buffer interno del E22 en hexadecimal\n"
					"7. Mostrar contenido del buffer interno del E22 en caracteres\n8. Ayuda\n");
			/* Mostrar configuración actual del módulo E22 */
			driver_E22_print_configuracion();
		break;
		case menu_wifi_config:
			printf("0. Atras\n");
			printf("1. SSID        ■ %s\n", get_SSID_pointer());
			printf("2. WiFi PASS\n3. Ayuda\n");
		break;
		case menu_time_config:
			printf("0. Atras\n1. Fecha %s\n2. Hora %s\n3. Actualizar automáticamente\n4. Ayuda\n",
					(char *)get_string_fecha(),(char *)get_string_hora());
		break;
		default:
			pos_menu_actual = menu_main;
			printf("1. Mensaje\n2. Configuración Web\n3. Ayuda\n");
		break;
	}
}


void get_estruct_message_mode_config(message_mode_config_t * estructura_donde_guardar_la_info) {
	estructura_donde_guardar_la_info->Periodo_seconds = _message_mode_config.Periodo_seconds;
	estructura_donde_guardar_la_info->Time_inicio_programado_segundos = _message_mode_config.Time_inicio_programado_segundos;
	estructura_donde_guardar_la_info->Ventana_minutes = _message_mode_config.Ventana_minutes;
	estructura_donde_guardar_la_info->message_mode = _message_mode_config.message_mode;
	strcpy((char*)estructura_donde_guardar_la_info->Mensaje, (char*)_message_mode_config.Mensaje);
}


void set_posicion_menu_actual_main (void) {
	pos_menu_actual = menu_main;
}

void set_estructura_message_mode_config(message_mode_config_t * estructura_con_la_info) {
	_message_mode_config.Periodo_seconds = estructura_con_la_info->Periodo_seconds;
	_message_mode_config.Time_inicio_programado_segundos = estructura_con_la_info->Time_inicio_programado_segundos;
	_message_mode_config.Ventana_minutes = estructura_con_la_info->Ventana_minutes;
	_message_mode_config.message_mode = estructura_con_la_info->message_mode;
	strcpy((char*)_message_mode_config.Mensaje, (char*)estructura_con_la_info->Mensaje);
}

/********************** end of file ******************************************/
