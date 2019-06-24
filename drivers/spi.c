/*
 * spi.c
 *
 * Created: 07.03.2019 20:21:54
 *  Author: ThePetrovich
 */ 

#include "spi.h"

/*------------------------------------------------------------
SPI init - spi_init()
Sets up SPI bus pins.
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void spi_init(){
	SPI_DDR |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(0<<SPI_MISO);
	SPI_PORT |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(1<<SPI_MISO);
	SPSR = (0<<SPI2X);	
}

/*------------------------------------------------------------
SPI write function - spi_write(uint8_t data)
Transmits and retrieves data from the SPI module.
Arguments: uint8_t data - data that will be sent
Returns: uint8_t - received data
------------------------------------------------------------*/
uint8_t spi_write(uint8_t data){	
	SPI_PORT &= ~(1<<SPI_SS);
	SPDR = data;
	
	while(!(SPSR & (1<<SPIF)));
	
	SPI_PORT |= (1<<SPI_SS);
	
	return SPDR;
}

/*------------------------------------------------------------
SPI register write function - spi_writeRegister(uint8_t address, uint8_t data, uint8_t mask, uint8_t isInverted)
Transmits and retrieves a single register from the SPI module.
Arguments: uint8_t address - register address
		   uint8_t data - value to be written
		   uint8_t mask - address mask - required for some devices
			to set up R/W mode. Leave 0 if no mask should be applied
		   uint8_t isInverted - determines how mask will be applied.
			0 - address | mask, 1 - address & ~mask
Returns: nothing
------------------------------------------------------------*/
void spi_writeRegister(uint8_t address, uint8_t data, uint8_t mask, uint8_t isInverted){
	SPI_PORT &= ~(1<<SPI_SS);
	
	if(isInverted)
		SPDR = (address & ~mask);
	else
		SPDR = (address | mask);	
	while(!(SPSR & (1<<SPIF)));
	
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	
	SPI_PORT |= (1<<SPI_SS);
}

/*------------------------------------------------------------
SPI register read function - spi_readRegister(uint8_t address, uint8_t isDelayed, uint8_t isInverted)
Transmits and retrieves a single register from the SPI module.
Arguments: uint8_t address - register address
		   uint8_t isDelayed - if set to 1, a 0xFF will be sent
		    after a register read request
		   uint8_t mask - address mask - required for some devices
		    to set up R/W mode. Leave 0 if no mask should be applied
		   uint8_t isInverted - determines how mask will be applied.
			0 - address | mask, 1 - address & ~mask
Returns: uint8_t - register value
------------------------------------------------------------*/
uint8_t spi_readRegister(uint8_t address, uint8_t isDelayed, uint8_t mask, uint8_t isInverted){
	SPI_PORT &= ~(1<<SPI_SS);
	
	if(isInverted)
		SPDR = (address & ~0x80);
	else
		SPDR = (address | 0x80);
	while(!(SPSR & (1<<SPIF)));
	
	if(isDelayed){
		SPDR = 0xFF;
		while(!(SPSR & (1<<SPIF)));
	}
	
	SPI_PORT |= (1<<SPI_SS);
	
	return SPDR;
}

/*------------------------------------------------------------
SPI universal IO function - spi_transfer(uint8_t type, uint8_t address, uint8_t * data, uint8_t size, uint8_t mask)
Transmits and retrieves multiple values through the SPI module.
Arguments: uint8_t type - operation type. 
			Values: SPI_WRITE, SPI_READ
			If type set to SPI_READ, received data will be written to
			the same array as was set to uint8_t * data.
		   uint8_t * data - pointer to data for transmssion array.
		   uint8_t size - amount of data to be sent.
		   uint8_t mask - address mask - required for some devices
			to set up R/W mode. Leave 0 if no mask should be applied.
Returns: nothing
------------------------------------------------------------*/
void spi_transfer(uint8_t type, uint8_t address, uint8_t * data, uint8_t size, uint8_t mask){
	SPI_PORT &= ~(1<<SPI_SS);
	
	if(type == SPI_WRITE)
		SPDR = (address | mask);
	else
		SPDR = (address & ~mask);
	
	for(int i = 0; i < size; i++){
		SPDR = data[i];
		while(!(SPSR & (1<<SPIF)));
		if(type == SPI_READ)
			data[i] = SPDR;
	}
	while(!(SPSR & (1<<SPIF)));
	SPI_PORT |= (1<<SPI_SS);
}

/*------------------------------------------------------------
SPI general setup function - spi_busSetup(uint8_t speed, uint8_t dord, uint8_t mode, uint8_t spi2x)
Sets up the SPI bus registers.
Arguments: uint8_t speed - SPI SCKL prescaler. 
			Values: SPI_PRESCALER_4, SPI_PRESCALER_16, SPI_PRESCALER_64, SPI_PRESCALER_128
		   uint8_t dord - bit order
			Values: MSBFIRST, LSBFIRST
		   uint8_t mode - CPOL and CPHA setup - see drivers/spi.h
			Values: SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3
		   uint8_t spi2x - if set to SPI_2X, doubles the SCKL speed
			Values: SPI_1X, SPI_2X
Returns: nothing
------------------------------------------------------------*/
void spi_busSetup(uint8_t speed, uint8_t dord, uint8_t mode, uint8_t spi2x){
	SPCR = (SPIIEN<<SPIE)|(SPIEN<<SPE)|(dord<<DORD)|(SPIMSTR<<MSTR)|(mode<<CPHA)|(speed << SPR0);
	
	if(spi2x) 
		SPSR |= (1 << SPI2X);	
	else 
		SPSR &= ~(1 << SPI2X);	
	//spi_communicate(0x00);
}

/*------------------------------------------------------------
SPI single write function - spi_simpleWrite(uint8_t data)
Sends a single byte to the SPI bus.
Arguments: uint8_t data - data to be sent 
Returns: nothing
------------------------------------------------------------*/
void spi_simpleWrite(uint8_t data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}

/*------------------------------------------------------------
SPI single read function - spi_simpleRead(uint8_t filler)
Receives a single byte from the SPI bus.
Arguments: uint8_t filler - a placeholder byte to complete
			the read operation.
Returns: uint8_t - received data
------------------------------------------------------------*/
uint8_t spi_simpleRead(uint8_t filler){
	SPDR = filler;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

/*------------------------------------------------------------
SPI bus disable - spi_busStop()
Disables the SPI bus and makes it available for other devices.
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void spi_busStop(){
	SPCR = 0; //TODO: save interrupt flag - for slave mode
	SPSR &= ~(1 << SPI2X);	
}