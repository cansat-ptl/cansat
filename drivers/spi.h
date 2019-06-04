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

#define SPIDRV_VER "0.9.1-rc1"
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
#define SPIEN 1			//SPE bit of SPCR
#define SPIIEN 0		//SPIE bit of SPCR
#define SPIDORD 0		//DORD bit of SPCR
#define SPIMSTR 1		//MSTR bit of SPCR
#define SPIMODEL 0		//CPOL bit of SPCR
#define SPIMODEH 0		//CPHA bit of SPCR
#define SPISPDL 0		//SPR0 bit of SPCR
#define SPISPDH 0		//SPR1 bit of SPCR
#endif
/*
 *	Possible SPI modes:
 *	0 - 0b00 - CPOL=0, CPHA=0 - mode 0
 *	1 - 0b01 - CPOL=0, CPHA=1 - mode 1
 *  2 - 0b10 - CPOL=1, CPHA=0 - mode 2
 *	3 - 0b11 - CPOL=1, CPHA=1 - mode 3
 */
void spi_init();
void spi_busSetup(uint8_t dord, uint8_t mode);
uint8_t spi_communicate(uint8_t data);
void spi_busStop();

#endif /* SPI_H_ */