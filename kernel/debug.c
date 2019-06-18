/*
 * debug.c
 *
 * Created: 02.06.2019 20:53:16
 *  Author: ThePetrovich
 */ 
#include "kernel.h"
#include "globals.h"

char levels[5][16] = {
	"",
	"[INFO]",
	"[WARN]",
	"[ERROR]",
	"[FATAL]"
};

inline void debugMessage(char* msg, uint8_t level) {
	uart0_puts(levels[level]);
	uart0_puts(msg);
}

inline void debugMessage_i(char* msg, uint8_t level) {
	sprintf((char*)&tx0_buffer, "%s%s", levels[level], msg);
	uart0_transmit();
	while(creg0 & (1<<TX0BUSY));
}

void debugMessageSD(char* msg, uint8_t level) {
	//sprintf((char*)&tx0_buffer, "%s%s", levels[level], msg);
	//uart0_transmit();
	//while(creg0 & (1<<TX0BUSY)){
	//	;
	//}
}

inline void logMessage(char* msg, uint8_t level){
	#ifdef UART_LOGGING
		debugMessage(msg, level);
	#endif
	#ifdef UART_LOGGING_I
		debugMessage_i(msg, level);
	#endif
	#ifdef SD_LOGGING
		debugMessageSD(msg, level);
	#endif
}
