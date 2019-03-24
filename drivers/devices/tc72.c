/*
 * tc72.c
 *
 * Created: 24.03.2019 14:20:13
 *  Author: Admin
 */ 
#include "tc72.h"

void tc72_init(){
	spi_busSetup(1, 3);
	TC72_DDR |= (1<<TC72_CS);
	TC72_PORT |= (1<<TC72_CS);
	spi_write(0x80);
	spi_write(0x00);
	TC72_PORT &= ~(1<<TC72_CS);
	_delay_ms(150);
	spi_busStop();
}

uint16_t tc72_requestTemperatureRaw(){
	spi_busSetup(1, 3);
	TC72_PORT |= (1<<TC72_CS);
	uint16_t raw = 0;
	uint8_t tempMSB = 0, tempLSB = 0;
	tempLSB = spi_read(0x01);
	tempMSB = spi_read(0x02);
	raw |= (tempMSB>>6)|(tempLSB>>6);
	TC72_PORT &= ~(1<<TC72_CS);
	spi_busStop();
	return raw;
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