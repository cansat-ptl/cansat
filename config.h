/*
 * config.h
 *
 * Created: 24.03.2019 16:41:43
 *  Author: ThePetrovich
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

#define F_CPU 8000000L						//CPU frequency

//#define DEBUG 1

//------------------------Project-settings------------------------
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include "kernel/kernel.h"
#include "tasks/tasks.h"
#include "kernel/debug.h"
#include "drivers/driver_config.h"
#include "kernel/drivers.h"
//----------------------------------------------------------------

//-----------------------Kernel-settings--------------------------
#define MAX_QUEUE_SIZE 32
#define ERR_QUEUE_OVERFLOW 1
#define ERR_QUEUE_END 2
//----------------------------------------------------------------

#endif /* CONFIG_H_ */