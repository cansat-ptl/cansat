/*
 * utils.c
 *
 * Created: 23.06.2019 10:27:23
 *  Author: WorldSkills-2019
 */ 
#include "../kernel.h"
#include "../drivers.h"

void util_printVersion(){
	char msg[128];
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]Kernel: starting up\r\n"));
	wdt_reset();
	sprintf_P(msg, PSTR("[INIT]Kernel: using kernel version %s built %s\r\n"), KERNEL_VER, KERNEL_TIMESTAMP);
	debug_logMessage(PGM_OFF, L_INFO, "%s", msg);
	wdt_reset();
	sprintf_P(msg, PSTR("[INIT]Kernel: using debug module version %s built %s\r\n"), DBG_MOD_VER, DBG_MOD_TIMESTAMP);
	debug_logMessage(PGM_OFF, L_INFO, "%s", msg);
	wdt_reset();
	sprintf_P(msg, PSTR("[INIT]Kernel: using HAL module version %s built %s\r\n"), HAL_MOD_VER, HAL_MOD_TIMESTAMP);
	debug_logMessage(PGM_OFF, L_INFO, "%s", msg);
	wdt_reset();
	sprintf_P(msg, PSTR("[INIT]Kernel: using UART interface driver version %s built %s\r\n"), UARTDRV_VER, UARTDRV_TIMESTAMP);
	debug_logMessage(PGM_OFF, L_INFO, "%s", msg);
	wdt_reset();
	sprintf_P(msg, PSTR("[INIT]Kernel: using SPI interface driver version %s built %s\r\n"), SPIDRV_VER, SPIDRV_TIMESTAMP);
	debug_logMessage(PGM_OFF, L_INFO, "%s", msg);
	wdt_reset();
	sprintf_P(msg, PSTR("[INIT]Kernel: using TWI interface driver version %s built %s\r\n"), TWIDRV_VER, TWIDRV_TIMESTAMP);
	debug_logMessage(PGM_OFF, L_INFO, "%s", msg);
	wdt_reset();
	sprintf_P(msg, PSTR("[INIT]Kernel: using ADXL345 device driver version %s built %s\r\n"), ADXLDRV_VER, ADXLDRV_TIMESTAMP);
	debug_logMessage(PGM_OFF, L_INFO, "%s", msg);
	wdt_reset();
	sprintf_P(msg, PSTR("[INIT]Kernel: using BMP280 device driver version %s built %s\r\n"), BMPDRV_VER, BMPDRV_TIMESTAMP);
	debug_logMessage(PGM_OFF, L_INFO, "%s", msg);
	wdt_reset();
	sprintf_P(msg, PSTR("[INIT]Kernel: using DS18B20 device driver version %s built %s\r\n"), DS18DRV_VER, DS18DRV_TIMESTAMP);
	debug_logMessage(PGM_OFF, L_INFO, "%s", msg);
	wdt_reset();
}