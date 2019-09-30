/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include "../kernel.h"
#include "../drivers.h"

#define kernel_saveContext()	\
asm volatile ("push r31 \n\t"	\
	"push r30 \n\t"				\
	"push r29 \n\t"				\
	"push r28 \n\t"				\
	"push r27 \n\t"				\
	"push r26 \n\t"				\
	"push r25 \n\t"				\
	"push r24 \n\t"				\
	"push r23 \n\t"				\
	"push r22 \n\t"				\
	"push r21 \n\t"				\
	"push r20 \n\t"				\
	"push r19 \n\t"				\
	"push r18 \n\t"				\
	"push r17 \n\t"				\
	"push r16 \n\t"				\
	"push r15 \n\t"				\
	"push r14 \n\t"				\
	"push r13 \n\t"				\
	"push r12 \n\t"				\
	"push r11 \n\t"				\
	"push r10 \n\t"				\
	"push r9 \n\t"				\
	"push r8 \n\t"				\
	"push r7 \n\t"				\
	"push r6 \n\t"				\
	"push r5 \n\t"				\
	"push r4 \n\t"				\
	"push r3 \n\t"				\
	"push r2 \n\t"				\
	"push r1 \n\t"				\
	"push r0 \n\t"				\
	"in   r0, %[_SREG_] \n\t"   \
	"push r0 \n\t" :: [_SREG_] "i" _SFR_IO_ADDR(SREG))


#define kernel_switchContext()  \
asm volatile ("lds  r26, kos_current_task \n\t" \
	"lds  r27, kos_current_task+1 \n\t" \
	"sbiw r26, 0 \n\t"			\
	"breq 1f \n\t"				\
	"in   r0, %[_SPL_] \n\t"	\
	"st   X+, r0 \n\t"			\
	"in   r0, %[_SPH_] \n\t"	\
	"st   X+, r0 \n\t"			\
	"1:"						\
	"mov  r26, %A[_next_task_] \n\t" \
	"mov  r27, %B[_next_task_] \n\t" \
	"sts  kos_current_task, r26 \n\t" \
	"sts  kos_current_task+1, r27 \n\t" \
	"ld   r0, X+ \n\t"			\
	"out  %[_SPL_], r0 \n\t"	\
	"ld   r0, X+ \n\t"			\
	"out  %[_SPH_], r0 \n\t"	\
	"pop  r31 \n\t"				\
	"bst  r31, %[_I_] \n\t"		\
	"bld  r31, %[_T_] \n\t"		\
	"andi r31, %[_nI_MASK_] \n\t" \
	"out  %[_SREG_], r31 \n\t" :: \
	[_SREG_] "i" _SFR_IO_ADDR(SREG), \
	[_I_] "i" SREG_I,			\
	[_T_] "i" SREG_T,			\
	[_nI_MASK_] "i" (~(1 << SREG_I)), \
	[_SPL_] "i" _SFR_IO_ADDR(SPL), \
	[_SPH_] "i" _SFR_IO_ADDR(SPH), \
	[_next_task_] "r" (task)))
	
#define kernel_restoreContext() \
asm volatile ("pop  r0 \n\t"	\
	"pop  r1 \n\t"				\
	"pop  r2 \n\t"				\
	"pop  r3 \n\t"				\
	"pop  r4 \n\t"				\
	"pop  r5 \n\t"				\
	"pop  r6 \n\t"				\
	"pop  r7 \n\t"				\
	"pop  r8 \n\t"				\
	"pop  r9 \n\t"				\
	"pop  r10 \n\t"				\
	"pop  r11 \n\t"				\
	"pop  r12 \n\t"				\
	"pop  r13 \n\t"				\
	"pop  r14 \n\t"				\
	"pop  r15 \n\t"				\
	"pop  r16 \n\t"				\
	"pop  r17 \n\t"				\
	"pop  r18 \n\t"				\
	"pop  r19 \n\t"				\
	"pop  r20 \n\t"				\
	"pop  r21 \n\t"				\
	"pop  r22 \n\t"				\
	"pop  r23 \n\t"				\
	"pop  r24 \n\t"				\
	"pop  r25 \n\t"				\
	"pop  r26 \n\t"				\
	"pop  r27 \n\t"				\
	"pop  r28 \n\t"				\
	"pop  r29 \n\t"				\
	"pop  r30 \n\t"				\
	"pop  r31 \n\t"				\
	"brtc 2f \n\t"				\
	"reti \n\t"					\
	"2: \n\t"					\
	"ret \n\t"					\
	"" ::)

static task *kernel_currentTask;
static volatile uint16_t kflags = 0;
static uint64_t e_time = 0;
extern uint8_t mcucsr_mirror;

static volatile struct taskStruct taskList[MAX_TASK_COUNT];
static volatile uint8_t taskIndex = 0;

static volatile uint8_t kernelStack[KERNEL_STACK_SIZE];
static volatile uint16_t stackUsage = 0;

void idle() {
	while(1) hal_nop();
}
static void kernel_handleError(uint8_t error);

void kernel_setFlag(uint8_t flag, uint8_t value)
{

} 

uint8_t kernel_checkFlag(uint8_t flag)
{

}

uint64_t kernel_getUptime()
{
	return e_time;
}


uint8_t kernel_setTaskState(task t_pointer, uint8_t state){

}

/*typedef struct taskStruct {
void *stackPtr;
void *statusPtr;
task taskPtr;
taskPriority priority;
taskStatus status;
uint8_t pid;
const char name[8];
};*/

uint8_t kernel_createTask(task t_pointer, uint16_t t_stackSize, taskPriority t_priority, const char t_name[8]){
	if(stackUsage + t_stackSize >= KERNEL_STACK_SIZE)
		return ERR_KRN_STACK_OVERFLOW;
	uint16_t *stackPointer = &kernelStack[KERNEL_STACK_SIZE-1] - stackUsage;
	
	stackPointer[0] = (uint16_t)t_pointer & 0xFF;
	stackPointer[-1] = (uint16_t)t_pointer >> 8;
	for (int i = -2; i > -34; i--){
		stackPointer[i] = 0;
	}
	stackPointer[-34] = 0x80;
	
	taskList[taskIndex].stackPtr = stackPointer - 35;
	taskList[taskIndex].priority = t_priority;
	taskList[taskIndex].taskPtr = t_pointer;
	taskList[taskIndex].status = KSTATE_READY;
	taskList[taskIndex].pid = taskIndex;
	strcpy(taskList[taskIndex].name, t_name);
	
	stackUsage += t_stackSize;
	return 0;
}

inline static void kernel_taskService()
{

}

static uint8_t kernel()
{

}

uint8_t kernel_init()
{
	
}

ISR(TIMER1_COMPA_vect)
{
	kernel_taskService();
	#ifndef USE_EXTERNAL_TIMER
		kernel_timerService();
	#endif
}
