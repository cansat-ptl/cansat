/*
 * tc72.h
 *
 * Created: 24.03.2019 14:20:05
 *  Author: Admin
 */ 


#ifndef TC72_H_
#define TC72_H_

#include <avr/io.h>
#include <stdint.h>
#include <math.h>
#include <util/delay.h>
#include "../spi.h"

#define TC72_PORT PORTA
#define TC72_DDR DDRA
#define TC72_CS 7

void tc72_init();
uint16_t tc72_requestTemperatureRaw();
float tc72_calculateTemperature(uint16_t raw);

#endif /* TC72_H_ */