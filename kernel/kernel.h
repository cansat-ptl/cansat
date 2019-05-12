/*
 * kernel.h
 *
 * Created: 11.05.2019 21:15:24
 *  Author: ThePetrovich
 */ 


#ifndef KERNEL_H_
#define KERNEL_H_

#include "types.h"
#include "timerservice.h"
#include <avr/common.h>

uint8_t kernelInit();
uint8_t addTimedTask(task t_ptr, uint8_t t_timeout, uint8_t t_delay);
uint8_t removeTask();
uint8_t taskManager();

#endif /* KERNEL_H_ */