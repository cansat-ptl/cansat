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

void spi_busSetup(uint8_t dord, uint8_t mode){
	SPCR = (SPIIEN<<SPIE)|(SPIEN<<SPE)|(dord<<DORD)|(SPIMSTR<<MSTR)|(mode<<CPHA)|(SPISPDH<<SPR1)|(SPISPDL<<SPR0);
	spi_write(0x00);
}
void spi_write(uint8_t data)
{
	SPI_PORT &= ~(1<<SPI_SS);
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	SPI_PORT |= (1<<SPI_SS);
}

uint8_t spi_read(uint8_t data)
{
	uint8_t response;
	SPI_PORT &= ~(1<<SPI_SS);
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	response = SPDR;
	SPI_PORT |= (1<<SPI_SS);
	return response;
}

void spi_busStop(){
	SPCR = 0;
}