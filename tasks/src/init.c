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

void init(){
	uart0_init(51);
	PORTA = 0;
	hal_enableInterrupts();
	//	getTestValues();
	hal_setupPins();
	delay(10);
	if(hal_checkBit_m(JUMPER_PIN, JUMPER_IN)) debug = 1;
	
	debug_sendMessage_p((char *)PSTR("[INIT]initd: SD card init            [OK]\r\n"), 1);
	sd_init();
	wdt_reset();
	
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
	
	nrf24_pinSetup();
	debug_logMessage((char *)PSTR("[INIT]initd: NRF24 IO setup          [OK]\r\n"), 1, 1);
	wdt_reset();
	
	debug_logMessage((char *)PSTR("[INIT]initd: hardware autotest\r\n"), 1, 1);
	autotest();
	debug_logMessage((char *)PSTR("[INIT]initd: hardware autotest complete\r\n"), 1, 1);
	wdt_reset();
	
	debug_logMessage((char *)PSTR("[INIT]initd: Task system setup..."), 1, 1);
	kernel_addTask(checkDeployment, 10);
	kernel_addTask(readADXL, 20);
	kernel_addTask(readBMP, 40);
	kernel_addTask(readGPS, 60);
	kernel_addTask(sendGPS, 80);
	kernel_addTask(sendMain, 100);
	//kernel_addTask(readDS18, 70);
	kernel_addTask(imu_filter, 100);
	debug_logMessage((char *)PSTR("DONE!\r\n"), 0, 1);
	wdt_reset();
	
	debug_logMessage((char *)PSTR("[INIT]initd: init process finished\r\n"), 1, 1);
	
	util_printVersion();
	
	kernel_checkMCUCSR();
	
}