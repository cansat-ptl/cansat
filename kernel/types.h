/*
 * types.h
 *
 * Created: 12.05.2019 18:36:04
 *  Author: ThePetrovich
 */ 

#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

typedef int (*task)(void);
typedef void (*timerISR)(void);
typedef uint8_t byte;

struct taskStruct {
	task pointer;
	uint16_t period;
	uint8_t priority;
	uint8_t state;
};

struct timerStruct {
	timerISR tsrPointer;
	uint32_t period;
	uint32_t savePeriod;
};

#endif /* TYPES_H_ */