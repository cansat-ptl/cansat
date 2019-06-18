/*
 * kernel.h
 *
 * Created: 11.05.2019 21:15:24
 *  Author: ThePetrovich
 */ 


#ifndef KERNEL_H_
#define KERNEL_H_

#ifndef F_CPU
//#warning "F_CPU is not defined"
#define F_CPU 8000000L
#endif

#define KERNEL_VER "0.1.4-bleeding"
#define KERNEL_TIMESTAMP __TIMESTAMP__

#ifndef KERNELconfig
#define MAX_QUEUE_SIZE 16
#define ERR_QUEUE_OVERFLOW 1
#define ERR_QUEUE_END 2
//#define DEBUG 1
#endif

#include "types.h"
#include "hal.h"
#include "../tasks/tasks.h"
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <avr/wdt.h>

//Drivers to load
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

uint8_t kernel_addCall(task t_ptr);
uint8_t kernel_addTask(task t_ptr, uint8_t t_period);
uint8_t kernel_removeCall();
uint8_t kernel_removeTask(uint8_t position);
void kernel_clearTaskQueue();
void kernel_clearCallQueue();
void kernel_checkMCUCSR();
uint8_t kernelInit();

void kernel_stopTimer();
void kernel_startTimer();

void wdt_saveMCUCSR(void) __attribute__((naked)) __attribute__((section(".init3")));
void wdt_disableWatchdog();
void wdt_enableWatchdog();

void debugMessage(char* msg, uint8_t level);
void debugMessage_i(char* msg, uint8_t level);
void debugMessageSD(char* msg, uint8_t level);
void logMessage(char* msg, uint8_t level);
#endif /* KERNEL_H_ */