/*
 * ds18b20.c
 *
 * Created: 17.05.2019 17:58:11
 *  Author: ThePetrovich
 */ 
#include "../ds18b20.h"

uint8_t ds18b20_reset(){
	cli();
	DS18B20_PORT &= ~(1 << DS18B20_IO);
	DS18B20_DDR |= (1 << DS18B20_IO);
	_delay_us(480);
	DS18B20_DDR &= ~(1 << DS18B20_IO);
	_delay_us(60);
	uint8_t i = (DS18B20_PIN & (1<<DS18B20_IO));
	_delay_us(410);
	sei();
	return i;
}

void ds18b20_writeBit(uint8_t value){
	cli();
	DS18B20_PORT &= ~(1 << DS18B20_IO);
	DS18B20_DDR |= (1 << DS18B20_IO);
	_delay_us(6);
	if(value) 
		DS18B20_DDR &= ~(1 << DS18B20_IO);
	_delay_us(60);
	DS18B20_DDR &= ~(1 << DS18B20_IO);
	sei();
}

uint8_t ds18b20_readBit(void){
	uint8_t value = 0;
	cli();
	DS18B20_PORT &= ~(1 << DS18B20_IO);
	DS18B20_DDR |= (1 << DS18B20_IO);
	_delay_us(6);
	DS18B20_DDR &= ~(1 << DS18B20_IO);
	_delay_us(10);
	if(DS18B20_PIN & (1<<DS18B20_IO)) 
		value = 1;
	_delay_us(55);
	sei();
	return value;
}

uint8_t ds18b20_readByte(void){
	uint8_t n = 0;
	for(uint8_t i = 8; i != 0; i--){
		n = (n >> 1);
		n |= (ds18b20_readBit() << 7);
	}
	return n;
}

void ds18b20_writeByte(uint8_t value){
	for(uint8_t i = 8; i != 0; i--){
		ds18b20_writeBit(value & 1);
		value = (value >> 1);
	}
}

void ds18b20_requestTemperature(void){
	ds18b20_reset();
	ds18b20_writeByte(DS_CMD_SKIPROM);
	ds18b20_writeByte(DS_CMD_CONVERTTEMP);
}
	
char * ds18b20_readTemperature(){
	static char ds_data[16];
	uint8_t raw[2];
	int8_t digit;
	uint16_t decimal;
	//float value = 0;
	
	ds18b20_reset();
	ds18b20_writeByte(DS_CMD_SKIPROM);
	ds18b20_writeByte(DS_CMD_RSCRATCHPAD);
	
	raw[0] = ds18b20_readByte();
	raw[1] = ds18b20_readByte();
	ds18b20_reset();
	
	digit = raw[0] >> 4;
	digit |= (raw[1] & 0x7) << 4;
	
	decimal = raw[0] & 0xF;
	decimal *= 625;
	
	sprintf(ds_data, "%+d.%04u", digit, decimal);
	/*if(digit < 0) value -= (float)(decimal / 1000);
	else value += (float)(decimal / 1000);*/
	
	return ds_data;
}