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
#define MAX_QUEUE_SIZE 16
#define ERR_QUEUE_OVERFLOW 1
#define ERR_QUEUE_END 2
#define ERR_WDT_RESET 3
#define ERR_BOD_RESET 4
#define TICKRATE 1 //in milliseconds
//#define DEBUG 1
#endif

#include "types.h"
#include "drivers.h"
#include "hal.h"
#include "debug.h"
#include "../tasks/tasks.h"
#include "watchdog.h"
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

uint8_t kernel_addCall(task t_ptr);
uint8_t kernel_addTask(task t_ptr, uint16_t t_period);
uint8_t kernel_removeCall();
uint8_t kernel_removeTask(uint8_t position);
void kernel_clearTaskQueue();
void kernel_clearCallQueue();
void kernel_displayError(uint8_t error);
void kernel_checkMCUCSR();
uint8_t kernelInit();

void kernel_stopTimer();
void kernel_startTimer();

void sd_puts(char * data);
void sd_flush();
void sd_readPtr();
void sd_init();

#endif /* KERNEL_H_ */