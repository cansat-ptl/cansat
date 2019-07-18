/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include "../kernel.h"

#define VERBOSE 0

static uint64_t e_time = 0;
static uint8_t callIndex[3] = {0, 0, 0};
static volatile uint16_t kflags = 0; 
static uint16_t kflags_mirror __attribute__ ((section (".noinit")));
static volatile uint8_t taskIndex = 0; //Index of the last task in queue
static volatile task callQueue[MAX_QUEUE_SIZE][3];
static volatile struct taskStruct taskQueue[MAX_QUEUE_SIZE];
extern uint8_t mcucsr_mirror;	 

static char const PROGMEM _MODULE_SIGNATURE[] = "Kernel";

void idle(); //System idle task, MUST me declared in tasks.c
void init(); //System init task, MUST me declared in tasks.c
void initTaskManager(); //Task manager init task, MUST me declared in tasks.c

void kernel_setFlag(uint8_t flag, uint8_t value)
{
	hal_disableInterrupts();
	uint8_t nvalue = !!value;
	kflags ^= (-1 * nvalue ^ kflags) & (1 << flag);
	hal_enableInterrupts();
} 

uint8_t kernel_checkFlag(uint8_t flag)
{
	hal_disableInterrupts();
	return hal_checkBit_m(kflags, flag);
	hal_enableInterrupts();
}

uint64_t kernel_getUptime()
{
	return e_time;
}

inline uint8_t kernel_addCall(task t_ptr, uint8_t t_priority)
{
	if(kernel_checkFlag(KFLAG_DEBUG) && VERBOSE){
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Kernel: added call to queue\r\n"));
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

uint8_t kernel_addTask(task t_ptr, uint16_t t_period, uint8_t t_priority)
{
	if(kernel_checkFlag(KFLAG_DEBUG) && VERBOSE){
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Kernel: added timed task to queue\r\n"));
	}
	if(hal_statusReg & (1 << 7)){
		hal_disableInterrupts();
	}
	for(int i = 0; i <= taskIndex; i++){
		if(taskQueue[i].pointer == t_ptr){
			taskQueue[i].period = t_period;
			taskQueue[i].priority = t_priority;
			hal_enableInterrupts();
			return 0;
		}
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


inline uint8_t kernel_removeCall(uint8_t t_priority)
{
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

inline uint8_t kernel_removeTask(uint8_t position)
{
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

void kernel_clearCallQueue(uint8_t t_priority)
{
	debug_logMessage(PGM_ON, L_WARN, (char *)PSTR("Kernel: call queue cleared\r\n"));
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		callQueue[i][t_priority] = idle;
	}
	callIndex[t_priority] = 0;
	hal_enableInterrupts();
}

void kernel_clearTaskQueue()
{
	debug_logMessage(PGM_ON, L_WARN, (char *)PSTR("Kernel: task queue cleared\r\n"));
	if(hal_statusReg & (1 << 7))
	hal_disableInterrupts();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		taskQueue[i].pointer = idle;
		taskQueue[i].period = 0;
		taskQueue[i].priority = PRIORITY_LOW;
	}
	taskIndex = 0;
	hal_enableInterrupts();
}

inline static void kernel_timerService()
{
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

void kernel_setupTimer()
{
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("DONE!\r\n"));
	hal_disableInterrupts();
	TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10); // prescaler 64
	TCNT1 = 0; 
	OCR1A = 125;
	hal_enableInterrupts();
}

void kernel_startTimer()
{
	hal_disableInterrupts();
	TIMSK |= (1 << OCIE1A);
	hal_enableInterrupts();
}

void kernel_stopTimer()
{
	hal_disableInterrupts();
	TIMSK &= ~(1 << OCIE1A);
	hal_enableInterrupts();
}

inline static uint8_t kernel_taskManager()
{
	if(callQueue[0][PRIORITY_HIGH] != idle || callQueue[1][PRIORITY_HIGH] != idle){
		(callQueue[0][PRIORITY_HIGH])();
		uint8_t code = kernel_removeCall(0);
		hal_enableInterrupts();
		return code;
	}
	else if(callQueue[0][PRIORITY_MID] != idle || callQueue[1][PRIORITY_MID] != idle){
		(callQueue[0][PRIORITY_MID])();
		uint8_t code = kernel_removeCall(1);
		hal_enableInterrupts();
		return code;
	}
	else if(callQueue[0][PRIORITY_LOW] != idle || callQueue[1][PRIORITY_LOW] != idle){
		(callQueue[0][PRIORITY_LOW])();
		uint8_t code = kernel_removeCall(2);
		hal_enableInterrupts();
		return code;
	}
	return 1;
}

uint8_t static kernel()
{
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("DONE!\r\n"));
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Kernel: starting task manager...DONE!\r\n"));
	while(1){
		wdt_reset();
		kernel_taskManager();
		hal_switchBit(&LED_KRN_PORT, LED_KRN);
	}
	return ERR_KRN_RETURN;
}

uint8_t kernelInit()
{
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
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Kernel: starting timer..."));
	kernel_setupTimer();
	kernel_startTimer();
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Kernel: starting kernel..."));
	kernel();
	
	return 0;
}

void kernel_checkMCUCSR()
{
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

void kernel_displayError(uint8_t error)
{
	switch(error){
		case ERR_QUEUE_OVERFLOW:
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("\r\n--------------------------------------------------------------------------------\r\n"));
			debug_logMessage(PGM_ON, L_FATAL, (char *)PSTR("A task/call queue overflow has occurred.\r\n"));
			debug_logMessage(PGM_ON, L_FATAL, (char *)PSTR("This is a critical issue, and immediate action is required.\r\n"));
			debug_logMessage(PGM_ON, L_FATAL, (char *)PSTR("Task manager will be reloaded and reset.\r\n"));
			debug_logMessage(PGM_ON, L_FATAL, (char *)PSTR("Please, report this to the developer as soon as possible.\r\n"));
			debug_logMessage(PGM_ON, L_FATAL, (char *)PSTR("Error details: MAX_QUEUE_SIZE >= callIndex/taskIndex\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("--------------------------------------------------------------------------------\r\n\r\n"));
		break;
		case ERR_WDT_RESET:
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("\r\n--------------------------------------------------------------------------------\r\n"));
			debug_logMessage(PGM_ON, L_FATAL, (char *)PSTR("The system has been reset by watchdog.\r\n"));
			debug_logMessage(PGM_ON, L_FATAL, (char *)PSTR("This is usually caused by software issues or faulty device connections.\r\n"));
			debug_logMessage(PGM_ON, L_FATAL, (char *)PSTR("Please, report this to the developer as soon as possible.\r\n"));
			debug_logMessage(PGM_ON, L_FATAL, (char *)PSTR("Error details: MCUCSR.WDRF = 1\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("--------------------------------------------------------------------------------\r\n\r\n"));
		break;
		case ERR_BOD_RESET:
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("\r\n--------------------------------------------------------------------------------\r\n"));
			debug_logMessage(PGM_ON, L_FATAL, (char *)PSTR("The system has been reset by brown-out detector.\r\n"));
			debug_logMessage(PGM_ON, L_FATAL, (char *)PSTR("This is usually caused by an unstable power supply.\r\n"));
			debug_logMessage(PGM_ON, L_FATAL, (char *)PSTR("Please, check power supply wire connections and circuitry as soon as possible.\r\n"));
			debug_logMessage(PGM_ON, L_FATAL, (char *)PSTR("Error details: MCUCSR.BORF = 1\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("--------------------------------------------------------------------------------\r\n\r\n"));
		break;	
	}
}

ISR(TIMER1_COMPA_vect)
{
	kernel_timerService();
}
