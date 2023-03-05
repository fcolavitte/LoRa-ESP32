/*
 * API_USB.c
 *
 *  Created on: 15 feb. 2023
 *      Author: Facundo
 */

#include "API_USB.h"

void analizar_input_USB(void);
void read_command(void);
void send_message(void);
void text_lower(uint8_t *text);
void display_tree(void);

typedef enum{
	menu_main 		= 0,
	menu_mensaje 	= 1,
	menu_config_web = 2,
	menu_wifi_config 	 = 21,
	menu_firebase_config = 22,
	menu_time_config 	 = 23
}pos_menu_t;

typedef enum{
	USB_mode,
	WiFi_mode
}MEF_mode_t;

extern void move_menu(uint8_t numero_ingresado);
extern void display_help(void);
extern pos_menu_t pos_menu_actual;
extern MEF_mode_t MEF_mode;
uint8_t USB_input[USB_INPUT_LENGHT+1] = {};
int i_USB_input = 0;
uint8_t pass_to_WiFi=0;


uint8_t USB_get_input(void){
    char ch[2] = {0,0};
    fgets(ch, 2, stdin);
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

void read_command(void){
	text_lower(&USB_input[1]);
	if(strstr((char *)(USB_input+1),"help")!=0){
		printf(">> Comando $help ingresado.\n");
		display_help();
	}
	else if(strstr((char *)(USB_input+1),"ls")!=0||strstr((char *)(USB_input+1),"list")!=0||strstr((char *)(USB_input+1),"tree")!=0){
		printf(">> Comando $ls ingresado.\n");
		display_tree();
	}
	else if(strstr((char *)(USB_input+1),"cd")!=0){
		printf(">> Comando $cd ingresado.\n");
	}
	else if(strstr((char *)(USB_input+1),"wifi")!=0||strstr((char *)(USB_input+1),"wifi-mode")!=0){
		printf(">> Comando $wifi ingresado.\n");
		pass_to_WiFi=1;
	}
	else if(strstr((char *)(USB_input+1),"menu")!=0||strstr((char *)(USB_input+1),"main")!=0){
		printf(">> Comando $menu ingresado.\n");
		pos_menu_actual = menu_main;
	}
	else {
		printf(">> Comando no reconocido.\n");
	}
}



void send_message(void){
	printf("Enviando \"%s\" por LoRa...\n",USB_input+1);
}

void text_lower(uint8_t *text){
	for(int i=0;i<USB_INPUT_LENGHT;i++){
		if(text[i]>='A'&&text[i]<='Z'){
			text[i]-='A'-'a';
		}
		if(text[i]==0){i=USB_INPUT_LENGHT;}
	}
}

void display_tree(void){
	printf("Menu principal\n");
	printf(" ├─1. Mensaje\n");
	printf(" │   ├─1.1. Señal Beacon\n");
	printf(" │   ├─1.2. Modo de Señal\n");
	printf(" │   ├─1.3. Periodo de emisión\n");
	printf(" │   ├─1.4. Ventana\n");
	printf(" │   └─1.5. Inicio de emisión\n");
	printf(" └─2. Configuración\n");
	printf("      ├─2.1. Módulo LoRa E22\n");
	printf("      │   ├─2.1.1. Network y Address del E22\n");
	printf("      │   ├─2.1.2. Velocidad de transmisión\n");
	printf("      │   ├─2.1.3. Potencia de transmisión\n");
	printf("      │   └─2.1.4. Frecuencia de transmisión\n");
	printf("      ├─2.2. WiFi Config\n");
	printf("      │   ├─2.2.1. SSD\n");
	printf("      │   └─2.2.2. WiFi PASS\n");
	printf("      ├─2.3. FireBase config\n");
	printf("      │   ├─2.3.1. Link\n");
	printf("      │   └─2.3.2. Reestablecer\n");
	printf("      └─2.4. Fecha y Hora\n");
	printf("           ├─2.4.1. Fecha\n");
	printf("           ├─2.4.2. Hora\n");
	printf("           └─2.4.3. Actualización automática\n");
}
