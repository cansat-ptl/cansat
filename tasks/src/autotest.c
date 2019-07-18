/*
 * autotest.c
 *
 * Created: 14.06.2019 19:23:32
 *  Author: ThePetrovich
 */ 

#include "../tasks.h"

float convertToDecimal(float lat);
uint16_t tests EEMEM;
uint16_t tests_r = 0;

void adxl345_test(){
	kernel_stopTimer();
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Testing ADXL345...\r\n"));
	uint8_t devid = adxl345_init();
	debug_logMessage(PGM_OFF, L_INFO, "ADXL device ID: %d\r\n", devid);
	
	for(int i = 0; i < 10; i++){
		wdt_reset();
		hal_writePin(&LED_BUILTIN_PORT, LED_BUILTIN, HIGH);
		hal_writePin(&LED_WRK_PORT, LED_WRK, HIGH);
		delay(100);
		int16_t ax = adxl345_readX();
		int16_t ay = adxl345_readY();
		int16_t az = adxl345_readZ();
		debug_logMessage(PGM_OFF, L_INFO, "ADXL data: %d %d %d\r\n", ax, ay, az);
		hal_writePin(&LED_BUILTIN_PORT, LED_BUILTIN, LOW);
		hal_writePin(&LED_WRK_PORT, LED_WRK, LOW);
		delay(100);
	}
	
	hal_setBit_m(tests_r, ADXL_TESTED);
	eeprom_write_word(&tests, tests_r);
	kernel_startTimer();
	wdt_reset();
}

void bmp280_test(){
	kernel_stopTimer();
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Testing BMP280...\r\n"));
	uint8_t devid = bmp280_init();
	debug_logMessage(PGM_OFF, L_INFO, "BMP device ID: %d\r\n", devid);
	
	//bmp280_printCalibrationData();
	for(int i = 0; i < 10; i++){
		wdt_reset();
		hal_writePin(&LED_BUILTIN_PORT, LED_BUILTIN, HIGH);
		hal_writePin(&LED_WRK_PORT, LED_WRK, HIGH);
		delay(100);
		double t = bmp280_readTemperature();
		delay(100);
		double p = bmp280_readPressure();
		debug_logMessage(PGM_OFF, L_INFO, "BMP data: %f %f\r\n", t, p); 
		hal_writePin(&LED_BUILTIN_PORT, LED_BUILTIN, LOW);
		hal_writePin(&LED_WRK_PORT, LED_WRK, LOW);
		delay(100);
	}
	
	hal_setBit_m(tests_r, BMP_TESTED);
	eeprom_write_word(&tests, tests_r);
	kernel_startTimer();
	wdt_reset();
}

void ds18b20_test(){
	kernel_stopTimer();
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Testing DS18B20...\r\n"));
	for(int i = 0; i < 10; i++){
		wdt_reset();
		hal_writePin(&LED_BUILTIN_PORT, LED_BUILTIN, HIGH);
		hal_writePin(&LED_WRK_PORT, LED_WRK, HIGH);
		delay(100);
		ds18b20_requestTemperature();
		delay(1000);
		char * t = ds18b20_readTemperature();
		debug_logMessage(PGM_OFF, L_INFO, "DS data: %s\r\n", t);
		hal_writePin(&LED_BUILTIN_PORT, LED_BUILTIN, LOW);
		hal_writePin(&LED_WRK_PORT, LED_WRK, LOW);
		delay(100);
	}
	
	hal_setBit_m(tests_r, DS_TESTED);
	eeprom_write_word(&tests, tests_r);
	kernel_startTimer();
	wdt_reset();
}

void imu_test(){
	kernel_stopTimer();
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Testing LSM303...\r\n"));
	imu_init();
	
	int16_t gyrData_raw_x = 0, gyrData_raw_y = 0, gyrData_raw_z = 0;
	int16_t accData_raw_x = 0, accData_raw_y = 0, accData_raw_z = 0;
	int16_t magData_raw_x = 0, magData_raw_y = 0, magData_raw_z = 0;
	for(int i = 0; i < 10; i++){
		wdt_reset();
		hal_writePin(&LED_BUILTIN_PORT, LED_BUILTIN, HIGH);
		hal_writePin(&LED_WRK_PORT, LED_WRK, HIGH);
		delay(100);
		imu_read();
		gyrData_raw_x = gyrData_raw_y = gyrData_raw_z = 0;
		accData_raw_x = accData_raw_y = accData_raw_z = 0;
		
		gyrData_raw_x |= (L3GD.XH << 8);
		gyrData_raw_x |= L3GD.XL;
		gyrData_raw_y |= (L3GD.YH << 8);
		gyrData_raw_y |= L3GD.YL;
		gyrData_raw_z |= (L3GD.ZH << 8);
		gyrData_raw_z |= L3GD.ZL;
		
		accData_raw_x |= (LSM.XH_A << 8);
		accData_raw_x |= LSM.XL_A;
		accData_raw_y |= (LSM.YH_A << 8);
		accData_raw_y |= LSM.YL_A;
		accData_raw_z |= (LSM.ZH_A << 8);
		accData_raw_z |= LSM.ZL_A;
		
		magData_raw_x |= (LSM.XH_M << 8);
		magData_raw_x |= LSM.XL_M;
		magData_raw_y |= (LSM.YH_M << 8);
		magData_raw_y |= LSM.YL_M;
		magData_raw_z |= (LSM.ZH_M << 8);
		magData_raw_z |= LSM.ZL_M;

		debug_logMessage(PGM_OFF, L_INFO, "IMU data: %d %d %d %d %d %d %d %d %d\r\n", gyrData_raw_x, gyrData_raw_y, gyrData_raw_z, accData_raw_x, accData_raw_y, accData_raw_z, magData_raw_x, magData_raw_y, magData_raw_z);
		hal_writePin(&LED_BUILTIN_PORT, LED_BUILTIN, LOW);
		hal_writePin(&LED_WRK_PORT, LED_WRK, LOW);
		delay(100);
	}
	
	hal_setBit_m(tests_r, IMU_TESTED);
	eeprom_write_word(&tests, tests_r);
	kernel_startTimer();
	wdt_reset();
}

void gps_test(){
	kernel_stopTimer();
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Testing GPS...\r\n"));
	for(int i = 0; i < 10; i++){
		wdt_reset();
		hal_writePin(&LED_BUILTIN_PORT, LED_BUILTIN, HIGH);
		hal_writePin(&LED_WRK_PORT, LED_WRK, HIGH);
		delay(100);
		float lat = convertToDecimal(GPS.latitude);
		float lon = convertToDecimal(GPS.longitude);
		debug_logMessage(PGM_OFF, L_INFO, "GPS: %d %d %d %f %f\r\n", GPS.day, GPS.month, GPS.year, lat, lon);
		hal_writePin(&LED_BUILTIN_PORT, LED_BUILTIN, LOW);
		hal_writePin(&LED_WRK_PORT, LED_WRK, LOW);
		delay(100);
	}
	
	hal_setBit_m(tests_r, GPS_TESTED);
	eeprom_write_word(&tests, tests_r);
	kernel_startTimer();
	wdt_reset();
}

void adc_test(){
	kernel_stopTimer();
	
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Testing ADC...\r\n"));
	for(int i = 0; i < 10; i++){
		wdt_reset();
		hal_writePin(&LED_BUILTIN_PORT, LED_BUILTIN, HIGH);
		hal_writePin(&LED_WRK_PORT, LED_WRK, HIGH);
		delay(100);
		int16_t adc0 = adc_read(0);
		int16_t adc1 = adc_read(1);
		debug_logMessage(PGM_OFF, L_INFO, "ADC: %d %d\r\n", adc0, adc1);
		hal_writePin(&LED_BUILTIN_PORT, LED_BUILTIN, LOW);
		hal_writePin(&LED_WRK_PORT, LED_WRK, LOW);
		delay(100);
	}
	
	hal_setBit_m(tests_r, ADC_TESTED);
	eeprom_write_word(&tests, tests_r);
	kernel_startTimer();
	wdt_reset();
}

void autotest(){
	if(!hal_checkBit_m(tests_r, ADXL_TESTED))
		adxl345_test();
	if(!hal_checkBit_m(tests_r, BMP_TESTED))
		bmp280_test();
	if(!hal_checkBit_m(tests_r, DS_TESTED))
		ds18b20_test();
	if(!hal_checkBit_m(tests_r, IMU_TESTED))
		imu_test();
	if(!hal_checkBit_m(tests_r, GPS_TESTED))
		gps_test();
	if(!hal_checkBit_m(tests_r, ADC_TESTED))
		adc_test();
}

void getTestValues(){
	tests_r = eeprom_read_word(&tests);
}