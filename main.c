/*
 * main.c
 *
 * Created: 07.03.2019 17:56:34
 *  Author: ThePetrovich
 */ 

#include "config.h"

int main(void){
	hal_enableInterrupts();
	wdt_enable(WDTO_2S);	
	kernelInit();
}
