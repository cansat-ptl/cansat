/*
 * drivers.h
 *
 * Created: 14.05.2019 19:24:18
 *  Author: ThePetrovich
 */ 

#ifndef DRIVERS_H_
#define DRIVERS_H_

#define SDCARD_DRV_VER "R0.0.2"
#define SDCARD_DRV_TIMESTAMP __TIMESTAMP__

#include "../drivers/interfaces/uart.h"
#include "../drivers/interfaces/spi.h"
#include "../drivers/interfaces/twi.h"
#include "../drivers/interfaces/adc.h"
#include "../drivers/devices/sensors/adxl345/adxl345.h"
#include "../drivers/devices/sensors/bmp280/bmp280.h"
#include "../drivers/devices/sensors/ds18b20/ds18b20.h"
#include "../drivers/devices/sensors/pololu/imuv3.h"
#include "../drivers/devices/radio/nRF.h"
#include "../drivers/devices/system/pff3a/pff.h"
#include "../drivers/devices/system/pff3a/diskio.h"

FATFS fs;
WORD logfile;

#endif /* DRIVERS_H_ */