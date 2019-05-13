/*
 * globals.h
 *
 * Created: 12.05.2019 18:06:28
 *  Author: ThePetrovich
 */ 


#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "types.h"

#ifndef KERNELconfig
#define MAX_QUEUE_SIZE 16
#define ERR_QUEUE_OVERFLOW 1
#define ERR_QUEUE_END 2
#endif

extern uint8_t callIndex; //Index of the last task in queue
extern uint8_t error;		//Latest task return code
extern volatile uint8_t flags;		//Common variable for kernel control flags
extern volatile unsigned char creg0;
extern volatile unsigned char creg1;

extern volatile char rx0_buffer[32];
extern volatile char tx0_buffer[128];
extern volatile int rx0_pointer;
extern volatile char *tx0_data;
extern volatile int tx0_pointer;
extern volatile int tx0_size;

extern volatile unsigned char creg0;
extern volatile unsigned char creg1;

extern volatile char rx1_buffer[32];
extern volatile char tx1_buffer[128];
extern volatile int rx1_pointer;
extern volatile char *tx1_data;
extern volatile int tx1_pointer;
extern volatile int tx1_size;

struct taskStruct {
	task pointer;    //Pointer to a task
	uint8_t period;	 //delay until the next task in ticks
};
extern volatile struct taskStruct taskQueue[MAX_QUEUE_SIZE];
extern volatile task callQueue[MAX_QUEUE_SIZE];

#endif /* GLOBALS_H_ */
