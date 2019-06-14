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

void adc_init();
uint16_t adc_read(uint8_t channel);

#endif /* ADC_H_ */