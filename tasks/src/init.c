/*
 * init.c
 *
 * Created: 14.06.2019 19:30:23
 *  Author: ThePetrovich
 */ 

#include "../tasks.h"
#include "../../kernel/kernel.h"
#include "../../kernel/utils.h"
#include "../../kernel/globals.h"

uint16_t tflags = 0;
void w2_init();

void initTaskManager(){
	kernel_addTask(checkDeployment, 100);
	kernel_addTask(readADXL, 200);
	kernel_addTask(readBMP, 400);
	kernel_addTask(readGPS, 600);
	kernel_addTask(sendGPS, 800);
	kernel_addTask(sendMain, 1000);
	kernel_addTask(sendOrient, 1200);
	kernel_addTask(imu_filter, 1000);
	kernel_addTask(requestDS18, 700);
}

void init(){
	hal_setupPins();
	uart0_init(12);
	hal_enableInterrupts();
	//	getTestValues();
	delay(10);
	if(hal_checkBit_m(JUMPER_PIN, JUMPER_IN)) debug = 0;
	if(debug) hal_writePin(&LED_DBG_PORT, LED_DBG, 1);
	else hal_writePin(&LED_DBG_PORT, LED_DBG, 0);
	
	sd_init();
	wdt_reset();
	kernel_checkMCUCSR();
	
	uart1_init(51);
	debug_logMessage((char *)PSTR("[INIT]initd: uart1 interface init    [OK]\r\n"), 1, 1);
	wdt_reset();
	
	//sd_readPtr();
	spi_init();
	debug_logMessage((char *)PSTR("[INIT]initd: SPI0 interface init     [OK]\r\n"), 1, 1);
	wdt_reset();
	
	TWBR = 2;
	TWSR = 0x00;
	PORTD |= (1<<PD0|1<<PD1);
	DDRD &= ~(1<<PD0) & (1<<PD1);
	TWCR = (1<<TWINT) | (1<<TWEN);
	debug_logMessage((char *)PSTR("[INIT]initd: TWI0 interface init     [OK]\r\n"), 1, 1);
	wdt_reset();
	
	adc_init();
	debug_logMessage((char *)PSTR("[INIT]initd: ADC init                [OK]\r\n"), 1, 1);
	wdt_reset();
	
	adxl345_pinSetup();
	debug_logMessage((char *)PSTR("[INIT]initd: ADXL345 IO setup        [OK]\r\n"), 1, 1);
	wdt_reset();
	
	bmp280_pinSetup();
	debug_logMessage((char *)PSTR("[INIT]initd: BMP280 IO setup         [OK]\r\n"), 1, 1);
	wdt_reset();
	
	nRF_init(0x4C + 2400);
	debug_logMessage((char *)PSTR("[INIT]initd: NRF24 IO setup          [OK]\r\n"), 1, 1);
	wdt_reset();
	
	debug_logMessage((char *)PSTR("[INIT]initd: hardware autotest\r\n"), 1, 1);
	autotest();
	debug_logMessage((char *)PSTR("[INIT]initd: hardware autotest complete\r\n"), 1, 1);
	wdt_reset();
	
	debug_logMessage((char *)PSTR("[INIT]initd: Task system setup..."), 1, 1);
	initTaskManager();
	debug_logMessage((char *)PSTR("DONE!\r\n"), 0, 1);
	wdt_reset();
	
	debug_logMessage((char *)PSTR("[INIT]initd: init process finished\r\n"), 1, 1);
	
	//util_printVersion();
	//imu_setupTimer();
}
