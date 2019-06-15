/*
 * bmp280.h
 *
 * Created: 08.06.2019 22:31:45
 *  Author: ThePetrovich
 */ 


#ifndef BMP280_H_
#define BMP280_H_

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <stdint.h>
#include "../../spi.h"
#include <math.h>

#ifndef BMP280config
#define BMP280_PORT PORTA
#define BMP280_DDR DDRA
#define BMP280_CS PA7
#endif

#define BMP280_REG_DIG_T1 0x88
#define BMP280_REG_DIG_T2 0x8A
#define BMP280_REG_DIG_T3 0x8C

#define BMP280_REG_DIG_P1 0x8E
#define BMP280_REG_DIG_P2 0x90
#define BMP280_REG_DIG_P3 0x92
#define BMP280_REG_DIG_P4 0x94
#define BMP280_REG_DIG_P5 0x96
#define BMP280_REG_DIG_P6 0x98
#define BMP280_REG_DIG_P7 0x9A
#define BMP280_REG_DIG_P8 0x9C
#define BMP280_REG_DIG_P9 0x9E

#define BMP280_REG_CHIPID 0xD0
#define BMP280_REG_VERSION 0xD1
#define BMP280_REG_SOFTRESET 0xE0

#define BMP280_REG_CAL26 0xE1

#define BMP280_REG_CONTROL 0xF4
#define BMP280_REG_CONFIG 0xF5
#define BMP280_REG_PRESSUREDATA 0xF7
#define BMP280_REG_TEMPDATA 0xFA

void bmp280_init();
int16_t bmp280_readTemperature();
int16_t bmp280_readPressure();
int16_t bmp280_calcAltitude(float sea_prs);

#endif /* BMP280_H_ */