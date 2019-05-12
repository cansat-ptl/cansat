/*
 * globals.h
 *
 * Created: 12.05.2019 18:06:28
 *  Author: Admin
 */ 


#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "types.h"

#ifndef KERNELconfig
#define MAX_QUEUE_SIZE 16
#define ERR_QUEUE_OVERFLOW 1
#define ERR_QUEUE_END 2
#endif

extern uint8_t queueIndex; //Index of the last task in queue
extern uint8_t error;		//Latest task return code
extern volatile uint8_t flags;		//Common variable for kernel control flags

struct taskStruct {
	task pointer;    //Pointer to a task
	uint8_t timeout; //unused
	uint8_t delay;	 //delay until the next task in ticks
};
extern volatile struct taskStruct taskQueue[MAX_QUEUE_SIZE];

#endif /* GLOBALS_H_ */

/*
uint8_t queueIndex = 0; //Index of the last task in queue
uint8_t error = 0;		//Latest task return code
volatile uint8_t flags = 0;		//Common variable for kernel control flags

typedef void (*task)(void);

struct taskStruct {
	task pointer;    //Pointer to a task
	uint8_t timeout; //unused
	uint8_t delay;	 //delay until the next task in ticks
};
volatile struct taskStruct taskQueue[MAX_QUEUE_SIZE];
*/