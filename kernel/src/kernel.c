/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include "../kernel.h"
#include "../drivers.h"

static volatile uint16_t kflags = 0;
static uint16_t kflags_mirror __attribute__ ((section (".noinit")));
static uint64_t e_time = 0;
extern uint8_t mcucsr_mirror;

static uint8_t callIndex[3] = {0, 0, 0};
static volatile uint8_t taskIndex = 0; //Index of the last task in queue
static volatile task callQueue[MAX_QUEUE_SIZE][3];
static volatile struct taskStruct taskQueue[MAX_QUEUE_SIZE];

static char const PROGMEM _MODULE_SIGNATURE[] = "Kernel";

int  idle(); //System idle task, MUST me declared in tasks.c
int systemInit();
void init(); //System init task, MUST me declared in tasks.c
void initTaskManager(); //Task manager init task, MUST me declared in tasks.c
static void kernel_handleError(uint8_t error);

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
	uint8_t flag_tmp = hal_checkBit_m(kflags, flag);
	hal_enableInterrupts();
	return flag_tmp;
}

uint64_t kernel_getUptime()
{
	return e_time;
}

inline uint8_t kernel_addCall(task t_ptr, uint8_t t_priority)
{
	if(kernel_checkFlag(KFLAG_DEBUG) && VERBOSE)
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("kernel: Added call to queue\r\n"));
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
		kernel_handleError(ERR_QUEUE_OVERFLOW);
		kernel_clearCallQueue(0);
		kernel_clearCallQueue(1);
		kernel_clearCallQueue(2);
		kernel_clearTaskQueue();
		initTaskManager();
		hal_enableInterrupts();
		return ERR_QUEUE_OVERFLOW;
	}
}

uint8_t kernel_addTask(task t_ptr, uint16_t t_period, uint8_t t_priority, uint8_t startupState)
{
	if(kernel_checkFlag(KFLAG_DEBUG) && VERBOSE)
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("kernel: Added timed task to queue\r\n"));
	if(hal_statusReg & (1 << 7)){
		hal_disableInterrupts();
	}
	for(int i = 0; i <= taskIndex; i++){
		if(taskQueue[i].pointer == t_ptr){
			taskQueue[i].period = t_period;
			taskQueue[i].priority = t_priority;
			taskQueue[i].state = startupState;
			hal_enableInterrupts();
			return 0;
		}
	}
	if(taskIndex < MAX_QUEUE_SIZE){
		taskIndex++;
		taskQueue[taskIndex].pointer = t_ptr;
		taskQueue[taskIndex].period = t_period;
		taskQueue[taskIndex].priority = t_priority;
		taskQueue[taskIndex].state = startupState;
		hal_enableInterrupts();
		return 0;
	}
	else {
		kernel_handleError(ERR_QUEUE_OVERFLOW);
		kernel_clearCallQueue(0);
		kernel_clearCallQueue(1);
		kernel_clearCallQueue(2);
		kernel_clearTaskQueue();
		initTaskManager();
		hal_enableInterrupts();
		return ERR_QUEUE_OVERFLOW;
	}
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
	taskQueue[position].priority = PRIORITY_LOW;
	taskQueue[position].state = KSTATE_ACTIVE;
	for(int j = position; j < MAX_QUEUE_SIZE-1; j++){
		taskQueue[j] = taskQueue[j+1];
	}
	taskQueue[MAX_QUEUE_SIZE-1].pointer = idle;
	taskQueue[MAX_QUEUE_SIZE-1].period = 0;	
	taskQueue[MAX_QUEUE_SIZE-1].priority = PRIORITY_LOW;
	taskQueue[MAX_QUEUE_SIZE-1].state = KSTATE_ACTIVE;
	hal_enableInterrupts();
	return 0;
}

uint8_t kernel_removeTaskByPtr(task t_pointer)
{
	uint8_t position;
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
	
	taskIndex--;
	for(position = 0; position < MAX_QUEUE_SIZE-1; position++){
		if(t_pointer == taskQueue[position].pointer)
			break;
	}
	
	if(position != MAX_QUEUE_SIZE-1){
		taskQueue[position].pointer = idle;
		taskQueue[position].period = 0;
		taskQueue[position].priority = PRIORITY_LOW;
		taskQueue[position].state = KSTATE_ACTIVE;
		for(int j = position; j < MAX_QUEUE_SIZE-1; j++){
			taskQueue[j] = taskQueue[j+1];
		}
		taskQueue[MAX_QUEUE_SIZE-1].pointer = idle;
		taskQueue[MAX_QUEUE_SIZE-1].period = 0;
		taskQueue[MAX_QUEUE_SIZE-1].priority = PRIORITY_LOW;
		taskQueue[position].state = KSTATE_ACTIVE;
		hal_enableInterrupts();
		return 0;
	}
	else {
		 hal_enableInterrupts();
		 return 1;
	}
}

void kernel_clearCallQueue(uint8_t t_priority)
{
	if(kernel_checkFlag(KFLAG_DEBUG) && VERBOSE)
		debug_logMessage(PGM_ON, L_WARN, (char *)PSTR("kernel: Call queue cleared\r\n"));
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
	if(kernel_checkFlag(KFLAG_DEBUG) && VERBOSE)
		debug_logMessage(PGM_ON, L_WARN, (char *)PSTR("kernel: Task queue cleared\r\n"));
	if(hal_statusReg & (1 << 7))
	hal_disableInterrupts();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		taskQueue[i].pointer = idle;
		taskQueue[i].period = 0;
		taskQueue[i].priority = PRIORITY_LOW;
		taskQueue[i].state = KSTATE_ACTIVE;
	}
	taskIndex = 0;
	hal_enableInterrupts();
}

uint8_t kernel_setTaskState(task t_pointer, uint8_t state){
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
	for(int i = 0; i < MAX_QUEUE_SIZE-1; i++){
		if(taskQueue[i].pointer == t_pointer){
			taskQueue[i].state = state;
			hal_enableInterrupts();
			return 0;
		}
	}
	hal_enableInterrupts();
	return 1;
}

inline static void kernel_taskService()
{
	kernel_setFlag(KFLAG_TIMER_ISR, 1);
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		if(taskQueue[i].pointer == idle) continue;
		else {
			if(taskQueue[i].period != 0)
				taskQueue[i].period--;
			else {
				if(taskQueue[i].state == KSTATE_ACTIVE){
					kernel_addCall(taskQueue[i].pointer, taskQueue[i].priority);
					kernel_removeTask(i);
				}
			}
		}
	}
	e_time += 1;
	hal_enableInterrupts();
	kernel_setFlag(KFLAG_TIMER_ISR, 0);
}

void kernel_setupTimer()
{
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("                         [DONE]\r\n"));
	hal_disableInterrupts();
	TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10); // prescaler 64
	TCNT1 = 0; 
	OCR1A = 125;
	hal_enableInterrupts();
	kernel_setFlag(KFLAG_TIMER_SET, 1);
}

void kernel_startTimer()
{
	if(kernel_checkFlag(KFLAG_TIMER_SET)){
		hal_disableInterrupts();
		TIMSK |= (1 << OCIE1A);
		hal_enableInterrupts();
		kernel_setFlag(KFLAG_TIMER_EN, 1);
	}
}

void kernel_stopTimer()
{
	if(kernel_checkFlag(KFLAG_TIMER_SET)){
		hal_disableInterrupts();
		TIMSK &= ~(1 << OCIE1A);
		hal_enableInterrupts();
		kernel_setFlag(KFLAG_TIMER_EN, 0);
	}
}

inline static uint8_t kernel_taskManager()
{
	static uint8_t highInARow = 0, medInARow = 0;
	if((callQueue[0][PRIORITY_HIGH] != idle || callQueue[1][PRIORITY_HIGH] != idle) && highInARow <= FORCE_LOWERPRIO_THRESHOLD){
		highInARow++;
		
		#ifdef PROFILING
			uint64_t startTime = kernel_getUptime();
		#endif
		
		int taskReturnCode = (callQueue[0][PRIORITY_HIGH])();
		if(taskReturnCode != 0) kernel_setTaskState(callQueue[0][PRIORITY_HIGH], KSTATE_SUSPENDED);
		kernel_removeCall(0);
		
		#ifdef PROFILING
			debug_logMessage(PGM_OFF, L_INFO, "kernel: Task exec time: %u ms\r\n", (unsigned int)(kernel_getUptime()-startTime));
		#endif
		
		return taskReturnCode;
	}
	else if((callQueue[0][PRIORITY_MID] != idle || callQueue[1][PRIORITY_MID] != idle)  && medInARow <= FORCE_LOWERPRIO_THRESHOLD){
		highInARow = 0;
		medInARow++;
		
		#ifdef PROFILING
			uint64_t startTime = kernel_getUptime();
		#endif
		
		int taskReturnCode = (callQueue[0][PRIORITY_MID])();
		if(taskReturnCode != 0) kernel_setTaskState(callQueue[0][PRIORITY_MID], KSTATE_SUSPENDED);
		kernel_removeCall(1);
		
		#ifdef PROFILING
			debug_logMessage(PGM_OFF, L_INFO, "kernel: Task exec time: %u ms\r\n", (unsigned int)(kernel_getUptime()-startTime));
		#endif
		
		return taskReturnCode;
	}
	else if(callQueue[0][PRIORITY_LOW] != idle || callQueue[1][PRIORITY_LOW] != idle){
		highInARow = 0;
		medInARow = 0;
		
		#ifdef PROFILING
			uint64_t startTime = kernel_getUptime();
		#endif
		
		int taskReturnCode = (callQueue[0][PRIORITY_LOW])();
		if(taskReturnCode != 0) kernel_setTaskState(callQueue[0][PRIORITY_LOW], KSTATE_SUSPENDED);
		kernel_removeCall(2);

		#ifdef PROFILING
			debug_logMessage(PGM_OFF, L_INFO, "kernel: Task exec time: %u ms\r\n", (unsigned int)(kernel_getUptime()-startTime));
		#endif
		
		return taskReturnCode;
	}
	
	return 0;
}

uint8_t static kernel()
{
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("                        [DONE]\r\n"));
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: Starting task manager"));
	initTaskManager();
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("                  [DONE]\r\n"));
	while(1){
		wdt_reset();
		uint8_t taskReturnCode = kernel_taskManager();
		if(taskReturnCode != 0) kernel_handleError(taskReturnCode);
		hal_switchBit(&LED_KRN_PORT, LED_KRN);
	}
	return ERR_KRN_RETURN;
}

uint8_t kernelInit()
{
	init();
	kernel_setFlag(KFLAG_INIT, 1);
	kernel_setFlag(KFLAG_TIMER_SET, 0);
	kernel_setFlag(KFLAG_TIMER_EN, 0);
	kernel_clearCallQueue(0);
	kernel_clearCallQueue(1);
	kernel_clearCallQueue(2);
	kernel_clearTaskQueue();
	
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: Starting timer"));
	kernel_setupTimer();
	kernel_startTimer();
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: Starting kernel"));
	kernel();
	
	return 0;
}

void kernel_checkMCUCSR()
{
	if(hal_checkBit_m(mcucsr_mirror, WDRF)){
		kernel_handleError(ERR_WDT_RESET);
		hal_setBit_m(kflags, WDRF);
		return;
	}
	if(hal_checkBit_m(mcucsr_mirror, BORF)){
		kernel_handleError(ERR_BOD_RESET);
		hal_setBit_m(kflags, BORF);
	}
	return;
}

static void kernel_handleError(uint8_t error)
{
	switch(error){
		case ERR_QUEUE_OVERFLOW:
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("\r\n--------------------------------------------------------------------------------\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] A task/call queue overflow has occurred.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] This is a critical issue, and immediate action is required.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Task manager will be reloaded and reset.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Please, report this to the developer as soon as possible.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Error details: MAX_QUEUE_SIZE >= callIndex/taskIndex\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("--------------------------------------------------------------------------------\r\n\r\n"));
		break;
		case ERR_WDT_RESET:
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("\r\n--------------------------------------------------------------------------------\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] The system has been reset by watchdog.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] This is usually caused by software issues or faulty device connections.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Please, report this to the developer as soon as possible.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Error details: MCUCSR.WDRF = 1\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("--------------------------------------------------------------------------------\r\n\r\n"));
		break;
		case ERR_BOD_RESET:
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("\r\n--------------------------------------------------------------------------------\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] The system has been reset by brown-out detector.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] This is usually caused by an unstable power supply.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Please, check power supply wire connections and circuitry as soon as possible.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Error details: MCUCSR.BORF = 1\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("--------------------------------------------------------------------------------\r\n\r\n"));
		break;	
		case ERR_DEVICE_FAIL:
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("\r\n--------------------------------------------------------------------------------\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] A major device failure has been reported by one of the tasks.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] To prevent damage and data corruption, the task has been suspended.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Please, check your device connections and circuitry as soon as possible.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Error details: taskReturnCode = ERR_DEVICE_FAIL\r\n")); //TODO: implement verbose error info, e.g what device has failed
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("--------------------------------------------------------------------------------\r\n\r\n"));
		break;
	}
}

ISR(TIMER1_COMPA_vect)
{
	kernel_taskService();
	#ifndef USE_EXTERNAL_TIMER
		kernel_timerService();
	#endif
}
