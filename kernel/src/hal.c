/*
 * hal.c
 *
 * Created: 21.06.2019 23:53:24
 *  Author: ThePetrovich
 */ 

#include "../hal.h"

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
