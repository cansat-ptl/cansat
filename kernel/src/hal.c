/*
 * hal.c
 *
 * Created: 21.06.2019 23:53:24
 *  Author: ThePetrovich
 */ 

#include "../hal.h"

#ifndef KERNEL_TIMER_PRESCALER
#define KERNEL_TIMER_PRESCALER 3
#endif

void hal_setupPins()
{
	PORTA = 0;
	DDRA = 0;
	PORTB = 0;
	DDRB = 0;
	PORTC = 0;
	DDRC = 0;
	PORTD = 0;
	DDRD = 0;
	PORTE = 0;
	DDRE = 0;
	PORTF = 0;
	DDRF = 0;
	DDRG |= (1 << PG3);
	DDRB |= (1 << PD4);
	hal_setPinMode(&JUMPER_DDR, JUMPER_OUT, OUTPUT);
	hal_writePin(&JUMPER_PORT, JUMPER_OUT, HIGH);
	DDRC |= (1 << PC4);
	DDRC |= (1 << PC5);
	DDRC |= (1 << PC6);
	DDRC |= (1 << PC7);
}

void hal_setupTimer()
{
//	debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("                         [DONE]\r\n"));
	hal_disableInterrupts();
	TCCR1B |= (1 << WGM12)|(KERNEL_TIMER_PRESCALER << CS10); // prescaler 64 cs11 & cs10 = 1
	TCNT1 = 0;
	OCR1A = 125;
	hal_enableInterrupts();
//	kernel_setFlag(KFLAG_TIMER_SET, 1);
}

void hal_startTimer()
{
//	if(kernel_checkFlag(KFLAG_TIMER_SET)){
		hal_disableInterrupts();
		TIMSK |= (1 << OCIE1A);
		hal_enableInterrupts();
//		kernel_setFlag(KFLAG_TIMER_EN, 1);
//	}
}

void hal_stopTimer()
{
//	if(kernel_checkFlag(KFLAG_TIMER_SET)){
		hal_disableInterrupts();
		TIMSK &= ~(1 << OCIE1A);
		hal_enableInterrupts();
//		kernel_setFlag(KFLAG_TIMER_EN, 0);
//	}
}