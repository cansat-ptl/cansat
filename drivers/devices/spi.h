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
#define SPIMODEL 1		//CPOL bit of SPCR
#define SPIMODEH 1		//CPHA bit of SPCR
#define SPISPDL 0		//SPR0 bit of SPCR
#define SPISPDH 0		//SPR1 bit of SPCR
#endif

void spi_init();
void spi_busSetup(uint8_t dord, uint8_t mode);
void spi_write(uint8_t data);
uint8_t spi_read(uint8_t data);
void spi_busStop();

#endif /* SPI_H_ */