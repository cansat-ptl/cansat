/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include "tasks.h"
#include "timersvc.h"

uint8_t queueIndex = 0;
uint8_t error = 0;

task taskQueue[MAX_QUEUE_SIZE];
struct taskStruct {
	task pointer;
	uint8_t timeout; //unused
	uint8_t delay; //delay until next task in ticks
} callQueue[MAX_QUEUE_SIZE];

uint8_t kernelInit(){
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		taskQueue[i] = idle;
		callQueue[i].pointer = idle;
		callQueue[i].timeout = 0;
		callQueue[i].delay = 0;
		startTimer();
	}
}

inline uint8_t addTask(task t_ptr, uint8_t t_timeout, uint8_t t_delay){
	if(SREG & (1 << 7)){
		cli();
	}
	if(queueIndex < MAX_QUEUE_SIZE){
		queueIndex++;
		taskQueue[queueIndex] = t_ptr;
		callQueue[queueIndex].pointer = t_ptr;
		callQueue[queueIndex].timeout = t_timeout;
		callQueue[queueIndex].delay = t_delay;
		sei();
		return 0;
	}
	else {
		sei();
		return ERR_QUEUE_OVERFLOW;
	}
}

inline uint8_t removeTask(){
	if(SREG & (1 << 7)){
		cli();
	}
	if(queueIndex != 0){
		queueIndex--;
		for(int i = 0; i < MAX_QUEUE_SIZE-1; i++){
			taskQueue[i] = taskQueue[i+1];
			callQueue[queueIndex].pointer = callQueue[queueIndex+1].pointer;
			callQueue[queueIndex].timeout = callQueue[queueIndex+1].timeout;
			callQueue[queueIndex].delay = callQueue[queueIndex+1].delay;
			taskQueue[MAX_QUEUE_SIZE-1] = idle;
			callQueue[MAX_QUEUE_SIZE-1].pointer = idle;
			callQueue[MAX_QUEUE_SIZE-1].timeout = 0;
			callQueue[MAX_QUEUE_SIZE-1].delay = 0;
			sei();
			return 0;
		}
	}	
	else {
		sei();
		return ERR_QUEUE_END;
	}
}

inline uint8_t taskManager(){
	if(callFlag){
		cli();
		error = (taskQueue[0])();
		uint8_t code = removeTask();
		sei();
		return code;	
	}
	return 0;
}