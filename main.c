/*
 * main.c
 *
 * Created: 07.03.2019 17:56:34
 *  Author: ThePetrovich
 */ 


#include "config.h"

int main(void){
	char msg[128]; 
	uart0_init(51);
	//spi_init();
	//tc72_init();
	sei();
	sprintf(msg, "[INIT]Main: starting up\r\n");
	logMessage(msg, 1);
	sprintf(msg, "[INIT]Main: using kernel version %s built %s\r\n", KERNEL_VER, KERNEL_TIMESTAMP);
	logMessage(msg, 1);
	sprintf(msg, "[INIT]Main: using UART driver version %s built %s\r\n", UARTDRV_VER, UARTDRV_TIMESTAMP);
	logMessage(msg, 1);
	sprintf(msg, "[INIT]Main: using SPI driver version %s built %s\r\n", SPIDRV_VER, SPIDRV_TIMESTAMP);
	logMessage(msg, 1);
	
	adxl345_test();
	bmp280_test();
	ds18b20_test();
	
	kernelInit();
}