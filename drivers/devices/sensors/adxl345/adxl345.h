/*
 * adxl345.h
 *
 * Created: 23.03.2019 17:28:15
 *  Author: ThePetrovich
 */ 


#ifndef ADXL345_H_
#define ADXL345_H_

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "../../../interfaces/spi.h"
#include "../../../driver_config.h"

#define ADXLDRV_VER "0.0.5-bleeding"
#define ADXLDRV_TIMESTAMP __TIMESTAMP__

#define ADXL345_DEFAULT_DEVID 0xE5
#define ADXL345_REG_DEVID 0x00
#define ADXL345_REG_POWERCTL 0x2D
#define ADXL345_REG_FORMAT 0x31
#define ADXL345_REG_X0 0x32
#define ADXL345_REG_X1 0x33
#define ADXL345_REG_Y0 0x34
#define ADXL345_REG_Y1 0x35
#define ADXL345_REG_Z0 0x36
#define ADXL345_REG_Z1 0x37

#define ERR_ADXL_DEVID_MISMATCH 1

void adxl345_pinSetup();
uint8_t	adxl345_init();
uint8_t adxl345_checkDevId(uint8_t devid);
int16_t adxl345_readX();
int16_t adxl345_readY();
int16_t adxl345_readZ();

#endif /* ADXL345_H_ */