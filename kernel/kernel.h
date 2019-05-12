/*
 * kernel.h
 *
 * Created: 11.05.2019 21:15:24
 *  Author: ThePetrovich
 */ 


#ifndef KERNEL_H_
#define KERNEL_H_

#include "types.h"
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <stdint.h>

uint8_t kernelInit();
uint8_t addTask(task t_ptr);
uint8_t addTimedTask(task t_ptr, uint8_t t_period);
uint8_t removeTask();
uint8_t taskManager();
uint8_t removeTask();
uint8_t removeTimedTask(task ptr, uint8_t t_period);
uint8_t kernel();

#endif /* KERNEL_H_ */