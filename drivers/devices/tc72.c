/*
 * tc72.c
 *
 * Created: 24.03.2019 14:20:13
 *  Author: ThePetrovich
 */ 
#include "tc72.h"

void tc72_init(){
	TC72_DDR |= (1<<TC72_CS);
	spi_busSetup(0, 0);
	TC72_PORT |= (1<<TC72_CS);
	spi_communicate(0x80);
	spi_communicate(0x00);
	TC72_PORT &= ~(1<<TC72_CS);
	_delay_ms(150);
	spi_busStop();
}

uint16_t tc72_requestTemperatureRaw(){
	spi_busSetup(0, 0);
	uint16_t raw = 0;
	uint8_t tempMSB = 0, tempLSB = 0;
	TC72_PORT |= (1<<TC72_CS);
	tempLSB = spi_communicate(0x01);
	tempMSB = spi_communicate(0x02);
	raw |= (tempMSB>>8)|(tempLSB>>8);
	TC72_PORT &= ~(1<<TC72_CS);
	spi_busStop();
	return tempMSB;
}

float tc72_calculateTemperature(uint16_t raw){
	float temp = 0;
	uint8_t buf = 0;
	for(int i = -2; i < 7; i++){
		buf = ((1<<(i+2)) & raw) >> (i+2);
		temp += (float)buf * pow(2, i);
		buf = 0;
	}
	if ((raw & (1<<10)) == (1<<10))
		temp = temp*-1;
	return temp;
}