/*
 * debug.h
 *
 * Created: 02.06.2019 21:16:49
 *  Author: ThePetrovich
 */ 


#ifndef DEBUG_H_
#define DEBUG_H_

#define UART_LOGGING 1

#include <stdint.h>
#include <stdio.h>
#include "drivers.h"

void debugMessage(char* msg, uint8_t level);
void debugMessageSD(char* msg, uint8_t level);
void logMessage(char* msg, uint8_t level);

#endif /* DEBUG_H_ */