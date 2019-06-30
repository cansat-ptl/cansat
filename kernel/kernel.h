/*
 * kernel.h
 *
 * Created: 11.05.2019 21:15:24
 *  Author: ThePetrovich
 */ 

#ifndef KERNEL_H_
#define KERNEL_H_

#define KERNEL_VER "0.2.0-rc1"
#define KERNEL_TIMESTAMP __TIMESTAMP__

#define SDCARD_MOD_VER "0.0.4-bleeding"
#define SDCARD_MOD_TIMESTAMP __TIMESTAMP__

#ifndef KERNELconfig
	#define MAX_QUEUE_SIZE 32
	#define ERR_QUEUE_OVERFLOW 1
	#define ERR_QUEUE_END 2
	#define ERR_WDT_RESET 3
	#define ERR_BOD_RESET 4
	#define PRIORITY_HIGH 0
	#define PRIORITY_MID 1
	#define PRIORITY_LOW 2
	#define TICKRATE 1 //in milliseconds
	
	#define KERNEL_SD_MODULE 1
	#define KERNEL_WDT_MODULE 1
	#define KERNEL_UTIL_MODULE 1
#endif

#include "types.h"
#include "hal.h"
#include "debug.h"
#include "../tasks/tasks.h"
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "../drivers/uart.h"
#include "../drivers/spi.h"
#include "../drivers/twi.h"
#include "../drivers/adc.h"
#include "../drivers/devices/sensors/adxl345.h"
#include "../drivers/devices/sensors/bmp280.h"
#include "../drivers/devices/sensors/ds18b20.h"
#include "../drivers/devices/radio/nrf24.h"
#include "../external/pololu-driver/imuv3.h"
#include "../external/pff3a/pff.h"
#include "../external/pff3a/diskio.h"
#include "../external/nRF/nRF.h"

FATFS fs;
WORD logfile;

uint8_t kernel_addCall(task t_ptr, uint8_t t_priority);
uint8_t kernel_addTask(task t_ptr, uint16_t t_period, uint8_t t_priority);
uint8_t kernel_removeCall(uint8_t t_priority);
uint8_t kernel_removeTask(uint8_t position);
void kernel_clearTaskQueue();
void kernel_clearCallQueue(uint8_t t_priority);
void kernel_displayError(uint8_t error);
void kernel_checkMCUCSR();
uint8_t kernelInit();

void kernel_stopTimer();
void kernel_startTimer();

#if KERNEL_SD_MODULE == 1
	void sd_puts(char * data);
	void sd_flush();
	void sd_readPtr();
	void sd_init();
#endif

#if KERNEL_WDT_MODULE == 1
	void wdt_saveMCUCSR(void) __attribute__((naked)) __attribute__((section(".init3")));
	//void wdt_disableWatchdog();
	void wdt_enableWatchdog();
#endif

#if KERNEL_UTIL_MODULE == 1
	void util_printVersion();
#endif

#endif /* KERNEL_H_ */