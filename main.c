/*
 * main.c
 *
 * Created: 07.03.2019 17:56:34
 *  Author: ThePetrovich
 */ 


#include "config.h"
#include "kernel/globals.h"
extern volatile struct GPS_t GPS;
extern uint16_t tests_r;

int main(void){
	getTestValues();
	char msg[128]; 
	uart0_init(51);
	uart1_init(51);
	spi_init();
	twi_init();
	adc_init();
	//tc72_init();
	sei();
	kernel_checkMCUCSR();
	wdt_enable(WDTO_2S);
	sprintf(msg, "[INIT]Main: starting up\r\n");
	logMessage(msg, 1, 1);
	wdt_reset();
	sprintf(msg, "[INIT]Main: using kernel version %s built %s\r\n", KERNEL_VER, KERNEL_TIMESTAMP);
	logMessage(msg, 1, 1);
	wdt_reset();
	sprintf(msg, "[INIT]Main: using UART driver version %s built %s\r\n", UARTDRV_VER, UARTDRV_TIMESTAMP);
	logMessage(msg, 1, 1);
	wdt_reset();
	sprintf(msg, "[INIT]Main: using SPI driver version %s built %s\r\n", SPIDRV_VER, SPIDRV_TIMESTAMP);
	logMessage(msg, 1, 1);
	wdt_reset();
	setPinMode(&DDRG, PG3, OUTPUT);
	
	if(!checkBit_m(tests_r, ADXL_TESTED))
		adxl345_test();
	if(!checkBit_m(tests_r, BMP_TESTED))
		bmp280_test();
	if(!checkBit_m(tests_r, DS_TESTED))
		ds18b20_test();
	if(!checkBit_m(tests_r, IMU_TESTED))
		imu_test();
	if(!checkBit_m(tests_r, GPS_TESTED))
		gps_test();
	
	kernelInit();
}