/*
 * autotest.c
 *
 * Created: 14.06.2019 19:23:32
 *  Author: Admin
 */ 
#include "tasks.h"
#include "../kernel/globals.h"

void adxl345_test(){
	kernel_stopTimer();
	char msg[64];
	logMessage("Testing ADXL345...", 1);
	adxl345_init();
	for(int i = 0; i < 10; i++){
		writePin(&PORTG, PG3, HIGH);
		delay(250);
		float ax = adxl345_readX();
		float ay = adxl345_readY();
		float az = adxl345_readZ();
		sprintf(msg, "ADXL data: %f %f %f\r\n", ax, ay, az);
		logMessage(msg, 1);
		writePin(&PORTG, PG3, LOW);
		delay(250);
	}
	kernel_startTimer();
}

void bmp280_test(){
	kernel_stopTimer();
	char msg[64];
	logMessage("Testing BMP280...", 1);
	bmp280_init();
	for(int i = 0; i < 10; i++){
		writePin(&PORTG, PG3, HIGH);
		delay(250);
		float t = bmp280_readTemperature();
		float p = bmp280_readPressure();
		sprintf(msg, "BMP data: %f %f\r\n", t, p);
		logMessage(msg, 1);
		writePin(&PORTG, PG3, LOW);
		delay(250);
	}
	kernel_startTimer();
}

void ds18b20_test(){
	kernel_stopTimer();
	char msg[64];
	logMessage("Testing DS18B20...", 1);
	for(int i = 0; i < 10; i++){
		writePin(&PORTG, PG3, HIGH);
		delay(250);
		float t = ds18b20_readTemperature();
		sprintf(msg, "DS data: %f\r\n", t);
		logMessage(msg, 1);
		writePin(&PORTG, PG3, LOW);
		delay(250);
	}
	kernel_startTimer();
}