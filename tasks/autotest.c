/*
 * autotest.c
 *
 * Created: 14.06.2019 19:23:32
 *  Author: Admin
 */ 
#include "tasks.h"
#include "../kernel/globals.h"

float convertToDecimal(float lat);

void adxl345_test(){
	kernel_stopTimer();
	char msg[64];
	logMessage("Testing ADXL345...\r\n", 1);
	if(adxl345_init() == ERR_ADXL_DEVID_MISMATCH)
		logMessage("ADXL init error: no ADXL345 connected/DEVID mismatch\r\n", 3);
	for(int i = 0; i < 10; i++){
		writePin(&PORTG, PG3, HIGH);
		delay(250);
		int16_t ax = adxl345_readX();
		int16_t ay = adxl345_readY();
		int16_t az = adxl345_readZ();
		sprintf(msg, "ADXL data: %d %d %d\r\n", ax, ay, az);
		logMessage(msg, 1);
		writePin(&PORTG, PG3, LOW);
		delay(250);
	}
	kernel_startTimer();
}

void bmp280_test(){
	kernel_stopTimer();
	char msg[64];
	logMessage("Testing BMP280...\r\n", 1);
	bmp280_init();
	for(int i = 0; i < 10; i++){
		writePin(&PORTG, PG3, HIGH);
		delay(250);
		int16_t t = bmp280_readTemperature();
		int16_t p = bmp280_readPressure();
		sprintf(msg, "BMP data: %d %d\r\n", t, p);
		logMessage(msg, 1);
		writePin(&PORTG, PG3, LOW);
		delay(250);
	}
	kernel_startTimer();
}

void ds18b20_test(){
	kernel_stopTimer();
	char msg[64];
	logMessage("Testing DS18B20...\r\n", 1);
	for(int i = 0; i < 10; i++){
		writePin(&PORTG, PG3, HIGH);
		delay(250);
		char * t = ds18b20_readTemperature();
		sprintf(msg, "DS data: %s\r\n", t);
		logMessage(msg, 1);
		writePin(&PORTG, PG3, LOW);
		delay(250);
	}
	kernel_startTimer();
}

void imu_test(){
	kernel_stopTimer();
	logMessage("Testing LSM303...\r\n", 1);
	imu_init();
	for(int i = 0; i < 10; i++){
		writePin(&PORTG, PG3, HIGH);
		delay(250);
		imu_read();
		logMessage("Reading imu data\r\n", 1);
		writePin(&PORTG, PG3, LOW);
		delay(250);
	}
	kernel_startTimer();
}

void gps_test(){
	kernel_stopTimer();
	logMessage("Testing GPS...\r\n", 1);
	char msg[64];
	for(int i = 0; i < 10; i++){
		writePin(&PORTG, PG3, HIGH);
		delay(250);
		float lat = convertToDecimal(GPS.latitude);
		float lon = convertToDecimal(GPS.longitude);
		sprintf(msg, "GPS: %d %d %d %f %f\r\n", GPS.day, GPS.month, GPS.year, lat, lon);
		logMessage(msg,1);
		writePin(&PORTG, PG3, LOW);
		delay(250);
	}
	kernel_startTimer();
}
