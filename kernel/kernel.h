/*
 * kernel.h
 *
 * Created: 11.05.2019 21:15:24
 *  Author: ThePetrovich
 */ 


#ifndef KERNEL_H_
#define KERNEL_H_

#define KERNEL_VER "0.0.2-bleeding"
#define KERNEL_TIMESTAMP __TIMESTAMP__

#include "types.h"
#include "../drivers/uart.h"
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

uint8_t addTask(task t_ptr);
uint8_t addTimedTask(task t_ptr, uint8_t t_period);
uint8_t removeTask();
uint8_t removeTimedTask(uint8_t position);
uint8_t kernelInit();
uint8_t kernel();
void timerService();
void clearTaskQueue();
void clearCallQueue();


#endif /* KERNEL_H_ */