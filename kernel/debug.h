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
#include <avr/pgmspace.h>
#include <stdio.h>
#include "drivers.h"

void debugMessage(char* msg, uint8_t level);
void debugMessage_i(char* msg, uint8_t level);
void debugMessageSD(char* msg, uint8_t level);
void debugMessage_p(const PROGMEM char * msg, uint8_t level);
void debugMessage_pi(const PROGMEM char * msg, uint8_t level);
void debugMessageSD_p(const PROGMEM char * msg, uint8_t level);
void logMessage(char* msg, uint8_t level, uint8_t pgm);

#endif /* DEBUG_H_ */