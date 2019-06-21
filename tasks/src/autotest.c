/*
 * autotest.c
 *
 * Created: 14.06.2019 19:23:32
 *  Author: ThePetrovich
 */ 

#include "../tasks.h"
#include "../../kernel/globals.h"

float convertToDecimal(float lat);
uint16_t tests EEMEM;
uint16_t tests_r = 0;

void adxl345_test(){
	kernel_stopTimer();
	char msg[64];
	logMessage((char *)PSTR("Testing ADXL345...\r\n"), 1, 1);
	if(adxl345_init() == ERR_ADXL_DEVID_MISMATCH)
		logMessage((char *)PSTR("ADXL init failure: no ADXL345 connected/DEVID mismatch\r\n"), 3, 1);
	for(int i = 0; i < 10; i++){
		wdt_reset();
		writePin(&PORTG, PG3, HIGH);
		delay(250);
		int16_t ax = adxl345_readX();
		int16_t ay = adxl345_readY();
		int16_t az = adxl345_readZ();
		sprintf(msg, "ADXL data: %d %d %d\r\n", ax, ay, az);
		logMessage(msg, 1, 0);
		writePin(&PORTG, PG3, LOW);
		delay(250);
	}
	setBit_m(tests_r, ADXL_TESTED);
	eeprom_write_word(&tests, tests_r);
	kernel_startTimer();
	wdt_reset();
}

void bmp280_test(){
	kernel_stopTimer();
	char msg[64];
	logMessage((char *)PSTR("Testing BMP280...\r\n"), 1, 1);
	bmp280_init();
	for(int i = 0; i < 10; i++){
		wdt_reset();
		writePin(&PORTG, PG3, HIGH);
		delay(250);
		int16_t t = bmp280_readTemperature();
		delay(100);
		int16_t p = bmp280_readPressure();
		sprintf(msg, "BMP data: %d %d\r\n", t, p);
		logMessage(msg, 1, 0);
		writePin(&PORTG, PG3, LOW);
		delay(250);
	}
	setBit_m(tests_r, BMP_TESTED);
	eeprom_write_word(&tests, tests_r);
	kernel_startTimer();
	wdt_reset();
}

void ds18b20_test(){
	kernel_stopTimer();
	char msg[64];
	logMessage((char *)PSTR("Testing DS18B20...\r\n"), 1, 1);
	for(int i = 0; i < 10; i++){
		wdt_reset();
		writePin(&PORTG, PG3, HIGH);
		delay(250);
		char * t = ds18b20_readTemperature();
		sprintf(msg, "DS data: %s\r\n", t);
		logMessage(msg, 1, 0);
		writePin(&PORTG, PG3, LOW);
		delay(250);
	}
	setBit_m(tests_r, DS_TESTED);
	eeprom_write_word(&tests, tests_r);
	kernel_startTimer();
	wdt_reset();
}

void imu_test(){
	kernel_stopTimer();
	logMessage((char *)PSTR("Testing LSM303...\r\n"), 1, 1);
	imu_init();
	for(int i = 0; i < 10; i++){
		wdt_reset();
		writePin(&PORTG, PG3, HIGH);
		delay(250);
		imu_read();
		logMessage("Reading imu data\r\n", 1, 0);
		writePin(&PORTG, PG3, LOW);
		delay(250);
	}
	setBit_m(tests_r, IMU_TESTED);
	eeprom_write_word(&tests, tests_r);
	kernel_startTimer();
	wdt_reset();
}

void gps_test(){
	kernel_stopTimer();
	wdt_reset();
	logMessage((char *)PSTR("Testing GPS...\r\n"), 1, 1);
	char msg[64];
	for(int i = 0; i < 10; i++){
		wdt_reset();
		writePin(&PORTG, PG3, HIGH);
		delay(250);
		float lat = convertToDecimal(GPS.latitude);
		float lon = convertToDecimal(GPS.longitude);
		sprintf(msg, "GPS: %d %d %d %f %f\r\n", GPS.day, GPS.month, GPS.year, lat, lon);
		logMessage(msg,1, 0);
		writePin(&PORTG, PG3, LOW);
		delay(250);
	}
	setBit_m(tests_r, GPS_TESTED);
	eeprom_write_word(&tests, tests_r);
	kernel_startTimer();
	wdt_reset();
}

void getTestValues(){
	tests_r = eeprom_read_word(&tests);
}