/*
 * bmp280.c
 *
 * Created: 08.06.2019 22:31:31
 *  Author: ThePetrovich
 */ 
#include "bmp280.h"

uint16_t dig_T1;
int16_t  dig_T2;
int16_t  dig_T3;

uint16_t dig_P1;
int16_t  dig_P2;
int16_t  dig_P3;
int16_t  dig_P4;
int16_t  dig_P5;
int16_t  dig_P6;
int16_t  dig_P7;
int16_t  dig_P8;
int16_t  dig_P9;

uint8_t  dig_H1;
int16_t  dig_H2;
uint8_t  dig_H3;
int16_t  dig_H4;
int16_t  dig_H5;
int8_t   dig_H6;

int32_t temp_calibrated;

//йняршкх х бекняхоедш

uint8_t bmp280_readRegister1(uint8_t address){
	uint8_t response;
	
	spi_busSetup(SPI_PRESCALER_4, LSBFIRST, SPI_MODE3, SPI_1X);
	SPI_PORT &= ~(1<<SPI_SS);
	BMP280_PORT &= ~(1<<BMP280_CS);
	
	spi_simpleWrite(address | 0x80);
	response = spi_simpleRead(0x00);
	
	BMP280_PORT |= (1<<BMP280_CS);
	SPI_PORT |= (1<<SPI_SS);
	spi_busStop();
	
	return response;
}

uint16_t bmp280_readRegister2(uint8_t address){
	uint16_t response;
	
	spi_busSetup(SPI_PRESCALER_4, LSBFIRST, SPI_MODE3, SPI_1X);
	SPI_PORT &= ~(1<<SPI_SS);
	BMP280_PORT &= ~(1<<BMP280_CS);
	
	spi_simpleWrite(address | 0x80);
	response = spi_simpleRead(0x00);
	response <<= 8;
	response |= spi_simpleRead(0x00);
	
	BMP280_PORT |= (1<<BMP280_CS);
	SPI_PORT |= (1<<SPI_SS);
	spi_busStop();
	
	return response;
}

uint32_t bmp280_readRegister3(uint8_t address){
	uint32_t response;
	
	spi_busSetup(SPI_PRESCALER_4, LSBFIRST, SPI_MODE3, SPI_1X);
	SPI_PORT &= ~(1<<SPI_SS);
	BMP280_PORT &= ~(1<<BMP280_CS);
	
	spi_simpleWrite(address | 0x80);
	response = spi_simpleRead(0x00);
	response <<= 8;
	response |= spi_simpleRead(0x00);
	response <<= 8;
	response |= spi_simpleRead(0x00);
	
	BMP280_PORT |= (1<<BMP280_CS);
	SPI_PORT |= (1<<SPI_SS);
	spi_busStop();
	
	return response;
}

void bmp280_writeRegister(uint8_t address, uint8_t data){
	
	spi_busSetup(SPI_PRESCALER_4, LSBFIRST, SPI_MODE3, SPI_1X);
	SPI_PORT &= ~(1<<SPI_SS);
	BMP280_PORT &= ~(1<<BMP280_CS);
	
	spi_simpleWrite(address & ~0x80);
	spi_simpleWrite(data & ~0x80);
	
	BMP280_PORT |= (1<<BMP280_CS);
	SPI_PORT |= (1<<SPI_SS);
	spi_busStop();
}

void bmp280_readCalibrationValues(){
	dig_T1 = bmp280_readRegister2(BMP280_REG_DIG_T1);
	dig_T2 = (int16_t)bmp280_readRegister2(BMP280_REG_DIG_T2);
	dig_T3 = (int16_t)bmp280_readRegister2(BMP280_REG_DIG_T3);

	dig_P1 = bmp280_readRegister2(BMP280_REG_DIG_P1);
	dig_P2 = (int16_t)bmp280_readRegister2(BMP280_REG_DIG_P2);
	dig_P3 = (int16_t)bmp280_readRegister2(BMP280_REG_DIG_P3);
	dig_P4 = (int16_t)bmp280_readRegister2(BMP280_REG_DIG_P4);
	dig_P5 = (int16_t)bmp280_readRegister2(BMP280_REG_DIG_P5);
	dig_P6 = (int16_t)bmp280_readRegister2(BMP280_REG_DIG_P6);
	dig_P7 = (int16_t)bmp280_readRegister2(BMP280_REG_DIG_P7);
	dig_P8 = (int16_t)bmp280_readRegister2(BMP280_REG_DIG_P8);
	dig_P9 = (int16_t)bmp280_readRegister2(BMP280_REG_DIG_P9);
}

int16_t bmp280_readTemperature(){
	int32_t var1, var2;

	int32_t adc_T = (int32_t)bmp280_readRegister3(BMP280_REG_TEMPDATA);
	adc_T >>= 4;

	var1  = ((((adc_T>>3) - ((int32_t)dig_T1 <<1))) * ((int32_t)dig_T2)) >> 11;
	var2  = (((((adc_T>>4) - ((int32_t)dig_T1)) * ((adc_T>>4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
	temp_calibrated = var1 + var2;

	float T  = (temp_calibrated * 5 + 128) >> 8;
	return (int16_t)T;
}

int16_t bmp280_readPressure(){
	int64_t var1, var2, p;
	bmp280_readTemperature();

	int32_t adc_P = (int32_t)bmp280_readRegister3(BMP280_REG_PRESSUREDATA);
	adc_P >>= 4;

	var1 = ((int64_t)temp_calibrated) - 128000;
	var2 = var1 * var1 * (int64_t)dig_P6;
	var2 = var2 + ((var1*(int64_t)dig_P5)<<17);
	var2 = var2 + (((int64_t)dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)dig_P3)>>8) + ((var1 * (int64_t)dig_P2)<<12); //WHAT THE FUCK I DONT EVEN
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dig_P1)>>33;

	if (var1 == 0) {
		return 0;
	}
	
	p = 1048576 - adc_P;
	p = (((p<<31) - var2)*3125) / var1;
	var1 = (((int64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)dig_P8) * p) >> 19;

	p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7)<<4);
	return (int16_t)p/256;
}

int16_t bmp280_calcAltitude(float sea_prs){
	float altitude;

	float prs= bmp280_readPressure();

	altitude = 44330 * (1.0 - pow(prs / sea_prs, 0.1903));

	return (int16_t)altitude;
}

void bmp280_init(){
	BMP280_DDR |= (1<<BMP280_CS);
	bmp280_writeRegister(BMP280_REG_CONTROL, 0x3F);
	bmp280_readCalibrationValues();
}
