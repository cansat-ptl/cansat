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
#define SD_DI   2
#define SD_DO   3
#define SD_CLK  1
#define SD_CS   6
int main(void){
	FRESULT res;
	//getTestValues();
	
	char msg[128]; 
	//DDRA |= (1 << PA3);
	
	uart0_init(51);
	uart1_init(51);
	//sd_readPtr();
	spi_init();
	twi_init();
	adc_init();
	adxl345_pinSetup();
	bmp280_pinSetup();
	nrf24_pinSetup();
	
	res = pf_mount(&fs);
	if(res == FR_OK){
		logMessage((char *)PSTR("[INIT]SD card mount: OK\r\n"), 1, 1);
		pf_lseek(0);
	}
	else logMessage((char *)PSTR("[INIT]SD card mount: ERR\r\n"), 1, 1);
	
//	res = pf_mount(&fs);
	
	sei();
	kernel_checkMCUCSR();
//	wdt_enable(WDTO_2S);
	logMessage((char *)PSTR("[INIT]Main: starting up\r\n"), 1, 1);
	wdt_reset();
	sprintf_P(msg, PSTR("[INIT]Main: using kernel version %s built %s\r\n"), KERNEL_VER, KERNEL_TIMESTAMP);
	logMessage((char *)msg, 1, 0);
	wdt_reset();
	sprintf_P(msg, PSTR("[INIT]Main: using UART driver version %s built %s\r\n"), UARTDRV_VER, UARTDRV_TIMESTAMP);
	logMessage((char *)msg, 1, 0);
	wdt_reset();
	sprintf_P(msg, PSTR("[INIT]Main: using SPI driver version %s built %s\r\n"), SPIDRV_VER, SPIDRV_TIMESTAMP);
	logMessage((char *)msg, 1, 0);
	
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