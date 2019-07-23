/*
 * adc.h
 *
 * Created: 15.06.2019 0:07:49
 *  Author: Admin
 */ 
#include <stdint.h>
#include <avr/io.h>

#ifndef ADC_H_
#define ADC_H_

#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7

void adc_init();
uint16_t adc_read(uint8_t channel);

#endif /* ADC_H_ */