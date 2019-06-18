/*
 * globals.h
 *
 * Created: 12.05.2019 18:06:28
 *  Author: ThePetrovich
 */ 


#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "types.h"

#ifndef F_CPU
//#warning "F_CPU is not defined
#define F_CPU 8000000L
#endif

extern uint8_t mcucsr_mirror;

#define MAX_QUEUE_SIZE 16

extern uint8_t callIndex;
extern volatile uint16_t kflags; 
extern volatile uint8_t taskIndex;
extern volatile task callQueue[MAX_QUEUE_SIZE];
struct taskStruct {
	task pointer;
	uint8_t period;
};

struct GPS_t
{
	unsigned char valid;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char day;
	unsigned char month;
	unsigned char year;
	unsigned int  millisecond;
	unsigned int cource;
	float latitude;
	unsigned char P;
	float longitude;
	float speed;
	float vspeed;
	unsigned char J;
	unsigned char Sats;
};

extern volatile struct GPS_t GPS;
extern volatile struct taskStruct taskQueue[MAX_QUEUE_SIZE];

extern volatile char rx0_buffer[32];
extern volatile char tx0_buffer[128];
extern volatile int rx0_pointer;
extern volatile char *tx0_data;
extern volatile int tx0_pointer;
extern volatile int tx0_size;

extern volatile unsigned char creg0;
extern volatile unsigned char creg1;

extern volatile char rx1_buffer[128];
extern volatile char tx1_buffer[128];
extern volatile int rx1_pointer;
extern volatile char *tx1_data;
extern volatile int tx1_pointer;
extern volatile int tx1_size;

#endif /* GLOBALS_H_ */
