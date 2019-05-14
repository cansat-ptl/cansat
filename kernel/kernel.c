/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include "kernel.h"
#include "globals.h"

uint8_t callIndex = 0; //Index of the last task in queue
volatile uint8_t taskIndex = 0; //Index of the last task in queue
volatile task callQueue[MAX_QUEUE_SIZE];
volatile struct taskStruct taskQueue[MAX_QUEUE_SIZE];

void idle(); //System idle task, MUST me declared in tasks.c
void init(); //System init task, MUST me declared in tasks.c

void debugMessage(char* msg){
	sprintf((char*)&tx0_buffer, msg);
	uart0_transmit();
	while(creg0 & (1<<TX0BUSY)){
		;
	}
}

inline uint8_t addTask(task t_ptr){
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


inline uint8_t removeTask(){
	if(SREG & (1 << 7))
		cli();
	if(callIndex != 0){
		callIndex--;
		for(int i = 0; i < MAX_QUEUE_SIZE-1; i++){
			callQueue[i] = callQueue[i+1];
		}
		callQueue[MAX_QUEUE_SIZE-1] = idle;
	}	
	else {
		callQueue[0] = idle;
	}
	sei();
	return 0;
}

inline uint8_t removeTimedTask(uint8_t position){
	if(SREG & (1 << 7))
		cli();
	taskIndex--;
	taskQueue[position].pointer = idle;
	taskQueue[position].period = 0;
	for(int j = position; j < MAX_QUEUE_SIZE-1; j++){
		taskQueue[j].pointer = taskQueue[j+1].pointer;
		taskQueue[j].period = taskQueue[j+1].period;
	}
	taskQueue[MAX_QUEUE_SIZE-1].pointer = idle;
	taskQueue[MAX_QUEUE_SIZE-1].period = 0;	
	sei();
	return 0;
}

void clearCallQueue(){
	if(SREG & (1 << 7))
		cli();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		callQueue[i] = 0;
	}
	callIndex = 0;
	sei();
}

void clearTaskQueue(){
	if(SREG & (1 << 7))
	cli();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		taskQueue[i].pointer = idle;
		taskQueue[i].period = 0;
	}
	taskIndex = 0;
	sei();
}

inline void timerService(){
	if(SREG & (1 << 7))
		cli();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		if(taskQueue[i].pointer == idle) continue;
		else {
			if(taskQueue[i].period != 0)
				taskQueue[i].period--;
			else {
				addTask(taskQueue[i].pointer);
				removeTimedTask(i);
			}
		}
	}
	sei();
}

void startTimer(){
	cli();
	TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10); //prescaler 64
	TCNT1 = 0; 
	OCR1A = 5000;
	TIMSK |= (1 << OCIE1A);
	sei();
}

inline uint8_t taskManager(){
	(callQueue[0])();
	uint8_t code = removeTask();
	return code;
}

uint8_t kernel(){
	addTimedTask(init, 1);
	debugMessage("[INIT]Kernel: starting task manager...DONE!\r\n");
	while(1){
		taskManager();
	}
}

uint8_t kernelInit(){
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		taskQueue[i].pointer = idle;
		taskQueue[i].period = 0;
		callQueue[i] = idle;
	}
	debugMessage("[INIT]Kernel: starting timer...");
	startTimer();
	debugMessage("DONE!\r\n");
	
	kernel();
	return 0;
}

ISR(TIMER1_COMPA_vect){
	timerService();
}