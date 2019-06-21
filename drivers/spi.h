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
#include "driver_config.h"

#define SPIDRV_VER "0.9.5-rc3"
#define SPIDRV_TIMESTAMP __TIMESTAMP__

//SPI mode predefines
#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3
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

/*
 *	Possible SPI modes:
 *	0 - 0b00 - CPOL=0, CPHA=0 - mode 0
 *	1 - 0b01 - CPOL=0, CPHA=1 - mode 1
 *  2 - 0b10 - CPOL=1, CPHA=0 - mode 2
 *	3 - 0b11 - CPOL=1, CPHA=1 - mode 3
 */
void spi_init();
void spi_busSetup(uint8_t speed, uint8_t dord, uint8_t mode, uint8_t spi2x);
uint8_t spi_write(uint8_t data);
void spi_writeRegister(uint8_t address, uint8_t data, uint8_t mask, uint8_t isInverted);
uint8_t spi_readRegister(uint8_t address, uint8_t isDelayed, uint8_t isInverted);
void spi_transfer(uint8_t type, uint8_t address, uint8_t * data, uint8_t size, uint8_t mask);
uint8_t spi_simpleRead(uint8_t filler);
void spi_simpleWrite(uint8_t data);
void spi_busStop();

#endif /* SPI_H_ */