/*
 * timerservice.c
 *
 * Created: 11.05.2019 21:14:04
 *  Author: ThePetrovich
 */ 

#include "timerservice.h"
#include "globals.h"

void startTimer(){
	cli();
	TCCR1A |= 0;
	TCNT1 = 0;						 //Resetting timer
	TCCR1B |= (1<<CS12)|(1<<WGM12);  //Timer1 CTC mode, prescaler 256
	OCR1A = 0x271;					 //625 as compare value
	TIMSK |= (1<<OCIE1A);			 //Fire interrupt when compare match, approx. every 20 ms
	sei();
}

void timerService(){
	if(SREG & (1 << 7)){
		cli();
	}
	if(taskQueue[0].delay != 0)
		taskQueue[0].delay--;
	else
		flags |= 1;
	sei();
}

ISR(TIMER1_COMPA_vect){
	timerService();
}