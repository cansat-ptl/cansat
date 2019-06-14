/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include "kernel.h"
#include "globals.h"

#define stackSetup() asm volatile(  \
	"ldi R16, hi8(RAMEND)\n\t"		    \
	"out SPH, R16\n\t"		            \
	"ldi R16, lo8(RAMEND)\n\t"		    \
	"out SPL, R16\n\t"::)

#define saveContext() asm volatile( \
	"PUSH    R0\n\t"				\
	"PUSH    R1\n\t"				\
	"PUSH    R2\n\t"				\
	"PUSH    R3\n\t"				\
	"PUSH    R4\n\t"				\
	"PUSH    R5\n\t"				\
	"PUSH    R6\n\t"				\
	"PUSH    R7\n\t"				\
	"PUSH    R8\n\t"				\
	"PUSH    R9\n\t"				\
	"PUSH    R10\n\t"				\
	"PUSH    R11\n\t"				\
	"PUSH    R12\n\t"				\
	"PUSH    R13\n\t"				\
	"PUSH    R14\n\t"				\
	"PUSH    R15\n\t"				\
	"PUSH    R16\n\t"				\
	"PUSH    R17\n\t"				\
	"PUSH    R18\n\t"				\
	"PUSH    R19\n\t"				\
	"PUSH    R20\n\t"				\
	"PUSH    R21\n\t"				\
	"PUSH    R22\n\t"				\
	"PUSH    R23\n\t"				\
	"PUSH    R24\n\t"				\
	"PUSH    R25\n\t"				\
	"PUSH    R26\n\t"				\
	"PUSH    R27\n\t"				\
	"PUSH    R28\n\t"				\
	"PUSH    R29\n\t"				\
	"PUSH    R30\n\t"				\
	"PUSH    R31\n\t"::)

#define restoreContext() asm volatile(\
	"POP    R31\n\t"				\
	"POP    R30\n\t"				\
	"POP    R29\n\t"				\
	"POP    R28\n\t"				\
	"POP    R27\n\t"				\
	"POP    R26\n\t"				\
	"POP    R25\n\t"				\
	"POP    R24\n\t"				\
	"POP    R23\n\t"				\
	"POP    R22\n\t"				\
	"POP    R21\n\t"				\
	"POP    R20\n\t"				\
	"POP    R19\n\t"				\
	"POP    R18\n\t"				\
	"POP    R17\n\t"				\
	"POP    R16\n\t"				\
	"POP    R15\n\t"				\
	"POP    R14\n\t"				\
	"POP    R13\n\t"				\
	"POP    R12\n\t"				\
	"POP    R11\n\t"				\
	"POP    R10\n\t"				\
	"POP    R9\n\t"					\
	"POP    R8\n\t"					\
	"POP    R7\n\t"					\
	"POP    R6\n\t"					\
	"POP    R5\n\t"					\
	"POP    R4\n\t"					\
	"POP    R3\n\t"					\
	"POP    R2\n\t"					\
	"POP    R1\n\t"					\
	"POP    R0\n\t"::)

uint8_t callIndex = 0; //Index of the last task in queue
volatile uint16_t kflags = 0; 
volatile uint8_t taskIndex = 0; //Index of the last task in queue
volatile task callQueue[MAX_QUEUE_SIZE];
volatile struct taskStruct taskQueue[MAX_QUEUE_SIZE];

void idle(); //System idle task, MUST me declared in tasks.c
void init(); //System init task, MUST me declared in tasks.c

inline uint8_t kernel_addCall(task t_ptr){
	#ifdef DEBUG
		logMessage("Kernel: added call to queue\r\n", 1);
	#endif
	if(statusReg & (1 << 7)){
		disableInterrupts();
	}
	if(taskIndex < MAX_QUEUE_SIZE){
		callIndex++;
		callQueue[callIndex] = t_ptr;
		enableInterrupts();
		return 0;
	}
	else {
		enableInterrupts();
		logMessage("Kernel: call queue overflow\r\n", 3);
		return ERR_QUEUE_OVERFLOW;
	}
}

uint8_t kernel_addTask(task t_ptr, uint8_t t_period){
	#ifdef DEBUG
		logMessage("Kernel: added timed task to queue\r\n", 1);
	#endif
	if(statusReg & (1 << 7)){
		disableInterrupts();
	}
	if(taskIndex < MAX_QUEUE_SIZE){
		taskIndex++;
		taskQueue[taskIndex].pointer = t_ptr;
		taskQueue[taskIndex].period = t_period;
		enableInterrupts();
		return 0;
	}
	else {
		enableInterrupts();
		logMessage("Kernel: task queue overflow\r\n", 3);
		return ERR_QUEUE_OVERFLOW;
	}
	enableInterrupts();
}


inline uint8_t kernel_removeCall(){
	if(statusReg & (1 << 7))
		disableInterrupts();
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
	enableInterrupts();
	return 0;
}

inline uint8_t kernel_removeTask(uint8_t position){
	if(statusReg & (1 << 7))
		disableInterrupts();
	taskIndex--;
	taskQueue[position].pointer = idle;
	taskQueue[position].period = 0;
	for(int j = position; j < MAX_QUEUE_SIZE-1; j++){
		taskQueue[j].pointer = taskQueue[j+1].pointer;
		taskQueue[j].period = taskQueue[j+1].period;
	}
	taskQueue[MAX_QUEUE_SIZE-1].pointer = idle;
	taskQueue[MAX_QUEUE_SIZE-1].period = 0;	
	enableInterrupts();
	return 0;
}

void kernel_clearCallQueue(){
	logMessage("Kernel: call queue cleared\r\n", 2);
	if(statusReg & (1 << 7))
		disableInterrupts();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		callQueue[i] = idle;
	}
	callIndex = 0;
	enableInterrupts();
}

void kernel_clearTaskQueue(){
	logMessage("Kernel: task queue cleared\r\n", 2);
	if(statusReg & (1 << 7))
	disableInterrupts();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		taskQueue[i].pointer = idle;
		taskQueue[i].period = 0;
	}
	taskIndex = 0;
	enableInterrupts();
}

inline void kernel_timerService(){
	if(statusReg & (1 << 7))
		disableInterrupts();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		if(taskQueue[i].pointer == idle) continue;
		else {
			if(taskQueue[i].period != 0)
				taskQueue[i].period--;
			else {
				kernel_addCall(taskQueue[i].pointer);
				kernel_removeTask(i);
			}
		}
	}
	enableInterrupts();
}

void kernel_setupTimer(){
	logMessage("DONE!\r\n", 0);
	disableInterrupts();
	TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10); //prescaler 64
	TCNT1 = 0; 
	OCR1A = 5000;
	enableInterrupts();
}

void kernel_startTimer(){
	disableInterrupts();
	TIMSK |= (1 << OCIE1A);
	enableInterrupts();
}

void kernel_stopTimer(){
	disableInterrupts();
	TIMSK &= ~(1 << OCIE1A);
	enableInterrupts();
}

inline uint8_t kernel_taskManager(){
	(callQueue[0])();
	uint8_t code = kernel_removeCall();
	enableInterrupts();
	return code;
}

uint8_t kernel(){
	logMessage("DONE!\r\n", 0);
	kernel_addTask(init, 1);
	logMessage("Kernel: starting task manager...DONE!\r\n", 1);
	while(1){
		kernel_taskManager();
	}
}

uint8_t kernelInit(){
//	stackSetup(); Fix assembly includes
	kernel_clearCallQueue();
	kernel_clearTaskQueue();
	logMessage("Kernel: starting timer...", 1);
	kernel_setupTimer();
	kernel_startTimer();
	logMessage("Kernel: starting kernel...", 1);
	kernel();
	return 0;
}

ISR(TIMER1_COMPA_vect){
	kernel_timerService();
}