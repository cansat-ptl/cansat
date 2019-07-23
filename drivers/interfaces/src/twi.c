/*
 * twi.c
 *
 * Created: 07.06.2019 23:15:20
 *  Author: ThePetrovich
 */ 
#include "../twi.h"

void twi_init(){
	TWBR = 2;
	TWSR = 0;
	PORTD = (1<<PD1) | (1<<PD0);
	DDRD = ~(1<<PD1) & (1<<PD0);
	TWCR = (1<<TWINT)|(1<<TWEN);
}

uint8_t twi_start(uint8_t address){
	TWCR = 0;
	sendStart();
	
	while(!(TWCR & (1<<TWINT)));
	if((TWSR & TW_OK) != TW_START) 
		return TWI_START_ERR;
		
	TWDR = address;
	TWCR = (1<<TWINT)|(1<<TWEN);
	
	while(!(TWCR & (1<<TWINT)));

	uint8_t twst = TW_STATUS & TW_OK;
	if((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK)) 
		return TWI_ACK_ERR;
		
	return 0;
}

uint8_t twi_write(uint8_t data){
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	
	while(!(TWCR & (1<<TWINT)));
	
	if((TWSR & TW_OK) != TW_MT_DATA_ACK)
		return TWI_WRITE_ERR;
		
	return 0;
}

uint8_t twi_read_ack(){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(!(TWCR & (1<<TWINT)));
	return TWDR;
}

uint8_t twi_read_nack(){
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	return TWDR;
}

uint8_t twi_writeRegister(uint8_t address, uint8_t _register, uint8_t* data, uint16_t size){
	if(twi_start(address | TW_BUS_ERROR)) 
		return TWI_START_ERR;
		
	twi_write(_register);
	for (int i = 0; i < size; i++)
		if (twi_write(data[i])) 
			return TWI_WRITE_ERR;
	twi_stop();
	
	return 0;
}

uint8_t twi_readRegister(uint8_t address, uint8_t _register, uint8_t* data, uint16_t size){
	if(twi_start(address | TW_BUS_ERROR))
		return TWI_START_ERR;
		
	twi_write(_register);
	if (twi_start(address | 0x01)) 
		return TWI_READ_ERR;
		
	for (int i = 0; i < (size-1); i++)
		data[i] = twi_read_ack();
	data[(size-1)] = twi_read_nack();
	twi_stop();
	
	return 0;
}

uint8_t twi_transfer(uint8_t mode, uint8_t address, uint8_t* data, uint8_t size){
	if(twi_start(address | mode))
	return 1;
	
	if(mode == TWI_WRITE)
	for(int i = 0; i < size; i++)
		if(twi_write(data[i]) != 0)
			return TWI_WRITE_ERR;
	
	else {
		for(int i = 0; i < (size-1); i++)
			data[i] = twi_read_ack();
		data[(size-1)] = twi_read_nack();
	}
	twi_stop();
	
	return 0;
}

void twi_stop(){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}
