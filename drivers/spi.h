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

#define SPIDRV_VER "0.9.3-rc2"
#define SPIDRV_TIMESTAMP __TIMESTAMP__

#ifndef SPIconfig
//SPI module port registers
#define SPI_PORT PORTB
#define SPI_DDR DDRB
//SPI module pins
#define SPI_MISO 3
#define SPI_MOSI 2
#define SPI_SCK 1
#define SPI_SS 0
//SPI module settings
#define SPIEN 1			//SPE bit of SPCR - SPI module enable
#define SPIIEN 0		//SPIE bit of SPCR - SPI interrupt enable
#define SPIDORD 0		//DORD bit of SPCR - default SPI bit order
#define SPIMSTR 1		//MSTR bit of SPCR - SPI master mode enable
#define SPIMODEL 0		//CPOL bit of SPCR - SPI clock polarity
#define SPIMODEH 0		//CPHA bit of SPCR - SPI clock phase
#define SPISPDL 0		//SPR0 bit of SPCR - SPI speed prescaler bit 0
#define SPISPDH 0		//SPR1 bit of SPCR - SPI speed prescaler bit 1
#endif
//SPI mode predefines
#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3
//SPI bit order predefines
#define MSBFIRST 1
#define LSBFIRST 0
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
void spi_writeRegister(uint8_t address, uint8_t data);
uint8_t spi_readRegister(uint8_t address, uint8_t isDelayed);
void spi_transfer(uint8_t type, uint8_t address, uint8_t * data, uint8_t size);
uint8_t spi_simpleRead();
void spi_simpleWrite(uint8_t data);
void spi_busStop();

#endif /* SPI_H_ */