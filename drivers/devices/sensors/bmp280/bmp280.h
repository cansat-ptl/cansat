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
#include "../../../interfaces/spi.h"
#include "../../../driver_config.h"
#include <math.h>

#define BMPDRV_VER "0.0.4-rc1"
#define BMPDRV_TIMESTAMP __TIMESTAMP__

#define BMP280_CHIPID 0x58
#define ERR_BMP_DEVID_MISMATCH 1

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

void bmp280_pinSetup();
uint8_t bmp280_init();
uint8_t bmp280_checkDevId(uint8_t devid);
double bmp280_readTemperature();
double bmp280_readPressure();
int16_t bmp280_calcAltitude(float sea_prs);

#include "../../../interfaces/uart.h"
void bmp280_printCalibrationData();

#endif /* BMP280_H_ */