/*
 * types.h
 *
 * Created: 12.05.2019 18:36:04
 *  Author: ThePetrovich
 */ 

#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

typedef void (*task)(void);
typedef void (*timerISR)(void);
typedef uint8_t byte;

typedef enum {KSTATE_UNINIT, KSTATE_SUSPENDED, KSTATE_BLOCKED, KSTATE_SEMAPHORE, KSTATE_READY, KSTATE_ACTIVE, KSTATE_RUNNING} taskStatus;
typedef enum {KPRIO_NONE, KPRIO_LOW, KPRIO_NORM, KPRIO_HIGH} taskPriority;

typedef struct taskStruct {
	void *stackPtr;
	void *statusPtr;
	task taskPtr;
	taskPriority priority;
	taskStatus status;
	uint8_t pid;
	const char name[8];
};

struct timerStruct {
	timerISR tsrPointer;
	uint32_t period;
	uint32_t savePeriod;
};



#endif /* TYPES_H_ */