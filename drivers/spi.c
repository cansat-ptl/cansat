/*
 * spi.c
 *
 * Created: 07.03.2019 20:21:54
 *  Author: Admin
 */ 

#include "spi.h"

void spi_init(){
	SPI_DDRX |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(0<<SPI_MISO);
	SPI_PORTX |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(1<<SPI_MISO);
	
	SPCR = (0<<SPIE)|(1<<SPE)|(1<<DORD)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA)|(0<<SPR1)|(0<<SPR0);
	SPSR = (0<<SPI2X);	
}

void spi_write(uint8_t data)
{
	SPI_PORTX &= ~(1<<SPI_SS);
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	SPI_PORTX |= (1<<SPI_SS);
}

uint8_t spi_read(uint8_t data)
{
	uint8_t response;
	SPI_PORTX &= ~(1<<SPI_SS);
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	response = SPDR;
	SPI_PORTX |= (1<<SPI_SS);
	return response;
}