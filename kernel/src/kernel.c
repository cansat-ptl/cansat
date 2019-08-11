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
static volatile task callQueueP0[MAX_HIGHPRIO_CALL_QUEUE_SIZE];
static volatile task callQueueP1[MAX_NORMPRIO_CALL_QUEUE_SIZE];
static volatile task callQueueP2[MAX_LOWPRIO_CALL_QUEUE_SIZE];
static volatile struct taskStruct taskQueue[MAX_TASK_QUEUE_SIZE];

static char const PROGMEM _MODULE_SIGNATURE[] = "Kernel";

int idle(); //System idle task, MUST me declared in tasks.c
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

static inline volatile task* kernel_getCallQueuePointer(uint8_t t_priority){
	switch(t_priority){
		case PRIORITY_HIGH:
			return callQueueP0;
		break;
		case PRIORITY_NORM:
			return callQueueP1;
		break;
		case PRIORITY_LOW:
			return callQueueP2;
		break;
		default:
			return callQueueP2;
		break;
	}
}

static inline uint8_t kernel_getMaxQueueSize(uint8_t t_priority){
	switch(t_priority){
		case PRIORITY_HIGH:
			return MAX_HIGHPRIO_CALL_QUEUE_SIZE;
		break;
		case PRIORITY_NORM:
			return MAX_NORMPRIO_CALL_QUEUE_SIZE;
		break;
		case PRIORITY_LOW:
			return MAX_LOWPRIO_CALL_QUEUE_SIZE;
		break;
		default:
			return 0;
		break;
	}
}

static inline void kernel_resetTaskByPosition(uint8_t position){
	taskQueue[position].pointer = idle;
	taskQueue[position].period = 0;
	taskQueue[position].priority = PRIORITY_LOW;
	taskQueue[position].state = KSTATE_ACTIVE;
}

inline uint8_t kernel_addCall(task t_ptr, uint8_t t_priority)
{	
	if(kernel_checkFlag(KFLAG_DEBUG) && VERBOSE)
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("kernel: Added call to queue\r\n"));
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
		
	uint8_t maxsize = kernel_getMaxQueueSize(t_priority);

	if(callIndex[t_priority] < maxsize){
		callIndex[t_priority]++;
		volatile task* ptr = kernel_getCallQueuePointer(t_priority);
		(ptr)[callIndex[t_priority]] = t_ptr;
		
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
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
		
	for(int i = 0; i <= taskIndex; i++){
		if(taskQueue[i].pointer == t_ptr){
			taskQueue[i].period = t_period;
			taskQueue[i].priority = t_priority;
			taskQueue[i].state = startupState;
			
			hal_enableInterrupts();
			return 0;
		}
	}
	if(taskIndex < MAX_TASK_QUEUE_SIZE){
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
		
	uint8_t maxsize = kernel_getMaxQueueSize(t_priority);
	volatile task* ptr = kernel_getCallQueuePointer(t_priority);
	
	if(callIndex[t_priority] != 0){
		callIndex[t_priority]--;
		for(int i = 0; i < maxsize-1; i++){
			(ptr)[i] = (ptr)[i+1];
		}
		(ptr)[maxsize-1] = idle;
	}	
	else {
		(ptr)[0] = idle;
	}
	
	hal_enableInterrupts();
	return 0;
}

inline uint8_t kernel_removeTask(uint8_t position)
{
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
		
	taskIndex--;
	kernel_resetTaskByPosition(position);
	for(int j = position; j < MAX_TASK_QUEUE_SIZE-1; j++){
		taskQueue[j] = taskQueue[j+1];
	}
	kernel_resetTaskByPosition(MAX_TASK_QUEUE_SIZE-1);
	
	hal_enableInterrupts();
	return 0;
}

uint8_t kernel_removeTaskByPtr(task t_pointer)
{
	uint8_t position;
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
	
	taskIndex--;
	for(position = 0; position < MAX_TASK_QUEUE_SIZE-1; position++){
		if(t_pointer == taskQueue[position].pointer)
			break;
	}
	
	if(position != MAX_TASK_QUEUE_SIZE-1){
		kernel_resetTaskByPosition(position);
		for(int j = position; j < MAX_TASK_QUEUE_SIZE-1; j++){
			taskQueue[j] = taskQueue[j+1];
		}
		kernel_resetTaskByPosition(MAX_TASK_QUEUE_SIZE-1);
		
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
	
	uint8_t maxsize = kernel_getMaxQueueSize(t_priority);	
	volatile task* ptr = kernel_getCallQueuePointer(t_priority);
	
	for(int i = 0; i < maxsize; i++){
		(ptr)[i] = idle;
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
		
	for(int i = 0; i < MAX_TASK_QUEUE_SIZE; i++){
		kernel_resetTaskByPosition(i);
	}
	taskIndex = 0;
	
	hal_enableInterrupts();
}

uint8_t kernel_setTaskState(task t_pointer, uint8_t state){
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
		
	for(int i = 0; i < MAX_TASK_QUEUE_SIZE-1; i++){
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
	for(int i = 0; i < MAX_TASK_QUEUE_SIZE; i++){
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
	TCCR1B |= (1 << WGM12)|(KERNEL_TIMER_PRESCALER << CS10); // prescaler 64 cs11 & cs10 = 1
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
	
	if((callQueueP0[0] != idle || callQueueP0[1] != idle) && highInARow <= FORCE_LOWERPRIO_THRESHOLD){
		highInARow++;
		
		#if PROFILING == 1
			uint64_t startTime = kernel_getUptime();
		#endif
		
		int taskReturnCode = (callQueueP0[0])();
		if(taskReturnCode != 0) kernel_setTaskState(callQueueP0[0], KSTATE_SUSPENDED);
		kernel_removeCall(0);
		
		#if PROFILING == 1
			debug_logMessage(PGM_OFF, L_INFO, "kernel: Task exec time: %u ms\r\n", (unsigned int)(kernel_getUptime()-startTime));
		#endif
		
		return taskReturnCode;
	}
	else if((callQueueP1[0] != idle || callQueueP1[1] != idle)  && medInARow <= FORCE_LOWERPRIO_THRESHOLD){
		highInARow = 0;
		medInARow++;
		
		#if PROFILING == 1
			uint64_t startTime = kernel_getUptime();
		#endif
		
		int taskReturnCode = (callQueueP1[0])();
		if(taskReturnCode != 0) kernel_setTaskState(callQueueP1[0], KSTATE_SUSPENDED);
		kernel_removeCall(1);
		
		#if PROFILING == 1
			debug_logMessage(PGM_OFF, L_INFO, "kernel: Task exec time: %u ms\r\n", (unsigned int)(kernel_getUptime()-startTime));
		#endif
		
		return taskReturnCode;
	}
	else if(callQueueP2[0] != idle || callQueueP2[1] != idle){
		highInARow = 0;
		medInARow = 0;
		
		#if PROFILING == 1
			uint64_t startTime = kernel_getUptime();
		#endif
		
		int taskReturnCode = (callQueueP2[0])();
		if(taskReturnCode != 0) kernel_setTaskState(callQueueP2[0], KSTATE_SUSPENDED);
		kernel_removeCall(2);

		#if PROFILING == 1
			debug_logMessage(PGM_OFF, L_INFO, "kernel: Task exec time: %u ms\r\n", (unsigned int)(kernel_getUptime()-startTime));
		#endif
		
		return taskReturnCode;
	}
	
	return 0;
}

static uint8_t kernel()
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
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("\r\n[INIT]kernel: Starting up CanSat kernel v%s\r\n\r\n"), KERNEL_VER);
	
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: Setting up PRIORITY_HIGH queue"));
	kernel_clearCallQueue(0);
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("         [DONE]\r\n"));
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: Setting up PRIORITY_NORM queue"));
	kernel_clearCallQueue(1);
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("         [DONE]\r\n"));
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: Setting up PRIORITY_LOW queue"));
	kernel_clearCallQueue(2);
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("          [DONE]\r\n"));
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: Setting up task queue"));
	kernel_clearTaskQueue();
	
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("                  [DONE]\r\n"));
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("\r\n----------------------Memory usage----------------------\r\n"));
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: HIGHPRIO queue size:             %d\r\n"), MAX_HIGHPRIO_CALL_QUEUE_SIZE);
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: NORMPRIO queue size:             %d\r\n"), MAX_NORMPRIO_CALL_QUEUE_SIZE);
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: LOWPRIO queue size:              %d\r\n"), MAX_LOWPRIO_CALL_QUEUE_SIZE);
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: Task queue size:                 %d\r\n"), MAX_TASK_QUEUE_SIZE);
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: HIGHPRIO queue memory usage:     %d bytes\r\n"), MAX_HIGHPRIO_CALL_QUEUE_SIZE * sizeof(void*));
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: NORMPRIO queue memory usage:     %d bytes\r\n"), MAX_NORMPRIO_CALL_QUEUE_SIZE * sizeof(void*));
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: LOWPRIO queue memory usage:      %d bytes\r\n"), MAX_LOWPRIO_CALL_QUEUE_SIZE * sizeof(void*));
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: Task queue memory usage:         %d bytes\r\n"), MAX_TASK_QUEUE_SIZE * sizeof(taskQueue[0]));
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[INIT]kernel: Total Task Manager memory usage: %d bytes\r\n"), MAX_TASK_QUEUE_SIZE * sizeof(taskQueue[0]) +\
																												  MAX_HIGHPRIO_CALL_QUEUE_SIZE * sizeof(void*) +\
																												  MAX_NORMPRIO_CALL_QUEUE_SIZE * sizeof(void*) +\
																												  MAX_LOWPRIO_CALL_QUEUE_SIZE * sizeof(void*));
	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("--------------------------------------------------------\r\n\r\n"));
	
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
