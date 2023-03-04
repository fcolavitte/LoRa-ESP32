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

extern void move_menu(uint8_t numero_ingresado);



uint8_t USB_input[USB_INPUT_LENGHT+1] = {};
int i_USB_input = 0;



void USB_get_input(void){
    char ch[2] = {};
    fgets(ch, 2, stdin);
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
}

void analizar_input_USB(void){
	i_USB_input=0;
    printf("Ingreso: %s\n", USB_input);
	if(USB_input[0]=='$'){
		read_command();
	}
	if(USB_input[0]=='>'){
		send_message();
	}
	if(USB_input[0]>='0' && USB_input[0]<='9'){
		USB_input[1]='\0';
		move_menu(USB_input[0]);
	}
	USB_input[0]='\0';
}

void read_command(void){
	printf("Ejecutando comando...\n");
}

void send_message(void){
	printf("Enviando \"%s\" por LoRa...\n",USB_input+1);
}
