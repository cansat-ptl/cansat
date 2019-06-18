/*
 * readsensors.c
 *
 * Created: 16.06.2019 0:49:48
 *  Author: Admin
 */ 
#include "../tasks.h"

#define DEBUG 1

void readBMP(){
	#ifdef DEBUG
		logMessage(PSTR("Reading BMP280\r\n"), 1, 1);
	#endif
	int16_t t2, alt;
	int32_t prs;
	t2 = bmp280_readTemperature();
	prs = bmp280_readPressure();
	alt = bmp280_calcAltitude(101325);
	sprintf(packetMain.t2, "T2=%d;", t2);
	sprintf(packetMain.prs, "PRS=%ld;", prs);
	sprintf(packetMain.alt, "ALT=%d;", alt);
	kernel_addTask(readBMP, 10);
	wdt_reset();
}

void readADXL(){
	#ifdef DEBUG
		logMessage(PSTR("Reading ADXL345\r\n"), 1, 1);
	#endif
	int16_t ax, ay, az;
	ax = adxl345_readX();
	ay = adxl345_readY();
	az = adxl345_readZ();
	sprintf(packetOrient.ax, "AX=%d;", ax);
	sprintf(packetOrient.ay, "AY=%d;", ay);
	sprintf(packetOrient.az, "AZ=%d;", az);
	kernel_addTask(readADXL, 5);
	wdt_reset();
}

void readDS18(){
	#ifdef DEBUG
		logMessage(PSTR("Reading DS18B20\r\n"), 1, 1);
	#endif
	char * t1 = ds18b20_readTemperature();
	sprintf(packetMain.t1, "T1=%s;", t1);
	kernel_addTask(readDS18, 15);
	wdt_reset();
}

void readIMU(){
	
}