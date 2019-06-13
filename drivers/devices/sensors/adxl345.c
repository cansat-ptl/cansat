/*
 * adxl345.c
 *
 * Created: 23.03.2019 17:28:05
 *  Author: ThePetrovich
 */ 
#include "adxl345.h"

uint8_t adxl345_init(){
	spi_busSetup(SPI_PRESCALER_4, LSBFIRST, SPI_MODE3, SPI_1X);
	cli();
	ADXL345_PORT |= (1<<ADXL345_CS);
	uint8_t devid = spi_readRegister(ADXL345_REG_DEVID, 1, 0);
	if(devid != ADXL345_DEFAULT_DEVID){
		ADXL345_PORT &= ~(1<<ADXL345_CS);
		spi_busStop();
		sei();
		return ERR_ADXL_DEVID_MISMATCH;
	}
	spi_writeRegister(ADXL345_REG_POWERCTL, ADXL345_VALUE_POWERCTL, 0x80, 1);
	spi_writeRegister(ADXL345_REG_FORMAT, ADXL345_VALUE_FORMAT, 0x80, 1);
	sei();
	ADXL345_PORT &= ~(1<<ADXL345_CS);
	spi_busStop();
	return 0;
}

float adxl345_readX(){
	spi_busSetup(SPI_PRESCALER_4, LSBFIRST, SPI_MODE3, SPI_1X);
	cli();
	ADXL345_PORT |= (1<<ADXL345_CS);
	int16_t x0 = spi_readRegister(ADXL345_REG_X0, 1, 0);
	int16_t x1 = spi_readRegister(ADXL345_REG_X1, 1, 0);
	ADXL345_PORT &= ~(1<<ADXL345_CS);
	sei();
	spi_busStop();
	x1 = x1 << 8;
	x1 += x0;
	return ((float)x1/32.0);
}

float adxl345_readY(){
	spi_busSetup(SPI_PRESCALER_4, LSBFIRST, SPI_MODE3, SPI_1X);
	cli();
	ADXL345_PORT |= (1<<ADXL345_CS);
	int16_t y0 = spi_readRegister(ADXL345_REG_Y0, 1, 0);
	int16_t y1 = spi_readRegister(ADXL345_REG_Y1, 1, 0);
	ADXL345_PORT &= ~(1<<ADXL345_CS);
	sei();
	spi_busStop();
	y1 = y1 << 8;
	y1 += y0;
	return ((float)y1/32.0)*10;
}

float adxl345_readZ(){
	spi_busSetup(SPI_PRESCALER_4, LSBFIRST, SPI_MODE3, SPI_1X);
	cli();
	ADXL345_PORT |= (1<<ADXL345_CS);
	int16_t z0 = spi_readRegister(ADXL345_REG_Z0, 1, 0);
	int16_t z1 = spi_readRegister(ADXL345_REG_Z1, 1, 0);
	ADXL345_PORT &= ~(1<<ADXL345_CS);
	sei();
	spi_busStop();
	z1 = z1 << 8;
	z1 += z0;
	return ((float)z1/32.0)*10;
}