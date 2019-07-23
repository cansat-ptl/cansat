/*
 * spi.h
 *
 * Created: 23.03.2019 17:27:39
 *  Author: ThePetrovich
 */ 

#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdint.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include "../driver_config.h"

#define SPIDRV_VER "0.9.6-rc3"
#define SPIDRV_TIMESTAMP __TIMESTAMP__

//SPI mode predefines
#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3
/*
 *	Possible SPI modes:
 *	0 - 0b00 - CPOL=0, CPHA=0 - mode 0
 *	1 - 0b01 - CPOL=0, CPHA=1 - mode 1
 *  2 - 0b10 - CPOL=1, CPHA=0 - mode 2
 *	3 - 0b11 - CPOL=1, CPHA=1 - mode 3
 */
//SPI bit order predefines
#define MSBFIRST 0
#define LSBFIRST 1
//SPI speed predefines
#define SPI_PRESCALER_4 0
#define SPI_PRESCALER_16 1
#define SPI_PRESCALER_64 2
#define SPI_PRESCALER_128 3
//SPI transfer modes
#define SPI_WRITE 1
#define SPI_READ 0
//SPI speed multipliers
#define SPI_2X 1
#define SPI_1X 0

#define spi_cslow() SPI_PORT &= ~(1<<SPI_SS)
#define spi_cshigh() SPI_PORT |= (1<<SPI_SS)

/*------------------------------------------------------------
SPI init - spi_init()
Sets up SPI bus pins.
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void spi_init();

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
void spi_busSetup(uint8_t speed, uint8_t dord, uint8_t mode, uint8_t spi2x);

/*------------------------------------------------------------
SPI write function - spi_write(uint8_t data)
Transmits and retrieves data from the SPI module.
Arguments: uint8_t data - data that will be sent
Returns: uint8_t - received data
------------------------------------------------------------*/
uint8_t spi_write(uint8_t data);

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
void spi_writeRegister(uint8_t address, uint8_t data, uint8_t mask, uint8_t isInverted);

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
uint8_t spi_readRegister(uint8_t address, uint8_t isDelayed, uint8_t mask, uint8_t isInverted);

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
void spi_transfer(uint8_t type, uint8_t address, uint8_t * data, uint8_t size, uint8_t mask);

/*------------------------------------------------------------
SPI single read function - spi_simpleRead(uint8_t filler)
Receives a single byte from the SPI bus.
Arguments: uint8_t filler - a placeholder byte to complete
			the read operation.
Returns: uint8_t - received data
------------------------------------------------------------*/
uint8_t spi_simpleRead(uint8_t filler);

/*------------------------------------------------------------
SPI single write function - spi_simpleWrite(uint8_t data)
Sends a single byte to the SPI bus.
Arguments: uint8_t data - data to be sent
Returns: nothing
------------------------------------------------------------*/
void spi_simpleWrite(uint8_t data);

/*------------------------------------------------------------
SPI bus disable - spi_busStop()
Disables the SPI bus and makes it available for other devices.
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void spi_busStop();

#endif /* SPI_H_ */