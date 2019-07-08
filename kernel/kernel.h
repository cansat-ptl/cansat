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
	#define TICKRATE 1 //in milliseconds
	
	#define ERR_QUEUE_OVERFLOW 1
	#define ERR_QUEUE_END 2
	#define ERR_WDT_RESET 3
	#define ERR_BOD_RESET 4
	#define ERR_KRN_RETURN 5
	
	#define PRIORITY_HIGH 0
	#define PRIORITY_MID 1
	#define PRIORITY_LOW 2
	
	#define KFLAG_INIT 0
	#define KFLAG_DEBUG 15
	
	#define KERNEL_SD_MODULE 1
	#define KERNEL_WDT_MODULE 1
	#define KERNEL_UTIL_MODULE 1
	#define KERNEL_DEBUG_MODULE 1
#endif

#include "types.h"
#include "hal.h"
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

#include "../drivers/interfaces/uart.h"
#include "../drivers/interfaces/spi.h"
#include "../drivers/interfaces/twi.h"
#include "../drivers/interfaces/adc.h"
#include "../drivers/devices/sensors/adxl345/adxl345.h"
#include "../drivers/devices/sensors/bmp280/bmp280.h"
#include "../drivers/devices/sensors/ds18b20/ds18b20.h"
#include "../drivers/devices/sensors/pololu/imuv3.h"
#include "../drivers/devices/radio/nRF.h"
#include "../drivers/devices/system/pff3a/pff.h"
#include "../drivers/devices/system/pff3a/diskio.h"

FATFS fs;
WORD logfile;

void kernel_setFlag(uint8_t flag, uint8_t value);
uint8_t kernel_checkFlag(uint8_t flag);
uint64_t kernel_getUptime();

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

#if KERNEL_DEBUG_MODULE == 1
	#define DBG_MOD_VER "0.9.1-staging"
	#define DBG_MOD_TIMESTAMP __TIMESTAMP__
	#define UART_LOGGING 1
	void debug_sendMessage(char* msg, uint8_t level);
	void debug_sendMessage_i(char* msg, uint8_t level);
	void debug_sendMessageSD(char* msg, uint8_t level);
	void debug_sendMessage_p(const char * msg, uint8_t level);
	void debug_sendMessage_pi(const char * msg, uint8_t level);
	void debug_sendMessageSD_p(const char * msg, uint8_t level);
	void debug_logMessage(char* msg, uint8_t level, uint8_t pgm);
#endif

#endif /* KERNEL_H_ */