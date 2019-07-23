/*
 * bmp280.c
 *
 * Created: 08.06.2019 22:31:31
 *  Author: ThePetrovich
 */ 
#ifndef F_CPU
#define F_CPU 8000000L
#endif

#include "../bmp280.h"
#include <util/delay.h>
#include <stdio.h>
#define bmp280_select() BMP280_PORT &= ~(1<<BMP280_CS)
#define bmp280_deselect() BMP280_PORT |= (1<<BMP280_CS)

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

int32_t temp_calibrated;

//йняршкх х бекняхоедш

void bmp280_pinSetup(){
	BMP280_DDR |= (1 << BMP280_CS);
	BMP280_PORT |= (1 << BMP280_CS);
}

uint8_t bmp280_readRegister1(uint8_t address){
	uint8_t response;
	
	spi_busSetup(SPI_PRESCALER_16, MSBFIRST, SPI_MODE3, SPI_1X);
	spi_cslow();
	bmp280_select();
	
	spi_simpleWrite(address | 0x80);
	response = spi_simpleRead(0x00);
	
	bmp280_deselect();
	spi_cshigh();
	spi_busStop();
	
	return response;
}

uint16_t bmp280_readRegister2(uint8_t address){
	uint16_t response;
	
	spi_busSetup(SPI_PRESCALER_16, MSBFIRST, SPI_MODE3, SPI_1X);
	spi_cslow();
	bmp280_select();
	
	spi_simpleWrite(address | 0x80);
	response = spi_simpleRead(0x00);
	response |= spi_simpleRead(0x00)<<8;
	
	bmp280_deselect();
	spi_cshigh();
	spi_busStop();
	
	return response;
}

uint32_t bmp280_readRegister3(uint8_t address){
	uint32_t response;
	
	spi_busSetup(SPI_PRESCALER_16, MSBFIRST, SPI_MODE3, SPI_1X);
	spi_cslow();
	bmp280_select();
	
	spi_simpleWrite(address | 0x80);
	response = spi_simpleRead(0x00);
	response <<= 8;
	response |= spi_simpleRead(0x00);
	response <<= 8;
	response |= spi_simpleRead(0x00);
	
	bmp280_deselect();
	spi_cshigh();
	spi_busStop();
	
	return response;
}

void bmp280_writeRegister(uint8_t address, uint8_t data){
	
	spi_busSetup(SPI_PRESCALER_16, MSBFIRST, SPI_MODE3, SPI_1X);
	spi_cslow();
	BMP280_PORT &= ~(1<<BMP280_CS);
	
	spi_simpleWrite(address & ~0x80);
	spi_simpleWrite(data & ~0x80);
	
	bmp280_deselect();
	spi_cshigh();
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

double bmp280_readTemperature(){
	double var1, var2, t;
	int32_t adc_T = (int32_t)bmp280_readRegister3(BMP280_REG_TEMPDATA);
	adc_T >>= 4;
	var1 = (((double)adc_T)/16384.0 - (double)dig_T1/1024.0)*((double)dig_T2);
	var2 = ((((double)adc_T)/131072.0-((double)dig_T1)/8192.0)*(((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0))*((double)dig_T3);
	temp_calibrated = (int32_t)(var1 + var2);
	t = (var1 + var2)/5120.0;
	
	return t;
}

double bmp280_readPressure(){
	double var1, var2, p;
	
	int32_t adc_P = (int32_t)bmp280_readRegister3(BMP280_REG_PRESSUREDATA);
	adc_P >>= 4;
	
	var1 = ((double)temp_calibrated/2.0)-64000.0;
	var2 = var1*var1*((double)dig_P6)/32768.0;
	var2 = var2+var1*((double)dig_P5)/2.0;
	var2 = (var2/4.0)+(((double)dig_P4)*65536.0);
	var1 = (((double)dig_P3)*var1*var1/524288.0+((double)dig_P2)*var1)/524288.0;
	var1 = (1.0 + var1/32768.0)*((double)dig_P1);
	p = 1048576.0 - (double)adc_P;
	p = (p - (var2/4096.0))*6250.0/var1;
	var1 = ((double)dig_P9)*p*p/2147483648.0;
	var2 = p*((double)dig_P8)/32768.0;
	
	p = p+(var1+var2+((double)dig_P7))/16.0;
	return p;
}

int16_t bmp280_calcAltitude(float sea_prs){
	float altitude;

	float prs= bmp280_readPressure();

	altitude = 44330 * (1.0 - pow(prs / sea_prs, 0.1903));

	return (int16_t)altitude;
}

void bmp280_printCalibrationData(){
	char msg[64];
	sprintf(msg, "%d\r\n", dig_T1);
	debug_logMessage(PGM_ON, L_INFO, PSTR("[DEBUG]BMP280: dig_T1="));
	debug_logMessage(PGM_OFF, L_NONE, "%d\r\n", dig_T1);
	debug_logMessage(PGM_ON, L_INFO, PSTR("[DEBUG]BMP280: dig_T2="));
	debug_logMessage(PGM_OFF, L_NONE, "%d\r\n", dig_T2);
	debug_logMessage(PGM_ON, L_INFO, PSTR("[DEBUG]BMP280: dig_T3="));
	debug_logMessage(PGM_OFF, L_NONE, "%d\r\n", dig_T3);
	debug_logMessage(PGM_ON, L_INFO, PSTR("[DEBUG]BMP280: dig_P1="));
	debug_logMessage(PGM_OFF, L_NONE, "%d\r\n", dig_P1);
	debug_logMessage(PGM_ON, L_INFO, PSTR("[DEBUG]BMP280: dig_P2="));
	debug_logMessage(PGM_OFF, L_NONE, "%d\r\n", dig_P2);
	debug_logMessage(PGM_ON, L_INFO, PSTR("[DEBUG]BMP280: dig_P3="));
	debug_logMessage(PGM_OFF, L_NONE, "%d\r\n", dig_P3);
	debug_logMessage(PGM_ON, L_INFO, PSTR("[DEBUG]BMP280: dig_P4="));
	debug_logMessage(PGM_OFF, L_NONE, "%d\r\n", dig_P4);
	debug_logMessage(PGM_ON, L_INFO, PSTR("[DEBUG]BMP280: dig_P5="));
	debug_logMessage(PGM_OFF, L_NONE, "%d\r\n", dig_P5);
	debug_logMessage(PGM_ON, L_INFO, PSTR("[DEBUG]BMP280: dig_P6="));
	debug_logMessage(PGM_OFF, L_NONE, "%d\r\n", dig_P6);
	debug_logMessage(PGM_ON, L_INFO, PSTR("[DEBUG]BMP280: dig_P7="));
	debug_logMessage(PGM_OFF, L_NONE, "%d\r\n", dig_P7);
	debug_logMessage(PGM_ON, L_INFO, PSTR("[DEBUG]BMP280: dig_P8="));
	debug_logMessage(PGM_OFF, L_NONE, "%d\r\n", dig_P8);
	debug_logMessage(PGM_ON, L_INFO, PSTR("[DEBUG]BMP280: dig_P9="));
	debug_logMessage(PGM_OFF, L_NONE, "%d\r\n", dig_P9);
	return;
}

uint8_t bmp280_init(){
	uint8_t devid = bmp280_readRegister1(0x00);
	bmp280_writeRegister(BMP280_REG_CONTROL, 0x3F);
	bmp280_readCalibrationValues();
	return devid;
}

uint8_t bmp280_checkDevId(uint8_t devid){
	if(devid != BMP280_CHIPID)
		return ERR_BMP_DEVID_MISMATCH;
	else
		return 0;
}

