/*
 * init.c
 *
 * Created: 14.06.2019 19:30:23
 *  Author: ThePetrovich
 */ 

#include "../tasks.h"
#include "../../kernel/kernel.h"

uint16_t tflags = 0;
void w2_init();

void initTaskManager(){
	kernel_addTask(imu_filter, 50, PRIORITY_HIGH);
	kernel_addTask(checkDeployment, 100, PRIORITY_LOW);
	kernel_addTask(readADXL, 200, PRIORITY_MID);
	kernel_addTask(readBMP, 400, PRIORITY_MID);
	kernel_addTask(readGPS, 600, PRIORITY_HIGH);
	kernel_addTask(sendGPS, 800, PRIORITY_LOW);
	kernel_addTask(sendMain, 1000, PRIORITY_LOW);
	kernel_addTask(sendOrient, 1200, PRIORITY_LOW);
	kernel_addTask(requestDS18, 700, PRIORITY_MID);
}

void init(){
	kernel_setFlag(KFLAG_INIT, 1);
	hal_setupPins();
	uart0_init(12);
	sd_init();
	getTestValues();
	
	hal_enableInterrupts();
	delay(10);
	
	if(hal_checkBit_m(JUMPER_PIN, JUMPER_IN)){
		kernel_setFlag(KFLAG_DEBUG, 1);
		hal_writePin(&LED_DBG_PORT, LED_DBG, HIGH);
	}
	else hal_writePin(&LED_DBG_PORT, LED_DBG, LOW);
	
	wdt_reset();
	kernel_checkMCUCSR();
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: uart1 interface init"));
	uart1_init(51);
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("    [OK]\r\n"));
	wdt_reset();
	
	//sd_readPtr();
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: SPI0 interface init"));
	spi_init();
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("     [OK]\r\n"));
	wdt_reset();
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: TWI0 interface init"));
	TWBR = 2;
	TWSR = 0x00;
	PORTD |= (1<<PD0|1<<PD1);
	DDRD &= ~(1<<PD0) & (1<<PD1);
	TWCR = (1<<TWINT) | (1<<TWEN);
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("     [OK]\r\n"));
	wdt_reset();
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: ADC init"));
	adc_init();
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("                [OK]\r\n"));
	wdt_reset();
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: ADXL345 IO setup"));
	adxl345_pinSetup();
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("        [OK]\r\n"));
	wdt_reset();
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: BMP280 IO setup"));
	bmp280_pinSetup();
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("         [OK]\r\n"));
	wdt_reset();
	
	//debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: BMP280 alt calibration"));
	//bmp280_readTemperature();
	//bmp280_readPressure();
	//altitude_init = (uint16_t)bmp280_calcAltitude(101325);
	//debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("  [OK]\r\n"));
	//wdt_reset();
	
	//debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: NRF24 setup"));
	//nRF_init(0x4C + 2400);
	//debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("             [OK]\r\n"));
	//wdt_reset();
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: hardware autotest\r\n"));
	autotest();
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: hardware autotest complete\r\n"));
	wdt_reset();
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: Task system setup..."));
	initTaskManager();
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("DONE!\r\n"), 0, 1);
	wdt_reset();
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: init process finished\r\n"));
	
	//util_printVersion();
	//imu_setupTimer();
	kernel_setFlag(KFLAG_INIT, 0);
}
