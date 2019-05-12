/*
 * timerservice.c
 *
 * Created: 11.05.2019 21:14:04
 *  Author: ThePetrovich
 */ 

#include "timerservice.h"
#include "globals.h"

void idle();

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
				removeTimedTask(taskQueue[i].pointer, taskQueue[i].period);
			}
		}
	}
	sei();
}

ISR(TIMER1_COMPA_vect){
	timerService();
}