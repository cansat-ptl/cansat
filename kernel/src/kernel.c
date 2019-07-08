/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include "../kernel.h"
#include "../globals.h"


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

#define VERBOSE 0

static uint64_t e_time = 0;
static uint8_t callIndex[3] = {0, 0, 0};
static volatile uint16_t kflags = 0; 
static uint16_t kflags_mirror __attribute__ ((section (".noinit")));
static volatile uint8_t taskIndex = 0; //Index of the last task in queue
static volatile task callQueue[MAX_QUEUE_SIZE][3];
static volatile struct taskStruct taskQueue[MAX_QUEUE_SIZE];

void idle(); //System idle task, MUST me declared in tasks.c
void init(); //System init task, MUST me declared in tasks.c
void initTaskManager(); //Task manager init task, MUST me declared in tasks.c

void kernel_setFlag(uint8_t flag, uint8_t value){
	uint8_t nvalue = !!value;
	kflags ^= (-1 * nvalue ^ kflags) & (1 << flag);
}

uint8_t kernel_checkFlag(uint8_t flag){
	return hal_checkBit_m(kflags, flag);
}

uint64_t kernel_getUptime(){
	return e_time;
}

inline uint8_t kernel_addCall(task t_ptr, uint8_t t_priority){
	if(kernel_checkFlag(KFLAG_DEBUG) && VERBOSE){
		debug_logMessage((char *)PSTR("Kernel: added call to queue\r\n"), 1, 1);
	}
	if(hal_statusReg & (1 << 7)){
		hal_disableInterrupts();
	}
	if(callIndex[t_priority] < MAX_QUEUE_SIZE){
		callIndex[t_priority]++;
		callQueue[callIndex[t_priority]][t_priority] = t_ptr;
		hal_enableInterrupts();
		return 0;
	}
	else {
		hal_enableInterrupts();
		kernel_displayError(ERR_QUEUE_OVERFLOW);
		kernel_clearCallQueue(0);
		kernel_clearCallQueue(1);
		kernel_clearCallQueue(2);
		kernel_clearTaskQueue();
		initTaskManager();
		return ERR_QUEUE_OVERFLOW;
	}
}

uint8_t kernel_addTask(task t_ptr, uint16_t t_period, uint8_t t_priority){
	if(kernel_checkFlag(KFLAG_DEBUG) && VERBOSE){
		debug_logMessage((char *)PSTR("Kernel: added timed task to queue\r\n"), 1, 1);
	}
	if(hal_statusReg & (1 << 7)){
		hal_disableInterrupts();
	}
	if(taskIndex < MAX_QUEUE_SIZE){
		taskIndex++;
		taskQueue[taskIndex].pointer = t_ptr;
		taskQueue[taskIndex].period = t_period;
		taskQueue[taskIndex].priority = t_priority;
		hal_enableInterrupts();
		return 0;
	}
	else {
		hal_enableInterrupts();
		kernel_displayError(ERR_QUEUE_OVERFLOW);
		kernel_clearCallQueue(0);
		kernel_clearCallQueue(1);
		kernel_clearCallQueue(2);
		kernel_clearTaskQueue();
		initTaskManager();
		return ERR_QUEUE_OVERFLOW;
	}
	hal_enableInterrupts();
}


inline uint8_t kernel_removeCall(uint8_t t_priority){
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
	if(callIndex[t_priority] != 0){
		callIndex[t_priority]--;
		for(int i = 0; i < MAX_QUEUE_SIZE-1; i++){
			callQueue[i][t_priority] = callQueue[i+1][t_priority];
		}
		callQueue[MAX_QUEUE_SIZE-1][t_priority] = idle;
	}	
	else {
		callQueue[0][t_priority] = idle;
	}
	hal_enableInterrupts();
	return 0;
}

inline uint8_t kernel_removeTask(uint8_t position){
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
	taskIndex--;
	taskQueue[position].pointer = idle;
	taskQueue[position].period = 0;
	taskQueue[position].priority = 0;
	for(int j = position; j < MAX_QUEUE_SIZE-1; j++){
		taskQueue[j] = taskQueue[j+1];
	}
	taskQueue[MAX_QUEUE_SIZE-1].pointer = idle;
	taskQueue[MAX_QUEUE_SIZE-1].period = 0;	
	taskQueue[MAX_QUEUE_SIZE-1].priority = 0;
	hal_enableInterrupts();
	return 0;
}

void kernel_clearCallQueue(uint8_t t_priority){
	debug_logMessage((char *)PSTR("Kernel: call queue cleared\r\n"), 2, 1);
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		callQueue[i][t_priority] = idle;
	}
	callIndex[t_priority] = 0;
	hal_enableInterrupts();
}

void kernel_clearTaskQueue(){
	debug_logMessage((char *)PSTR("Kernel: task queue cleared\r\n"), 2, 1);
	if(hal_statusReg & (1 << 7))
	hal_disableInterrupts();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		taskQueue[i].pointer = idle;
		taskQueue[i].period = 0;
		taskQueue[i].priority = 0;
	}
	taskIndex = 0;
	hal_enableInterrupts();
}

inline static void kernel_timerService(){
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		if(taskQueue[i].pointer == idle) continue;
		else {
			if(taskQueue[i].period != 0)
				taskQueue[i].period--;
			else {
				kernel_addCall(taskQueue[i].pointer, taskQueue[i].priority);
				kernel_removeTask(i);
			}
		}
	}
	e_time += 1;
	hal_enableInterrupts();
}

void kernel_setupTimer(){
	debug_logMessage((char *)PSTR("DONE!\r\n"), 0, 1);
	hal_disableInterrupts();
	TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10); // prescaler 64
	TCNT1 = 0; 
	OCR1A = 125;
	hal_enableInterrupts();
}

void kernel_startTimer(){
	hal_disableInterrupts();
	TIMSK |= (1 << OCIE1A);
	hal_enableInterrupts();
}

void kernel_stopTimer(){
	hal_disableInterrupts();
	TIMSK &= ~(1 << OCIE1A);
	hal_enableInterrupts();
}

inline static uint8_t kernel_taskManager(){
	if(callIndex[0] > 0){
		(callQueue[0][0])();
		uint8_t code = kernel_removeCall(0);
		hal_enableInterrupts();
		return code;
	}
	else if(callIndex[1] > 0){
		(callQueue[0][1])();
		uint8_t code = kernel_removeCall(1);
		hal_enableInterrupts();
		return code;
	}
	else if(callIndex[2] > 0){
		(callQueue[0][2])();
		uint8_t code = kernel_removeCall(2);
		hal_enableInterrupts();
		return code;
	}
	return 1;
}

uint8_t static kernel(){
	debug_logMessage((char *)PSTR("DONE!\r\n"), 0, 1);
	debug_logMessage((char *)PSTR("Kernel: starting task manager...DONE!\r\n"), 1, 1);
	while(1){
		wdt_reset();
		kernel_taskManager();
		hal_switchBit(&LED_KRN_PORT, LED_KRN);
	}
	return ERR_KRN_RETURN;
}

uint8_t kernelInit(){
//	stackSetup(); Fix assembly includes
	wdt_reset();
	kernel_clearCallQueue(0);
	wdt_reset();
	kernel_clearCallQueue(1);
	wdt_reset();
	kernel_clearCallQueue(2);
	wdt_reset();
	kernel_clearTaskQueue();
	wdt_reset();
	init();
	
	debug_logMessage((char *)PSTR("Kernel: starting timer..."), 1, 1);
	kernel_setupTimer();
	kernel_startTimer();
	debug_logMessage((char *)PSTR("Kernel: starting kernel..."), 1, 1);
	kernel();
	
	return 0;
}

void kernel_checkMCUCSR(){
	if(hal_checkBit_m(mcucsr_mirror, WDRF)){
		kernel_displayError(ERR_WDT_RESET);
		hal_setBit_m(kflags, WDRF);
		return;
	}
	if(hal_checkBit_m(mcucsr_mirror, BORF)){
		kernel_displayError(ERR_BOD_RESET);
		hal_setBit_m(kflags, BORF);
	}
	return;
}

void kernel_displayError(uint8_t error){
	switch(error){
		case ERR_QUEUE_OVERFLOW:
			debug_logMessage((char *)PSTR("\r\n--------------------------------------------------------------------------------\r\n"), 0, 1);
			debug_logMessage((char *)PSTR("A task/call queue overflow has occurred.\r\n"), 4, 1);
			debug_logMessage((char *)PSTR("This is a critical issue, and immediate action is required.\r\n"), 4, 1);
			debug_logMessage((char *)PSTR("Task manager will be reloaded and reset.\r\n"), 4, 1);
			debug_logMessage((char *)PSTR("Please, report this to the developer as soon as possible.\r\n"), 4, 1);
			debug_logMessage((char *)PSTR("Error details: MAX_QUEUE_SIZE >= callIndex/taskIndex\r\n"), 4, 1);
			debug_logMessage((char *)PSTR("--------------------------------------------------------------------------------\r\n\r\n"), 0, 1);
		break;
		case ERR_WDT_RESET:
			debug_logMessage((char *)PSTR("\r\n--------------------------------------------------------------------------------\r\n"), 0, 1);
			debug_logMessage((char *)PSTR("The system has been reset by watchdog.\r\n"), 4, 1);
			debug_logMessage((char *)PSTR("This is usually caused by software issues or faulty device connections.\r\n"), 4, 1);
			debug_logMessage((char *)PSTR("Please, report this to the developer as soon as possible.\r\n"), 4, 1);
			debug_logMessage((char *)PSTR("Error details: MCUCSR.WDRF = 1\r\n"), 4, 1);
			debug_logMessage((char *)PSTR("--------------------------------------------------------------------------------\r\n\r\n"), 0, 1);
		break;
		case ERR_BOD_RESET:
			debug_logMessage((char *)PSTR("\r\n--------------------------------------------------------------------------------\r\n"), 0, 1);
			debug_logMessage((char *)PSTR("The system has been reset by brown-out detector.\r\n"), 4, 1);
			debug_logMessage((char *)PSTR("This is usually caused by an unstable power supply.\r\n"), 4, 1);
			debug_logMessage((char *)PSTR("Please, check power supply wire connections and circuitry as soon as possible.\r\n"), 4, 1);
			debug_logMessage((char *)PSTR("Error details: MCUCSR.BORF = 1\r\n"), 4, 1);
			debug_logMessage((char *)PSTR("--------------------------------------------------------------------------------\r\n\r\n"), 0, 1);
		break;	
	}
}

ISR(TIMER1_COMPA_vect){
	kernel_timerService();
}
