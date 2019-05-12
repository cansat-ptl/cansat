/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include "kernel.h"
#include "globals.h"

uint8_t queueIndex = 0; //Index of the last task in queue
uint8_t error = 0;		//Latest task return code
volatile uint8_t flags = 0;		//Common variable for kernel control flags
volatile struct taskStruct taskQueue[MAX_QUEUE_SIZE];

uint8_t kernelInit(){
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		taskQueue[i].pointer = idle;
		taskQueue[i].timeout = 0;
		taskQueue[i].delay = 0;
		startTimer();
	}
	return 0;
}

uint8_t addTask(task t_ptr, uint8_t t_timeout, uint8_t t_delay){
	if(SREG & (1 << 7)){
		cli();
	}
	if(queueIndex < MAX_QUEUE_SIZE){
		queueIndex++;
		taskQueue[queueIndex].pointer = t_ptr;
		taskQueue[queueIndex].timeout = t_timeout;
		taskQueue[queueIndex].delay = t_delay;
		sei();
		return 0;
	}
	else {
		sei();
		return ERR_QUEUE_OVERFLOW;
	}
}

uint8_t removeTask(){
	if(SREG & (1 << 7)){
		cli();
	}
	if(queueIndex != 0){
		queueIndex--;
		for(int i = 0; i < MAX_QUEUE_SIZE-1; i++){
			taskQueue[i].pointer = taskQueue[i+1].pointer;
			taskQueue[i].timeout = taskQueue[i+1].timeout;
			taskQueue[i].delay = taskQueue[i+1].delay;
			taskQueue[MAX_QUEUE_SIZE-1].pointer = idle;
			taskQueue[MAX_QUEUE_SIZE-1].timeout = 0;
			taskQueue[MAX_QUEUE_SIZE-1].delay = 0;
			sei();
			return 0;
		}
	}	
	else {
		sei();
		return ERR_QUEUE_END;
	}
	return 0;
}

uint8_t taskManager(){
	if(flags & 1){
		cli();
		(taskQueue[0].pointer)();
		uint8_t code = removeTask();
		sei();
		return code;	
	}
	return 0;
}