/*
 * spi.h
 *
 * Created: 23.03.2019 17:27:39
 *  Author: Admin
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdint.h>

#define SPI_PORTX PORTB
#define SPI_DDRX DDRB

#define SPI_MISO 3
#define SPI_MOSI 2
#define SPI_SCK 1
#define SPI_SS 0

void spi_init();
void spi_write(uint8_t data);
uint8_t spi_read(uint8_t data);

#endif /* SPI_H_ */