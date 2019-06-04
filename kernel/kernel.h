/*
 * kernel.h
 *
 * Created: 11.05.2019 21:15:24
 *  Author: ThePetrovich
 */ 


#ifndef KERNEL_H_
#define KERNEL_H_

#define KERNEL_VER "0.1.1-bleeding"
#define KERNEL_TIMESTAMP __TIMESTAMP__

#ifndef KERNELconfig
#define MAX_QUEUE_SIZE 16
#define ERR_QUEUE_OVERFLOW 1
#define ERR_QUEUE_END 2
//#define DEBUG 1
#endif

#include "types.h"
#include "drivers.h"
#include "hal.h"
#include "debug.h"
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

uint8_t addCall(task t_ptr);
uint8_t addTask(task t_ptr, uint8_t t_period);
uint8_t removeCall();
uint8_t removeTask(uint8_t position);
void clearTaskQueue();
void clearCallQueue();

uint8_t kernelInit();
uint8_t kernel();
void timerService();

#endif /* KERNEL_H_ */