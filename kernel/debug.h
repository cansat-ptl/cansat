/*
 * debug.h
 *
 * Created: 02.06.2019 21:16:49
 *  Author: ThePetrovich
 */ 

#ifndef DEBUG_H_
#define DEBUG_H_

#define DBG_MOD_VER "0.9.1-staging"
#define DBG_MOD_TIMESTAMP __TIMESTAMP__

#define UART_LOGGING 1

#include <stdint.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "kernel.h"

void debug_sendMessage(char* msg, uint8_t level);
void debug_sendMessage_i(char* msg, uint8_t level);
void debug_sendMessageSD(char* msg, uint8_t level);
void debug_sendMessage_p(const char * msg, uint8_t level);
void debug_sendMessage_pi(const char * msg, uint8_t level);
void debug_sendMessageSD_p(const char * msg, uint8_t level);
void debug_logMessage(char* msg, uint8_t level, uint8_t pgm);

#endif /* DEBUG_H_ */