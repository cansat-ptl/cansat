/*
 * adc.c
 *
 * Created: 15.06.2019 0:07:38
 *  Author: Admin
 */ 
#include "../adc.h"

void adc_init(){
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //prescaler 64
	ADMUX |= (0<<REFS1)|(1<<REFS0);
}

uint16_t adc_read(uint8_t channel){
	ADMUX = (0<<REFS1)|(1<<REFS0)|(channel << MUX0);
	ADCSRA |= (1<<ADSC);
	while((ADCSRA & (1<<ADSC)));
	return (uint16_t)ADC;
}