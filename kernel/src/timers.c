/*
 * timers.c
 *
 * Created: 20.07.2019 10:41:13
 *  Author: WorldSkills-2019
 */ 
#include "../kernel.h"

static volatile struct timerStruct timerQueue[MAX_TIMER_COUNT];
static volatile uint8_t timerIndex = 0;

uint8_t kernel_setTimer(timerISR t_pointer, uint32_t t_period)
{
	if(!kernel_checkFlag(KFLAG_TIMER_EN)) return 2;
		
	if(hal_statusReg & (1 << 7)){
		hal_disableInterrupts();
	}
	for(int i = 0; i <= timerIndex; i++){
		if(timerQueue[i].tsrPointer == t_pointer){
			timerQueue[i].period = t_period;
			timerQueue[i].savePeriod = t_period;
			hal_enableInterrupts();
			return 0;
		}
	}
	if(timerIndex < MAX_TIMER_COUNT){
		timerIndex++;
		timerQueue[timerIndex].tsrPointer = t_pointer;
		timerQueue[timerIndex].period = t_period;
		timerQueue[timerIndex].savePeriod = t_period;
		hal_enableInterrupts();
		return 0;
	}
	else {
		hal_enableInterrupts();
		return MAX_TIMER_COUNT;
	}
}

uint8_t kernel_removeTimer(timerISR t_pointer)
{
	if(!kernel_checkFlag(KFLAG_TIMER_EN)) return 2;
	
	uint8_t position;
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
	
	timerIndex--;
	for(position = 0; position < MAX_TIMER_COUNT-1; position++){
		if(t_pointer == timerQueue[position].tsrPointer)
		break;
	}
	
	if(position != MAX_TIMER_COUNT-1){
		timerQueue[position].tsrPointer = NULL;
		timerQueue[position].period = 0;
		for(int j = position; j < MAX_TIMER_COUNT-1; j++){
			timerQueue[j] = timerQueue[j+1];
		}
		timerQueue[MAX_TIMER_COUNT-1].tsrPointer = NULL;
		timerQueue[MAX_TIMER_COUNT-1].period = 0;
		hal_enableInterrupts();
		return 0;
	}
	hal_enableInterrupts();
	return 0;
}

void kernel_timerService()
{
	kernel_setFlag(KFLAG_TIMER_ISR, 1);
	if(hal_statusReg & (1 << 7))
		hal_disableInterrupts();
	for(int i = 0; i < MAX_TIMER_COUNT; i++){
		if(timerQueue[i].tsrPointer == NULL) continue;
		else {
			if(timerQueue[i].period != 0)
				timerQueue[i].period--;
			else {
				if(timerQueue[i].tsrPointer != NULL) (timerQueue[i].tsrPointer)(); //Additional NULLPTR protection
				timerQueue[i].period = timerQueue[i].savePeriod;
			}
		}
	}
	kernel_setFlag(KFLAG_TIMER_ISR, 0);
	hal_enableInterrupts();
}
