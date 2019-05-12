/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include "kernel.h"
#include "globals.h"

uint8_t callIndex = 0; //Index of the last task in queue
uint8_t taskIndex = 0; //Index of the last task in queue
uint8_t error = 0;		//Latest task return code
volatile uint8_t flags = 0;		//Common variable for kernel control flags
volatile task callQueue[MAX_QUEUE_SIZE];
volatile struct taskStruct taskQueue[MAX_QUEUE_SIZE];

void idle();

uint8_t kernelInit(){
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		taskQueue[i].pointer = idle;
		taskQueue[i].period = 0;
		startTimer();
	}
	return 0;
}

uint8_t addTask(task t_ptr){
	if(SREG & (1 << 7)){
		cli();
	}
	if(taskIndex < MAX_QUEUE_SIZE){
		callIndex++;
		callQueue[callIndex] = t_ptr;
		sei();
		return 0;
	}
	else {
		sei();
		return ERR_QUEUE_OVERFLOW;
	}
	sei();
}

uint8_t addTimedTask(task t_ptr, uint8_t t_period){
	if(SREG & (1 << 7)){
		cli();
	}
	if(taskIndex < MAX_QUEUE_SIZE){
		taskIndex++;
		taskQueue[taskIndex].pointer = t_ptr;
		taskQueue[taskIndex].period = t_period;
		sei();
		return 0;
	}
	else {
		sei();
		return ERR_QUEUE_OVERFLOW;
	}
	sei();
}

uint8_t removeTask(){
	if(SREG & (1 << 7)){
		cli();
	}
	if(callIndex != 0){
		callIndex--;
		for(int i = 0; i < MAX_QUEUE_SIZE-1; i++){
			callQueue[i] = callQueue[i+1];
			callQueue[MAX_QUEUE_SIZE-1] = idle;
			sei();
			return 0;
		}
	}	
	else {
		sei();
		return ERR_QUEUE_END;
	}
	sei();
	return 0;
}

inline uint8_t removeTimedTask(task ptr uint8_t t_period){
	if(SREG & (1 << 7)){
		cli();
	}
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		if(taskQueue[i].pointer == ptr && taskQueue[i].period == t_period && taskQueue[i].repeat == t_repeat){
			taskIndex--;
			for(int j = i; i < MAX_QUEUE_SIZE-1; j++){
				taskQueue[j].pointer = taskQueue[j+1].pointer;
				taskQueue[j].period = taskQueue[j+1].period;
				taskQueue[MAX_QUEUE_SIZE-1].pointer = idle;
				taskQueue[MAX_QUEUE_SIZE-1].period = 0;
			}
		}
		else return -1;		
	}
	sei();
	return 0;
}

inline uint8_t taskManager(){
	cli();
	(callQueue[0])();
	uint8_t code = removeTask();
	sei();
	flags = 0;	//temp
	return code;
}