/*
 * timerservice.c
 *
 * Created: 11.05.2019 21:14:04
 *  Author: ThePetrovich
 */ 

#include "timerservice.h"
#include "globals.h"

void startTimer(){
	/*cli();
	TCCR1A |= 0;
	TCNT1 = 0;						 //Resetting timer
	TCCR1B |= (1<<CS12)|(1<<WGM12);  //Timer1 CTC mode, prescaler 256
	OCR1A = 0x271;					 //625 as compare value
	TIMSK |= (1<<OCIE1A);			 //Fire interrupt when compare match, approx. every 20 ms
	sei();*/
	cli();
	TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10);
	TCNT1 = 0; // initialize counter
	OCR1A = 5000;  // initialize compare value
	TIMSK |= (1 << OCIE1A); // enable compare interrupt
	sei(); // enable global interrupts
}

inline static void timerService(){
	if(SREG & (1 << 7)){
		cli();
	}
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		if(taskQueue[i].pointer == idle) continue;
		else {
			if(taskQueue[i].period != 0)
				taskQueue[i].period--;
			else {
				callQueue[callIndex] = taskQueue[i].pointer;
				removeTimedTask(taskQueue[i].pointer, taskQueue[i].period);
			}
		}
	}
	sei();
}

ISR(TIMER1_COMPA_vect){
	timerService();
}