/*
 * spi.c
 *
 * Created: 07.03.2019 20:21:54
 *  Author: ThePetrovich
 */ 

#include "spi.h"

void spi_init(){
	SPI_DDR |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(0<<SPI_MISO);
	SPI_PORT |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(1<<SPI_MISO);
	SPSR = (0<<SPI2X);	
}

uint8_t spi_write(uint8_t data){
	uint8_t response;
	
	SPI_PORT &= ~(1<<SPI_SS);
	SPDR = data;
	
	while(!(SPSR & (1<<SPIF)));
	response = SPDR;
	
	SPI_PORT |= (1<<SPI_SS);
	
	return response;
}

void spi_writeRegister(uint8_t address, uint8_t data){
	SPI_PORT &= ~(1<<SPI_SS);
	
	SPDR = (address | 0x80);
	while(!(SPSR & (1<<SPIF)));
	
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	
	SPI_PORT |= (1<<SPI_SS);
}

uint8_t spi_readRegister(uint8_t address, uint8_t isDelayed){
	uint8_t response;
	
	SPI_PORT &= ~(1<<SPI_SS);
	SPDR = (address & ~0x80);
	while(!(SPSR & (1<<SPIF)));
	
	if(isDelayed){
		SPDR = 0xFF;
		while(!(SPSR & (1<<SPIF)));
	}
	
	response = SPDR;
	SPI_PORT |= (1<<SPI_SS);
	
	return response;
}

void spi_transfer(uint8_t type, uint8_t address, uint8_t * data, uint8_t size){
	SPI_PORT &= ~(1<<SPI_SS);
	
	if(type == SPI_WRITE)
		SPDR = (address | 0x80);
	else
		SPDR = (address & ~0x80);
	
	for(int i = 0; i < size; i++){
		SPDR = data[i];
		while(!(SPSR & (1<<SPIF)));
		if(type == SPI_READ)
			data[i] = SPDR;
	}
	while(!(SPSR & (1<<SPIF)));
	SPI_PORT |= (1<<SPI_SS);
}

void spi_busSetup(uint8_t speed, uint8_t dord, uint8_t mode, uint8_t spi2x){
	SPCR = (SPIIEN<<SPIE)|(SPIEN<<SPE)|(dord<<DORD)|(SPIMSTR<<MSTR)|(mode<<CPHA)|(speed << SPR0);
	
	if(spi2x) 
		SPSR |= (1 << SPI2X);	
	else 
		SPSR &= ~(1 << SPI2X);	
	//spi_communicate(0x00);
}

void spi_simpleWrite(uint8_t data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}
uint8_t spi_simpleRead(){
	uint8_t response;
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));
	response = SPDR;
	return response;
}

void spi_busStop(){
	SPCR = 0; //TODO: save interrupt flag - for slave mode
	SPSR &= ~(1 << SPI2X);	
}